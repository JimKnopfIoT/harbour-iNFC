#include "emulator.h"
#include "nfcdbus.h"
#include "ndefutil.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDebug>

using namespace Nfcd;

// NFC Forum Type 4 Tag NDEF application
static const char* NDEF_AID = "D2760000850101";
#define FID_CC   0xE103
#define FID_NDEF 0xE104

#define SW_OK               0x90, 0x00
#define SW_FILE_NOT_FOUND   0x6A, 0x82
#define SW_WRONG_PARAMS     0x6A, 0x86
#define SW_NOT_SUPPORTED    0x6A, 0x81

#define OBJ_PATH "/org/infc/ndefapp"

// ==========================================================================
// HostAppAdaptor - straight pass through to Emulator
// ==========================================================================

HostAppAdaptor::HostAppAdaptor(Emulator* parent) :
    QDBusAbstractAdaptor(parent),
    iEmulator(parent)
{
    setAutoRelaySignals(false);
}

void HostAppAdaptor::Start(const QDBusObjectPath&)
{
    iEmulator->note(tr("Reader field detected"));
}

void HostAppAdaptor::Restart(const QDBusObjectPath&)
{
    iEmulator->iSelectedFile = 0;
    iEmulator->note(tr("Session restarted"));
}

void HostAppAdaptor::Stop(const QDBusObjectPath&)
{
    iEmulator->iSelectedFile = 0;
    iEmulator->note(tr("Reader gone"));
}

void HostAppAdaptor::ImplicitSelect(const QDBusObjectPath&)
{
    iEmulator->note(tr("Implicitly selected"));
}

void HostAppAdaptor::Select(const QDBusObjectPath&)
{
    iEmulator->note(tr("NDEF application selected"));
}

void HostAppAdaptor::Deselect(const QDBusObjectPath&)
{
    iEmulator->iSelectedFile = 0;
}

void HostAppAdaptor::ResponseStatus(uint, bool)
{
}

QByteArray HostAppAdaptor::Process(const QDBusObjectPath&, uchar cla,
    uchar ins, uchar p1, uchar p2, const QByteArray& data, uint le,
    uchar& sw1, uchar& sw2, uint& responseId)
{
    responseId = 0;
    sw1 = 0x6A; sw2 = 0x81;     // function not supported, until proven otherwise

    if (cla != 0x00) {
        return QByteArray();
    }

    if (ins == 0xA4) {
        // SELECT by file identifier
        if (p1 == 0x00 && data.size() == 2) {
            const quint16 fid = ((quint8)data.at(0) << 8) | (quint8)data.at(1);
            if (fid == FID_CC) {
                iEmulator->iSelectedFile = 1;
                iEmulator->note(tr("SELECT CC file"));
                sw1 = 0x90; sw2 = 0x00;
                return QByteArray();
            }
            if (fid == FID_NDEF) {
                iEmulator->iSelectedFile = 2;
                iEmulator->note(tr("SELECT NDEF file"));
                sw1 = 0x90; sw2 = 0x00;
                return QByteArray();
            }
        }
        sw1 = 0x6A; sw2 = 0x82;
        return QByteArray();
    }

    if (ins == 0xB0) {
        // READ BINARY, offset in P1P2
        const int offset = (p1 << 8) | p2;
        QByteArray file;
        if (iEmulator->iSelectedFile == 1) {
            file = iEmulator->ccFile();
        } else if (iEmulator->iSelectedFile == 2) {
            file = iEmulator->ndefFile();
        } else {
            sw1 = 0x69; sw2 = 0x86;     // command not allowed, no current EF
            return QByteArray();
        }
        if (offset > file.size()) {
            sw1 = 0x6B; sw2 = 0x00;     // wrong parameters, offset outside EF
            return QByteArray();
        }
        int n = (le == 0 || le > 0x100) ? (file.size() - offset) : (int)le;
        if (n > file.size() - offset) n = file.size() - offset;
        iEmulator->note(tr("READ BINARY %1 bytes @ %2").arg(n).arg(offset));
        sw1 = 0x90; sw2 = 0x00;
        return file.mid(offset, n);
    }

    return QByteArray();
}

// ==========================================================================
// Emulator
// ==========================================================================

Emulator::Emulator(QObject* parent) :
    QObject(parent),
    iAdaptor(new HostAppAdaptor(this)),
    iActive(false),
    iModeId(0),
    iParamId(0),
    iSelectedFile(0)
{
    setText(QStringLiteral("iNFC"));
    QDBusConnection::systemBus().registerObject(QStringLiteral(OBJ_PATH),
        this, QDBusConnection::ExportAdaptors);
}

Emulator::~Emulator()
{
    stop();
    QDBusConnection::systemBus().unregisterObject(QStringLiteral(OBJ_PATH));
}

bool Emulator::supported() const
{
    QDBusMessage r = call(QStringLiteral("/nfc0"), IFACE_ADAPTER,
        QStringLiteral("GetSupportedModes"));
    if (r.type() == QDBusMessage::ErrorMessage) return false;
    return r.arguments().value(0).toUInt() & ModeCardEmulation;
}

void Emulator::note(const QString& event)
{
    iLastEvent = event;
    emit lastEventChanged();
}

void Emulator::setText(const QString& text)
{
    iMessage = NdefUtil::buildText(text);
    iSummary = tr("Text: %1").arg(text);
    emit contentChanged();
}

void Emulator::setUri(const QString& uri)
{
    iMessage = NdefUtil::buildUri(uri);
    iSummary = tr("URI: %1").arg(uri);
    emit contentChanged();
}

void Emulator::setRawMessage(const QString& hex)
{
    iMessage = QByteArray::fromHex(hex.toLatin1());
    iSummary = tr("Captured message, %1 bytes").arg(iMessage.size());
    emit contentChanged();
}

void Emulator::setNfcid1(const QString& hex)
{
    if (iNfcid1 == hex) return;
    iNfcid1 = hex;
    emit nfcid1Changed();
}

QByteArray Emulator::ccFile() const
{
    // NFCForum-TS-Type-4-Tag 2.0, capability container
    const int maxNdef = iMessage.size() + 2 + 16;
    QByteArray cc;
    cc.append((char)0x00); cc.append((char)0x0F);   // CCLEN
    cc.append((char)0x20);                          // mapping version 2.0
    cc.append((char)0x00); cc.append((char)0xFF);   // MLe
    cc.append((char)0x00); cc.append((char)0xFF);   // MLc
    cc.append((char)0x04); cc.append((char)0x06);   // NDEF File Control TLV
    cc.append((char)0xE1); cc.append((char)0x04);   // file identifier
    cc.append((char)((maxNdef >> 8) & 0xff));
    cc.append((char)(maxNdef & 0xff));
    cc.append((char)0x00);                          // read access: free
    cc.append((char)0xFF);                          // write access: denied
    return cc;
}

QByteArray Emulator::ndefFile() const
{
    QByteArray f;
    f.append((char)((iMessage.size() >> 8) & 0xff));
    f.append((char)(iMessage.size() & 0xff));
    f.append(iMessage);
    return f;
}

void Emulator::start()
{
    if (iActive) return;

    // Ask nfcd to listen as a card. We deliberately do not disable
    // reader/writer mode: nfcd arbitrates, and leaving it on means a reader
    // and a tag can both be handled while the panel is open.
    QDBusMessage r = call(QString::fromLatin1(PATH_ROOT), IFACE_DAEMON,
        QStringLiteral("RequestMode"),
        QVariantList() << uint(ModeCardEmulation) << uint(0));
    if (r.type() == QDBusMessage::ErrorMessage) {
        note(tr("Cannot enter card emulation: %1").arg(r.errorMessage()));
        return;
    }
    iModeId = r.arguments().value(0).toUInt();

    r = call(QString::fromLatin1(PATH_ROOT), IFACE_DAEMON,
        QStringLiteral("RegisterLocalHostApp"),
        QVariantList() << QVariant::fromValue(QDBusObjectPath(QStringLiteral(OBJ_PATH)))
                       << QStringLiteral("infc")
                       << QVariant::fromValue(QByteArray::fromHex(NDEF_AID))
                       << uint(1));   // allow implicit selection
    if (r.type() == QDBusMessage::ErrorMessage) {
        note(tr("Cannot register NDEF application: %1").arg(r.errorMessage()));
        call(QString::fromLatin1(PATH_ROOT), IFACE_DAEMON,
            QStringLiteral("ReleaseMode"), QVariantList() << iModeId);
        iModeId = 0;
        return;
    }

    // Optional: present a chosen NFCID1. Only 4 byte (single size) values
    // are meaningful here; nfcd 1.2.2+ is required.
    const QByteArray id = fromHex(iNfcid1);
    if (id.size() == 4) {
        QVariantMap params;
        params.insert(QStringLiteral("LA_NFCID1"),
            QVariant::fromValue(QDBusVariant(id)));
        QDBusMessage p = call(QStringLiteral("/nfc0"), IFACE_ADAPTER,
            QStringLiteral("RequestParams"),
            QVariantList() << params << false);
        if (p.type() != QDBusMessage::ErrorMessage) {
            iParamId = p.arguments().value(0).toUInt();
        }
    }

    iActive = true;
    iSelectedFile = 0;
    note(tr("Emulating - hold the phone against a reader"));
    emit activeChanged();
}

void Emulator::stop()
{
    if (!iActive) return;

    call(QString::fromLatin1(PATH_ROOT), IFACE_DAEMON,
        QStringLiteral("UnregisterLocalHostApp"),
        QVariantList() << QVariant::fromValue(
            QDBusObjectPath(QStringLiteral(OBJ_PATH))));

    if (iParamId) {
        call(QStringLiteral("/nfc0"), IFACE_ADAPTER,
            QStringLiteral("ReleaseParams"), QVariantList() << iParamId);
        iParamId = 0;
    }
    if (iModeId) {
        call(QString::fromLatin1(PATH_ROOT), IFACE_DAEMON,
            QStringLiteral("ReleaseMode"), QVariantList() << iModeId);
        iModeId = 0;
    }

    iActive = false;
    iSelectedFile = 0;
    note(tr("Stopped"));
    emit activeChanged();
}
