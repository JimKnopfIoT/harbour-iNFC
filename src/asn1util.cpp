#include "asn1util.h"

#include <QObject>
#include <QStringList>
#include <QVariantMap>

namespace Asn1 {

#define TAG_INTEGER      0x02
#define TAG_OID          0x06
#define TAG_IA5STRING    0x16
#define TAG_SEQUENCE     0x30
#define TAG_SET          0x31

// --------------------------------------------------------------------------
// Minimal DER cursor
// --------------------------------------------------------------------------

class Cursor
{
public:
    Cursor(const QByteArray& d, int p = 0, int e = -1) :
        data(d), pos(p), end(e < 0 ? d.size() : e) {}

    bool atEnd() const { return pos >= end; }
    quint8 peekTag() const { return pos < end ? (quint8)data.at(pos) : 0; }

    // Reads tag and length, returns the contents and advances past them.
    bool next(quint8* tag, QByteArray* contents)
    {
        if (pos + 2 > end) return false;
        *tag = (quint8)data.at(pos++);

        int len = (quint8)data.at(pos++);
        if (len & 0x80) {
            const int n = len & 0x7f;
            if (n == 0 || n > 4 || pos + n > end) return false;
            len = 0;
            for (int i = 0; i < n; i++) {
                len = (len << 8) | (quint8)data.at(pos++);
            }
        }
        if (len < 0 || pos + len > end) return false;
        *contents = data.mid(pos, len);
        pos += len;
        return true;
    }

private:
    const QByteArray& data;
    int pos;
    int end;
};

QString oidToString(const QByteArray& c)
{
    if (c.isEmpty()) return QString();

    QStringList parts;
    const quint8 first = (quint8)c.at(0);
    parts << QString::number(first / 40) << QString::number(first % 40);

    quint64 acc = 0;
    for (int i = 1; i < c.size(); i++) {
        const quint8 b = (quint8)c.at(i);
        acc = (acc << 7) | (b & 0x7f);
        if (!(b & 0x80)) {
            parts << QString::number(acc);
            acc = 0;
        }
    }
    return parts.join(QStringLiteral("."));
}

// --------------------------------------------------------------------------
// BSI TR-03110 protocol families, matched by OID prefix
// --------------------------------------------------------------------------

struct Family {
    const char* prefix;
    const char* name;
};

static const Family FAMILIES[] = {
    { "0.4.0.127.0.7.2.2.1",  "Chip Authentication public key" },
    { "0.4.0.127.0.7.2.2.2",  "Terminal Authentication" },
    { "0.4.0.127.0.7.2.2.3",  "Chip Authentication" },
    { "0.4.0.127.0.7.2.2.4",  "PACE" },
    { "0.4.0.127.0.7.2.2.5",  "Restricted Identification" },
    { "0.4.0.127.0.7.2.2.6",  "Card info locator" },
    { "0.4.0.127.0.7.2.2.7",  "eID security info" },
    { "0.4.0.127.0.7.2.2.8",  "Privileged terminal" },
    { "0.4.0.127.0.7.2.2.9",  "PACE public key" },
    { "0.4.0.127.0.7.2.2.11", "Restricted Identification domain" },
    { "0.4.0.127.0.7.1.2",    "Domain parameters" },
    { "0.4.0.127.0.7.1.1",    "Key agreement" },
    { "0.4.0.127.0.7.2.2",    "TR-03110 protocol" }
};

static const int FAMILY_COUNT = sizeof(FAMILIES) / sizeof(FAMILIES[0]);

static QString familyName(const QString& oid)
{
    // Longest prefix wins, so walk the table and keep the best match.
    QString best;
    int bestLen = 0;
    for (int i = 0; i < FAMILY_COUNT; i++) {
        const QString p = QString::fromLatin1(FAMILIES[i].prefix);
        if ((oid == p || oid.startsWith(p + QLatin1Char('.'))) &&
            p.length() > bestLen) {
            bestLen = p.length();
            best = QString::fromLatin1(FAMILIES[i].name);
        }
    }
    return best.isEmpty() ? QObject::tr("unknown protocol") : best;
}

// standardizedDomainParameters, TR-03110 part 3
static QString domainName(int id)
{
    switch (id) {
    case 0:  return QStringLiteral("1024-bit MODP group");
    case 1:  return QStringLiteral("2048-bit MODP group");
    case 2:  return QStringLiteral("2048-bit MODP group (224 subgroup)");
    case 8:  return QStringLiteral("NIST P-192");
    case 9:  return QStringLiteral("brainpoolP192r1");
    case 10: return QStringLiteral("NIST P-224");
    case 11: return QStringLiteral("brainpoolP224r1");
    case 12: return QStringLiteral("NIST P-256");
    case 13: return QStringLiteral("brainpoolP256r1");
    case 14: return QStringLiteral("brainpoolP320r1");
    case 15: return QStringLiteral("NIST P-384");
    case 16: return QStringLiteral("brainpoolP384r1");
    case 17: return QStringLiteral("brainpoolP512r1");
    case 18: return QStringLiteral("NIST P-521");
    default: return QString();
    }
}

static qint64 integerValue(const QByteArray& c)
{
    qint64 v = 0;
    for (int i = 0; i < c.size() && i < 8; i++) {
        v = (v << 8) | (quint8)c.at(i);
    }
    return v;
}

QVariantList decodeSecurityInfos(const QByteArray& der)
{
    QVariantList out;
    if (der.isEmpty()) return out;

    Cursor top(der);
    quint8 tag = 0;
    QByteArray body;
    if (!top.next(&tag, &body) || (tag != TAG_SET && tag != TAG_SEQUENCE)) {
        return out;
    }

    Cursor entries(body);
    while (!entries.atEnd()) {
        quint8 seqTag = 0;
        QByteArray seq;
        if (!entries.next(&seqTag, &seq)) break;
        if (seqTag != TAG_SEQUENCE) continue;

        Cursor fields(seq);
        QString oid;
        QStringList details;

        while (!fields.atEnd()) {
            quint8 t = 0;
            QByteArray c;
            if (!fields.next(&t, &c)) break;

            if (t == TAG_OID && oid.isEmpty()) {
                oid = oidToString(c);
            } else if (t == TAG_INTEGER) {
                const qint64 n = integerValue(c);
                // In a PACEInfo the second integer is the domain parameter
                // id. Elsewhere it is a version or a key id, so only annotate
                // when the name is actually known.
                const QString dn = details.isEmpty() ? QString() : domainName((int)n);
                details << (dn.isEmpty() ? QString::number(n)
                    : QStringLiteral("%1 (%2)").arg(n).arg(dn));
            } else if (t == TAG_IA5STRING) {
                details << QString::fromLatin1(c);
            } else if (t == TAG_SEQUENCE) {
                // Nested parameter set - show its OID if it has one.
                Cursor inner(c);
                quint8 it = 0;
                QByteArray ic;
                if (inner.next(&it, &ic) && it == TAG_OID) {
                    details << oidToString(ic);
                }
            }
        }

        if (oid.isEmpty()) continue;

        QVariantMap e;
        e.insert(QStringLiteral("label"), familyName(oid));
        e.insert(QStringLiteral("oid"), oid);
        e.insert(QStringLiteral("value"), details.isEmpty() ? oid
            : QStringLiteral("%1\n%2").arg(oid).arg(details.join(QStringLiteral(" · "))));
        out.append(e);
    }

    return out;
}

} // namespace Asn1
