/*
 * iNFC - low level helpers for talking to nfcd over the system bus.
 *
 * Everything in here is deliberately dumb: marshalling, hex formatting and
 * a couple of blocking call wrappers. All policy lives in NfcBackend.
 */
#ifndef NFCDBUS_H
#define NFCDBUS_H

#include <QByteArray>
#include <QDBusMessage>
#include <QString>
#include <QVariantList>
#include <QVariantMap>

namespace Nfcd {

// Well known names on the system bus
extern const char* SERVICE;
extern const char* PATH_ROOT;
extern const char* IFACE_DAEMON;
extern const char* IFACE_ADAPTER;
extern const char* IFACE_TAG;
extern const char* IFACE_TAG_T2;
extern const char* IFACE_ISODEP;
extern const char* IFACE_NDEF;

// Technology bitmask (Daemon.GetTechs, Adapter.GetSupportedTechs)
enum Tech {
    TechNone = 0x00,
    TechA    = 0x01,
    TechB    = 0x02,
    TechF    = 0x04,
    TechAll  = 0x07
};

// Mode bitmask (Daemon.GetMode, Adapter.GetSupportedModes)
enum Mode {
    ModeNone          = 0x00,
    ModeP2PInitiator  = 0x01,
    ModeReaderWriter  = 0x02,
    ModeP2PTarget     = 0x04,
    ModeCardEmulation = 0x08
};

// Tag.GetProtocol enum values
enum Protocol {
    ProtoUnknown = 0,
    ProtoType1   = 1,
    ProtoType2   = 2,
    ProtoType3   = 4,
    ProtoType4A  = 8,
    ProtoType4B  = 16,
    ProtoNfcDep  = 32
};

// Blocking method call. Returns the reply message; check isError() on it.
QDBusMessage call(const QString& path, const QString& iface,
    const QString& method, const QVariantList& args = QVariantList(),
    int timeoutMs = 5000);

// a{sv} -> QVariantMap, unwrapping the inner variants
QVariantMap dictFrom(const QVariant& v);

// Pretty printers
QString hex(const QByteArray& b, char sep = ':');
QString hexPlain(const QByteArray& b);
QByteArray fromHex(const QString& s);
QString protocolName(uint proto);
QString technologyName(uint tech);

} // namespace Nfcd

#endif // NFCDBUS_H
