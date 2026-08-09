#include "ndefutil.h"

#include <QObject>

namespace NdefUtil {

// NFCForum-TS-RTD_URI_1.0, table 3
static const char* URI_PREFIX[] = {
    "", "http://www.", "https://www.", "http://", "https://", "tel:",
    "mailto:", "ftp://anonymous:anonymous@", "ftp://ftp.", "ftps://",
    "sftp://", "smb://", "nfs://", "ftp://", "dav://", "news:",
    "telnet://", "imap:", "rtsp://", "urn:", "pop:", "sip:", "sips:",
    "tftp:", "btspp://", "btl2cap://", "btgoep://", "tcpobex://",
    "irdaobex://", "file://", "urn:epc:id:", "urn:epc:tag:",
    "urn:epc:pat:", "urn:epc:raw:", "urn:epc:", "urn:nfc:"
};
static const int URI_PREFIX_COUNT = sizeof(URI_PREFIX) / sizeof(URI_PREFIX[0]);

QString expandUri(const QByteArray& payload)
{
    if (payload.isEmpty()) return QString();
    const quint8 code = (quint8)payload.at(0);
    const QString rest = QString::fromUtf8(payload.mid(1));
    if (code < URI_PREFIX_COUNT) {
        return QString::fromLatin1(URI_PREFIX[code]) + rest;
    }
    return rest;
}

static QString tnfName(uint tnf)
{
    switch (tnf) {
    case TnfEmpty:       return QStringLiteral("empty");
    case TnfWellKnown:   return QStringLiteral("NFC Forum well-known");
    case TnfMediaType:   return QStringLiteral("media type");
    case TnfAbsoluteUri: return QStringLiteral("absolute URI");
    case TnfExternal:    return QStringLiteral("external type");
    default:             return QStringLiteral("unknown");
    }
}

QVariantMap describe(uint tnf, const QByteArray& type, const QByteArray& id,
    const QByteArray& payload)
{
    QVariantMap m;
    m.insert(QStringLiteral("tnf"), tnf);
    m.insert(QStringLiteral("tnfName"), tnfName(tnf));
    m.insert(QStringLiteral("type"), QString::fromLatin1(type));
    m.insert(QStringLiteral("id"), QString::fromLatin1(id.toHex()).toUpper());
    m.insert(QStringLiteral("raw"), QString::fromLatin1(payload.toHex()).toUpper());
    m.insert(QStringLiteral("uri"), QString());

    QString kind = QStringLiteral("other");
    QString title = QString::fromLatin1(type);
    QString text;

    if (tnf == TnfWellKnown && type == "T") {
        // Text record: status byte, language code, UTF-8/16 text
        if (!payload.isEmpty()) {
            const quint8 status = (quint8)payload.at(0);
            const int langLen = status & 0x3f;
            const QByteArray body = payload.mid(1 + langLen);
            text = (status & 0x80) ? QString::fromUtf16(
                (const ushort*)body.constData(), body.size() / 2)
                : QString::fromUtf8(body);
            kind = QStringLiteral("text");
            title = QObject::tr("Text (%1)")
                .arg(QString::fromLatin1(payload.mid(1, langLen)));
        }
    } else if (tnf == TnfWellKnown && type == "U") {
        kind = QStringLiteral("uri");
        text = expandUri(payload);
        m.insert(QStringLiteral("uri"), text);
        title = QObject::tr("URI");
    } else if (tnf == TnfWellKnown && type == "Sp") {
        kind = QStringLiteral("smartposter");
        title = QObject::tr("Smart poster");
        text = QObject::tr("%1 bytes of nested records").arg(payload.size());
    } else if (tnf == TnfMediaType && type == "text/vcard") {
        kind = QStringLiteral("vcard");
        title = QObject::tr("Contact (vCard)");
        text = QString::fromUtf8(payload);
    } else if (tnf == TnfMediaType &&
               type == "application/vnd.wfa.wsc") {
        kind = QStringLiteral("wifi");
        title = QObject::tr("Wi-Fi credentials");
        text = QObject::tr("%1 bytes, WSC encoded").arg(payload.size());
    } else if (tnf == TnfMediaType) {
        kind = QStringLiteral("media");
        text = QString::fromUtf8(payload);
    } else if (tnf == TnfAbsoluteUri) {
        kind = QStringLiteral("uri");
        text = QString::fromUtf8(payload);
        m.insert(QStringLiteral("uri"), text);
        title = QObject::tr("Absolute URI");
    } else {
        text = QString::fromLatin1(payload.toHex()).toUpper();
    }

    m.insert(QStringLiteral("kind"), kind);
    m.insert(QStringLiteral("title"), title);
    m.insert(QStringLiteral("text"), text);
    return m;
}

// Wrap a single record into a complete message: MB and ME both set, short
// record form when the payload allows it.
static QByteArray wrap(quint8 tnf, const QByteArray& type,
    const QByteArray& payload)
{
    QByteArray out;
    const bool shortRecord = payload.size() < 256;
    quint8 flags = 0xc0 | tnf;           // MB | ME | TNF
    if (shortRecord) flags |= 0x10;      // SR

    out.append((char)flags);
    out.append((char)type.size());
    if (shortRecord) {
        out.append((char)payload.size());
    } else {
        const quint32 n = payload.size();
        out.append((char)((n >> 24) & 0xff));
        out.append((char)((n >> 16) & 0xff));
        out.append((char)((n >> 8) & 0xff));
        out.append((char)(n & 0xff));
    }
    out.append(type);
    out.append(payload);
    return out;
}

QByteArray buildText(const QString& text, const QString& lang)
{
    const QByteArray l = lang.toLatin1().left(63);
    QByteArray payload;
    payload.append((char)(l.size()));    // UTF-8, language length
    payload.append(l);
    payload.append(text.toUtf8());
    return wrap(TnfWellKnown, QByteArrayLiteral("T"), payload);
}

QByteArray buildUri(const QString& uri)
{
    quint8 code = 0;
    QString rest = uri;
    // Longest matching prefix wins, so scan all of them.
    int bestLen = 0;
    for (int i = 1; i < URI_PREFIX_COUNT; i++) {
        const QString p = QString::fromLatin1(URI_PREFIX[i]);
        if (uri.startsWith(p) && p.length() > bestLen) {
            bestLen = p.length();
            code = (quint8)i;
        }
    }
    if (bestLen) rest = uri.mid(bestLen);

    QByteArray payload;
    payload.append((char)code);
    payload.append(rest.toUtf8());
    return wrap(TnfWellKnown, QByteArrayLiteral("U"), payload);
}

} // namespace NdefUtil
