#include "nfcdbus.h"

#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusVariant>

namespace Nfcd {

const char* SERVICE       = "org.sailfishos.nfc.daemon";
const char* PATH_ROOT     = "/";
const char* IFACE_DAEMON  = "org.sailfishos.nfc.Daemon";
const char* IFACE_ADAPTER = "org.sailfishos.nfc.Adapter";
const char* IFACE_TAG     = "org.sailfishos.nfc.Tag";
const char* IFACE_TAG_T2  = "org.sailfishos.nfc.TagType2";
const char* IFACE_ISODEP  = "org.sailfishos.nfc.IsoDep";
const char* IFACE_NDEF    = "org.sailfishos.nfc.NDEF";

QDBusMessage call(const QString& path, const QString& iface,
    const QString& method, const QVariantList& args, int timeoutMs)
{
    QDBusMessage msg = QDBusMessage::createMethodCall(
        QString::fromLatin1(SERVICE), path, iface, method);
    msg.setArguments(args);
    return QDBusConnection::systemBus().call(msg, QDBus::Block, timeoutMs);
}

QVariantMap dictFrom(const QVariant& v)
{
    // a{sv} arrives as a QDBusArgument. Let QtDBus do the demarshalling - the
    // hand rolled version guarded on currentType() and silently produced
    // nothing, which cost us every poll parameter and every activation
    // parameter on the first real card.
    QVariantMap out;
    if (v.canConvert<QDBusArgument>()) {
        out = qdbus_cast<QVariantMap>(v.value<QDBusArgument>());
    } else if (v.type() == QVariant::Map) {
        out = v.toMap();
    }

    // Values may still be wrapped in a QDBusVariant one level down.
    for (QVariantMap::iterator it = out.begin(); it != out.end(); ++it) {
        if (it.value().canConvert<QDBusVariant>()) {
            it.value() = it.value().value<QDBusVariant>().variant();
        }
    }
    return out;
}

QString hex(const QByteArray& b, char sep)
{
    QString s;
    s.reserve(b.size() * 3);
    for (int i = 0; i < b.size(); i++) {
        if (i) s.append(QChar::fromLatin1(sep));
        s.append(QString("%1").arg((quint8)b.at(i), 2, 16, QChar('0')).toUpper());
    }
    return s;
}

QString hexPlain(const QByteArray& b)
{
    return QString::fromLatin1(b.toHex()).toUpper();
}

QByteArray fromHex(const QString& s)
{
    QString clean = s;
    clean.remove(QRegExp("[^0-9A-Fa-f]"));
    if (clean.size() & 1) clean.chop(1);
    return QByteArray::fromHex(clean.toLatin1());
}

QString protocolName(uint proto)
{
    switch (proto) {
    case ProtoType1:  return QStringLiteral("Type 1 Tag");
    case ProtoType2:  return QStringLiteral("Type 2 Tag");
    case ProtoType3:  return QStringLiteral("Type 3 Tag");
    case ProtoType4A: return QStringLiteral("Type 4A (ISO-DEP)");
    case ProtoType4B: return QStringLiteral("Type 4B (ISO-DEP)");
    case ProtoNfcDep: return QStringLiteral("NFC-DEP (P2P)");
    default:          return QStringLiteral("unknown");
    }
}

QString technologyName(uint tech)
{
    switch (tech) {
    case TechA: return QStringLiteral("NFC-A");
    case TechB: return QStringLiteral("NFC-B");
    case TechF: return QStringLiteral("NFC-F");
    default:    return QStringLiteral("unknown");
    }
}

} // namespace Nfcd
