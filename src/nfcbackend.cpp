#include "nfcbackend.h"
#include "asn1util.h"
#include "nfcdbus.h"
#include "ndefutil.h"

#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusMetaType>
#include <QDBusObjectPath>
#include <QDBusVariant>
#include <QDebug>

using namespace Nfcd;

// ==========================================================================
// Application probe table
//
// Every entry is a plain SELECT BY DF NAME. Read only, no secrets involved,
// nothing is modified on the card. A card that answers 6982 ("security status
// not satisfied") still proves the application is there - that is exactly the
// case for the eID application, which refuses to talk without a terminal
// certificate but does not pretend to be absent.
// ==========================================================================

struct AppProbe {
    const char* aid;        // hex, no separators
    const char* name;       // shown to the user
    const char* kind;       // classification hint, empty = no hint
};

static const AppProbe PROBES[] = {
    { "325041592E5359532E4444463031", "PPSE (contactless payment)", "payment" },
    { "A0000000031010",               "Visa credit/debit",          "payment" },
    { "A0000000041010",               "Mastercard credit/debit",    "payment" },
    { "A00000005945430100",           "girocard (EAPS)",            "payment" },
    { "D27600000102",                 "DF.HCA (health card)",       "egk"     },
    { "D2760001448000",               "gematik root",               "egk"     },
    { "E80704007F00070302",           "eID application",            "eid"     },
    { "A0000002471001",               "ePassport (ICAO LDS1)",      "epass"   },
    { "D2760000850101",               "NDEF Type 4 Tag",            "ndef"    },
    { "D27600012401",                 "OpenPGP card",               "token"   },
    { "A0000005272101",               "YubiKey OTP",                "token"   },
    { "A0000006472F0001",             "FIDO2 / U2F",                "token"   },
    { "A000000308000010000100",       "PIV",                        "token"   }
};

static const int PROBE_COUNT = sizeof(PROBES) / sizeof(PROBES[0]);

// ==========================================================================
// ATS decoding (ISO/IEC 14443-4)
//
// The activation parameters of an NFC-A card are the interface bytes of its
// Answer To Select. They say nothing about what is on the card and everything
// about how one is allowed to talk to it.
// ==========================================================================

// FSCI -> maximum frame size in bytes
static const int FSCI_TABLE[16] = {
    16, 24, 32, 40, 48, 64, 96, 128, 256, 256, 256, 256, 256, 256, 256, 256
};

static QString describeT0(quint8 t0)
{
    const int fsci = t0 & 0x0f;
    QStringList present;
    if (t0 & 0x10) present << QStringLiteral("TA");
    if (t0 & 0x20) present << QStringLiteral("TB");
    if (t0 & 0x40) present << QStringLiteral("TC");

    QString s = QObject::tr("max frame %1 bytes (FSCI %2)")
        .arg(FSCI_TABLE[fsci]).arg(fsci);
    if (!present.isEmpty()) {
        s += QStringLiteral(" · ") + QObject::tr("%1 follow")
            .arg(present.join(QStringLiteral(", ")));
    }
    return s;
}

static QString describeTa(quint8 ta)
{
    // b3..b1 = card to reader, b7..b5 = reader to card, in multiples of 106.
    QStringList ds, dr;
    if (ta & 0x01) dr << QStringLiteral("212");
    if (ta & 0x02) dr << QStringLiteral("424");
    if (ta & 0x04) dr << QStringLiteral("848");
    if (ta & 0x10) ds << QStringLiteral("212");
    if (ta & 0x20) ds << QStringLiteral("424");
    if (ta & 0x40) ds << QStringLiteral("848");

    QString s;
    if (ds.isEmpty() && dr.isEmpty()) {
        s = QObject::tr("106 kbit/s only");
    } else {
        s = QObject::tr("106 plus %1 kbit/s to card, %2 to reader")
            .arg(dr.isEmpty() ? QObject::tr("none") : dr.join(QStringLiteral("/")))
            .arg(ds.isEmpty() ? QObject::tr("none") : ds.join(QStringLiteral("/")));
    }
    if (ta & 0x80) {
        s += QStringLiteral(" · ") + QObject::tr("same rate required both ways");
    }
    return s;
}

static QString describeTb(quint8 tb)
{
    // Base unit is 256 * 16 / 13.56 MHz = 302 us.
    const int fwi = (tb >> 4) & 0x0f;
    const int sfgi = tb & 0x0f;
    const double fwtMs = 0.302 * (double)(1u << fwi);
    const double sfgtMs = 0.302 * (double)(1u << sfgi);

    return QObject::tr("waiting time ~%1 ms (FWI %2) · guard time ~%3 ms (SFGI %4)")
        .arg(fwtMs, 0, 'f', fwtMs < 10 ? 2 : 0).arg(fwi)
        .arg(sfgtMs, 0, 'f', 2).arg(sfgi);
}

static QString describeTc(quint8 tc)
{
    QStringList l;
    l << ((tc & 0x02) ? QObject::tr("CID supported")
                      : QObject::tr("no CID"));
    l << ((tc & 0x01) ? QObject::tr("NAD supported")
                      : QObject::tr("no NAD"));
    return l.join(QStringLiteral(" · "));
}

// ==========================================================================
// NfcBackend
// ==========================================================================

NfcBackend::NfcBackend(QObject* parent) :
    QObject(parent),
    iDaemonPresent(false),
    iAdapterEnabled(false),
    iSupportedModes(0),
    iSupportedTechs(0),
    iState(Idle),
    iBlockId(0),
    iKindPanel(PanelRead),
    iSak(0),
    iHasCardAccess(false),
    iArmTimer(new QTimer(this)),
    iTargetTimer(new QTimer(this))
{
    iArmTimer->setSingleShot(true);
    iArmTimer->setInterval(25000);
    connect(iArmTimer, SIGNAL(timeout()), SLOT(onArmTimeout()));

    iTargetTimer->setSingleShot(true);
    iTargetTimer->setInterval(2500);
    connect(iTargetTimer, SIGNAL(timeout()), SLOT(onUnreadableTarget()));

    QDBusConnection bus = QDBusConnection::systemBus();
    bus.connect(QString(), QStringLiteral("/org/freedesktop/DBus"),
        QStringLiteral("org.freedesktop.DBus"),
        QStringLiteral("NameOwnerChanged"),
        this, SLOT(onNameOwnerChanged(QString,QString,QString)));

    connectDaemon();
    iStatusText = tr("Ready - pull down to read");
}

NfcBackend::~NfcBackend()
{
    unblockPolling();
}

QString NfcBackend::supportedTechs() const
{
    QStringList l;
    if (iSupportedTechs & TechA) l << QStringLiteral("NFC-A");
    if (iSupportedTechs & TechB) l << QStringLiteral("NFC-B");
    if (iSupportedTechs & TechF) l << QStringLiteral("NFC-F");
    return l.isEmpty() ? tr("none") : l.join(QStringLiteral(", "));
}

void NfcBackend::onNameOwnerChanged(const QString& name, const QString&,
    const QString& newOwner)
{
    if (name == QLatin1String(SERVICE)) {
        if (newOwner.isEmpty()) {
            iDaemonPresent = false;
            iAdapterPath.clear();
            iBlockId = 0;
            emit daemonChanged();
            setState(Failed, tr("nfcd went away"));
        } else {
            connectDaemon();
        }
    }
}

void NfcBackend::connectDaemon()
{
    QDBusMessage r = call(QString::fromLatin1(PATH_ROOT), IFACE_DAEMON,
        QStringLiteral("GetAll4"));
    if (r.type() == QDBusMessage::ErrorMessage) {
        iDaemonPresent = false;
        emit daemonChanged();
        return;
    }

    const QVariantList a = r.arguments();
    // version, adapters(ao), daemon_version, mode, techs
    if (a.count() >= 3) {
        // nfc_version.h: major in 7 bits, minor and release in 12 each.
        const int v = a.at(2).toInt();
        iDaemonVersion = QStringLiteral("%1.%2.%3")
            .arg((v >> 24) & 0x7f).arg((v >> 12) & 0xfff).arg(v & 0xfff);
    }
    const QList<QDBusObjectPath> adapters =
        qdbus_cast<QList<QDBusObjectPath> >(a.value(1));
    if (!adapters.isEmpty()) {
        iAdapterPath = adapters.first().path();
    }
    iDaemonPresent = true;

    refreshAdapter();

    if (!iAdapterPath.isEmpty()) {
        QDBusConnection::systemBus().connect(QString::fromLatin1(SERVICE),
            iAdapterPath, QString::fromLatin1(IFACE_ADAPTER),
            QStringLiteral("TagsChanged"), this,
            SLOT(onTagsChanged(QList<QDBusObjectPath>)));
        // Tells us something entered the field even when nfcd cannot offer
        // an interface for it - the MIFARE Classic case.
        QDBusConnection::systemBus().connect(QString::fromLatin1(SERVICE),
            iAdapterPath, QString::fromLatin1(IFACE_ADAPTER),
            QStringLiteral("TargetPresentChanged"), this,
            SLOT(onTargetPresentChanged(bool)));
    }

    // Quiet by default. This is the whole point of the app.
    blockPolling();
    emit daemonChanged();
}

void NfcBackend::refreshAdapter()
{
    if (iAdapterPath.isEmpty()) return;

    QDBusMessage r = call(iAdapterPath, IFACE_ADAPTER, QStringLiteral("GetAll4"));
    if (r.type() == QDBusMessage::ErrorMessage) return;

    const QVariantList a = r.arguments();
    // version, enabled, powered, supported_modes, mode, target_present,
    // tags, peers, hosts, supported_techs, params
    iAdapterEnabled = a.value(1).toBool();
    iSupportedModes = a.value(3).toUInt();
    iSupportedTechs = a.value(9).toUInt();
}

void NfcBackend::blockPolling()
{
    if (iBlockId || iAdapterPath.isEmpty()) return;

    QDBusMessage r = call(QString::fromLatin1(PATH_ROOT), IFACE_DAEMON,
        QStringLiteral("RequestTechs"),
        QVariantList() << uint(TechNone) << uint(TechAll));
    if (r.type() != QDBusMessage::ErrorMessage) {
        iBlockId = r.arguments().value(0).toUInt();
    }
}

void NfcBackend::unblockPolling()
{
    if (!iBlockId) return;

    call(QString::fromLatin1(PATH_ROOT), IFACE_DAEMON,
        QStringLiteral("ReleaseTechs"), QVariantList() << iBlockId);
    iBlockId = 0;
}

void NfcBackend::setState(State state, const QString& text)
{
    iState = state;
    if (!text.isNull()) {
        iStatusText = text;
    }
    emit stateChanged();
}

void NfcBackend::handOff()
{
    if (iFields.isEmpty()) return;

    iDetailFields = iFields;
    iDetailRecords = iRecords;
    iDetailKind = iKind;
    iDetailKindLabel = iKindLabel;
    iDetailUid = iUid;
    iDetailFrequency = iFrequency;
    iDetailType2Dump = iType2Dump;   // carry the raw dump across, or a save
                                     // from a detail panel loses it

    // The Read panel goes back to being empty. Deliberately not a copy: the
    // result exists once, and it is now over there. That includes the status
    // line - "Read Payment card" above an empty page is exactly the
    // ambiguity this hand over is meant to remove.
    clear();
    setState(Idle, tr("Ready - pull down to read"));
    emit detailChanged();
}

void NfcBackend::arm()
{
    if (!iDaemonPresent) {
        setState(Failed, tr("nfcd is not running"));
        return;
    }
    if (!iAdapterEnabled) {
        setState(Failed, tr("NFC is off - enable it in Settings"));
        return;
    }
    // Empty the panel the moment a read is requested, not when it finishes.
    // Otherwise the old values sit there while you are already holding the
    // next card, and you cannot tell which is which.
    clear();

    unblockPolling();
    iArmTimer->start();
    setState(Armed, tr("Hold a tag against the back of the phone"));
}

void NfcBackend::disarm()
{
    iArmTimer->stop();
    iTargetTimer->stop();
    iPendingWrite.clear();
    blockPolling();
    if (iState == Armed) {
        setState(Idle, tr("Ready - pull down to read"));
    } else {
        setState(iState == Reading ? Idle : (State)iState, QString());
    }
}

void NfcBackend::onArmTimeout()
{
    if (iState != Armed) return;
    iTargetTimer->stop();
    iPendingWrite.clear();
    blockPolling();
    setState(Failed, tr("Nothing showed up. Pull down to try again."));
}

void NfcBackend::onTargetPresentChanged(bool present)
{
    if (iState != Armed) return;
    if (present) {
        // Give nfcd a moment to produce a tag object. If it does not, the
        // card is one the stack cannot talk to.
        iTargetTimer->start();
    } else {
        iTargetTimer->stop();
    }
}

void NfcBackend::onUnreadableTarget()
{
    if (iState != Armed) return;

    iArmTimer->stop();
    clear();

    // Whatever it was, it answered in the 13.56 MHz field - that is the only
    // band the phone polls.
    iFrequency = QStringLiteral("13.56 MHz");

    const QString g = tr("Detection");
    addField(g, tr("Target"), tr("detected in the field"));
    addField(g, tr("Interface"), tr("none offered by nfcd"));

    iKind = QStringLiteral("unsupported");
    iKindLabel = tr("Card the stack cannot talk to");
    iKindPanel = PanelOther;

    blockPolling();
    setState(Done, iKindLabel);
    emit resultChanged();
    emit tagRead();
}

void NfcBackend::clear()
{
    iFields.clear();
    iRecords.clear();
    iFoundApps.clear();
    iKind.clear();
    iKindLabel.clear();
    iKindPanel = PanelRead;
    iUid.clear();
    iFrequency.clear();
    iSak = 0;
    iHasCardAccess = false;
    iType2Dump.clear();
    emit resultChanged();
}

void NfcBackend::armWriteText(const QString& text)
{
    iPendingWrite = NdefUtil::buildText(text);
    arm();
    if (iState == Armed) {
        setState(Armed, tr("Hold a writeable tag against the phone"));
    }
}

void NfcBackend::armWriteUri(const QString& uri)
{
    iPendingWrite = NdefUtil::buildUri(uri);
    arm();
    if (iState == Armed) {
        setState(Armed, tr("Hold a writeable tag against the phone"));
    }
}

void NfcBackend::onTagsChanged(const QList<QDBusObjectPath>& tags)
{
    if (iState != Armed || tags.isEmpty()) return;
    if (!iPendingWrite.isEmpty()) {
        writeTag(tags.first().path());
    } else {
        readTag(tags.first().path());
    }
}

void NfcBackend::writeTag(const QString& tagPath)
{
    iArmTimer->stop();
    iTargetTimer->stop();
    setState(Reading, tr("Writing..."));

    const QByteArray message = iPendingWrite;
    iPendingWrite.clear();

    // NDEF message TLV: T=0x03, L, V..., terminator TLV 0xFE
    QByteArray tlv;
    tlv.append((char)0x03);
    if (message.size() < 255) {
        tlv.append((char)message.size());
    } else {
        tlv.append((char)0xFF);
        tlv.append((char)((message.size() >> 8) & 0xff));
        tlv.append((char)(message.size() & 0xff));
    }
    tlv.append(message);
    tlv.append((char)0xFE);

    call(tagPath, IFACE_TAG, QStringLiteral("Acquire2"), QVariantList() << false);

    QDBusMessage r = call(tagPath, IFACE_TAG_T2, QStringLiteral("WriteData"),
        QVariantList() << uint(0) << QVariant::fromValue(tlv), 15000);

    call(tagPath, IFACE_TAG, QStringLiteral("Release2"));
    blockPolling();

    if (r.type() == QDBusMessage::ErrorMessage) {
        setState(Failed, tr("Write failed: %1").arg(r.errorMessage()));
    } else {
        const uint written = r.arguments().value(0).toUInt();
        setState(Done, tr("Wrote %1 bytes").arg(written));
    }
}

void NfcBackend::addField(const QString& group, const QString& label,
    const QString& value, bool emittable, bool mono)
{
    QVariantMap m;
    m.insert(QStringLiteral("group"), group);
    m.insert(QStringLiteral("label"), label);
    m.insert(QStringLiteral("value"), value);
    m.insert(QStringLiteral("emittable"), emittable);
    m.insert(QStringLiteral("mono"), mono);
    iFields.append(m);
}

void NfcBackend::readTag(const QString& tagPath)
{
    iArmTimer->stop();
    iTargetTimer->stop();
    setState(Reading, tr("Reading..."));
    clear();

    // The phone's radio is 13.56 MHz, full stop. There is no other band it
    // can reach - LF and UHF need different hardware.
    iFrequency = QStringLiteral("13.56 MHz");

    QDBusMessage r = call(tagPath, IFACE_TAG, QStringLiteral("GetAll3"));
    if (r.type() == QDBusMessage::ErrorMessage) {
        blockPolling();
        setState(Failed, tr("Could not read the tag: %1").arg(r.errorMessage()));
        return;
    }

    const QVariantList a = r.arguments();
    // version, present, technology, protocol, type, interfaces,
    // ndef_records, poll_parameters
    const uint tech  = a.value(2).toUInt();
    const uint proto = a.value(3).toUInt();
    const QStringList interfaces = a.value(5).toStringList();
    const QList<QDBusObjectPath> ndefPaths =
        qdbus_cast<QList<QDBusObjectPath> >(a.value(6));
    const QVariantMap pollParams = dictFrom(a.value(7));

    // Hold the tag for the duration of the read so nfcd does not drop it
    // between our calls.
    call(tagPath, IFACE_TAG, QStringLiteral("Acquire2"),
        QVariantList() << false);

    readTransport(tagPath, tech, proto, pollParams);

    if (interfaces.contains(QLatin1String(IFACE_TAG_T2))) {
        readType2(tagPath);
    }
    if (interfaces.contains(QLatin1String(IFACE_ISODEP))) {
        readIsoDep(tagPath);
        probeApplications(tagPath);
    }

    QStringList recordPaths;
    for (int i = 0; i < ndefPaths.count(); i++) {
        recordPaths.append(ndefPaths.at(i).path());
    }
    readNdef(recordPaths);
    decodeBrushHead();

    classify(proto, interfaces);

    call(tagPath, IFACE_TAG, QStringLiteral("Release2"));

    // One read per arm(). Radio goes quiet again immediately.
    blockPolling();
    setState(Done, tr("Read %1").arg(iKindLabel));
    emit resultChanged();
    emit tagRead();
}

void NfcBackend::readTransport(const QString& tagPath, uint tech, uint proto,
    const QVariantMap& pollParams)
{
    Q_UNUSED(tagPath)

    const QString g = tr("Transport");
    addField(g, tr("Technology"), technologyName(tech));
    addField(g, tr("Protocol"), protocolName(proto));

    if (pollParams.contains(QStringLiteral("NFCID1"))) {
        const QByteArray id = pollParams.value(QStringLiteral("NFCID1")).toByteArray();
        iUid = hex(id);
        // A 4 byte NFCID1 is a single size identifier, which is what
        // Adapter LA_NFCID1 can be set to. Longer ones cannot be emulated.
        addField(g, tr("NFCID1 (UID)"), iUid, id.size() == 4, true);
        if (id.size() == 4 && (quint8)id.at(0) == 0x08) {
            addField(g, tr("UID type"), tr("random (re-generated per session)"));
        }
    }
    if (pollParams.contains(QStringLiteral("SEL_RES"))) {
        const QByteArray sel = pollParams.value(QStringLiteral("SEL_RES")).toByteArray();
        if (!sel.isEmpty()) {
            iSak = (quint8)sel.at(0);
            addField(g, tr("SAK (SEL_RES)"), hexPlain(sel), false, true);
        }
    }
    if (pollParams.contains(QStringLiteral("NFCID0"))) {
        const QByteArray id = pollParams.value(QStringLiteral("NFCID0")).toByteArray();
        iUid = hex(id);
        addField(g, tr("NFCID0"), iUid, false, true);
    }
    if (pollParams.contains(QStringLiteral("APPDATA"))) {
        addField(g, tr("Application data"),
            hexPlain(pollParams.value(QStringLiteral("APPDATA")).toByteArray()),
            false, true);
    }
    if (pollParams.contains(QStringLiteral("PROTINFO"))) {
        addField(g, tr("Protocol info"),
            hexPlain(pollParams.value(QStringLiteral("PROTINFO")).toByteArray()),
            false, true);
    }
}

void NfcBackend::readType2(const QString& tagPath)
{
    const QString g = tr("Memory");

    QDBusMessage r = call(tagPath, IFACE_TAG_T2, QStringLiteral("GetAll"));
    if (r.type() != QDBusMessage::ErrorMessage) {
        const QVariantList a = r.arguments();
        addField(g, tr("Block size"), QString::number(a.value(1).toUInt()));
        addField(g, tr("Data size"), tr("%1 bytes").arg(a.value(2).toUInt()));
        const QByteArray serial = a.value(3).toByteArray();
        if (!serial.isEmpty()) {
            addField(g, tr("Serial"), hex(serial), false, true);
        }
    }

    r = call(tagPath, IFACE_TAG_T2, QStringLiteral("ReadAllData"), QVariantList(), 10000);
    if (r.type() != QDBusMessage::ErrorMessage) {
        iType2Dump = r.arguments().value(0).toByteArray();
        if (!iType2Dump.isEmpty()) {
            addField(g, tr("Dump"), hexPlain(iType2Dump), false, true);
        }
    }
}

void NfcBackend::readIsoDep(const QString& tagPath)
{
    const QString g = tr("ISO-DEP");

    QDBusMessage r = call(tagPath, IFACE_ISODEP,
        QStringLiteral("GetActivationParameters"));
    if (r.type() != QDBusMessage::ErrorMessage) {
        const QVariantMap p = dictFrom(r.arguments().value(0));
        if (p.contains(QStringLiteral("HB"))) {
            addField(g, tr("Historical bytes"),
                hexPlain(p.value(QStringLiteral("HB")).toByteArray()), false, true);
        }
        // The ATS interface bytes, spelled out. The raw value stays in the
        // label so nothing is hidden, but the reading is what is useful.
        for (int i = 0; i < 4; i++) {
            static const char* keys[] = { "T0", "TA", "TB", "TC" };
            const QString key = QString::fromLatin1(keys[i]);
            if (!p.contains(key)) continue;

            const quint8 b = (quint8)p.value(key).toUInt();
            QString meaning;
            switch (i) {
            case 0: meaning = describeT0(b); break;
            case 1: meaning = describeTa(b); break;
            case 2: meaning = describeTb(b); break;
            case 3: meaning = describeTc(b); break;
            }
            addField(g, tr("%1 (format byte %2)", "ATS interface byte")
                .arg(key).arg(QString("0x%1")
                    .arg(b, 2, 16, QChar('0')).toUpper()), meaning);
        }
        if (p.contains(QStringLiteral("MBLI"))) {
            const uint mbli = p.value(QStringLiteral("MBLI")).toUInt();
            addField(g, tr("MBLI"), mbli
                ? tr("%1 - card buffer is %2x the frame size").arg(mbli).arg(1u << (mbli - 1))
                : tr("0 - no buffer information"));
        }
        if (p.contains(QStringLiteral("DID"))) {
            addField(g, tr("DID"),
                QString::number(p.value(QStringLiteral("DID")).toUInt()));
        }
    }

    // EF.CardAccess is readable without any secret - it is how a terminal
    // learns which PACE parameters the card offers. Its mere presence tells
    // us we are looking at a PACE protected document.
    QDBusMessage sel = call(tagPath, IFACE_ISODEP, QStringLiteral("Transmit"),
        QVariantList() << QVariant::fromValue(quint8(0x00))
                       << QVariant::fromValue(quint8(0xA4))
                       << QVariant::fromValue(quint8(0x00))
                       << QVariant::fromValue(quint8(0x0C))
                       << QVariant::fromValue(QByteArray::fromHex("3F00"))
                       << uint(0));
    Q_UNUSED(sel)

    QDBusMessage rb = call(tagPath, IFACE_ISODEP, QStringLiteral("Transmit"),
        QVariantList() << QVariant::fromValue(quint8(0x00))
                       << QVariant::fromValue(quint8(0xB0))
                       << QVariant::fromValue(quint8(0x9C))
                       << QVariant::fromValue(quint8(0x00))
                       << QVariant::fromValue(QByteArray())
                       << uint(0x100));
    if (rb.type() != QDBusMessage::ErrorMessage) {
        const QVariantList a = rb.arguments();
        const quint8 sw1 = a.value(1).toUInt();
        const quint8 sw2 = a.value(2).toUInt();
        const QByteArray data = a.value(0).toByteArray();
        if (sw1 == 0x90 && sw2 == 0x00 && !data.isEmpty()) {
            iHasCardAccess = true;
            addField(g, tr("Access control"),
                tr("PACE - needs CAN, MRZ or PIN"));

            // The hex blob is the least interesting part of it. Decode the
            // SecurityInfos and show what the document is willing to talk.
            const QVariantList infos = Asn1::decodeSecurityInfos(data);
            for (int i = 0; i < infos.count(); i++) {
                const QVariantMap e = infos.at(i).toMap();
                addField(tr("Security infos"),
                    e.value(QStringLiteral("label")).toString(),
                    e.value(QStringLiteral("value")).toString());
            }
            addField(tr("Security infos"), tr("EF.CardAccess (raw)"),
                hexPlain(data), false, true);
        }
    }
}

void NfcBackend::probeApplications(const QString& tagPath)
{
    const QString g = tr("Applications");

    // A card that was pulled away mid-probe would otherwise cost us one
    // timeout per remaining AID, freezing the UI for half a minute.
    int consecutiveErrors = 0;

    for (int i = 0; i < PROBE_COUNT; i++) {
        const AppProbe& p = PROBES[i];
        const QByteArray aid = QByteArray::fromHex(p.aid);

        QDBusMessage r = call(tagPath, IFACE_ISODEP, QStringLiteral("Transmit"),
            QVariantList() << QVariant::fromValue(quint8(0x00))
                           << QVariant::fromValue(quint8(0xA4))
                           << QVariant::fromValue(quint8(0x04))
                           << QVariant::fromValue(quint8(0x00))
                           << QVariant::fromValue(aid)
                           << uint(0x100),
            1200);
        if (r.type() == QDBusMessage::ErrorMessage) {
            if (++consecutiveErrors >= 3) {
                addField(g, tr("Probing"),
                    tr("stopped early - the card stopped answering"));
                break;
            }
            continue;
        }
        consecutiveErrors = 0;

        const QVariantList a = r.arguments();
        const quint8 sw1 = a.value(1).toUInt();
        const quint8 sw2 = a.value(2).toUInt();
        const quint16 sw = (sw1 << 8) | sw2;

        QString note;
        if (sw == 0x9000) {
            note = tr("present");
        } else if (sw == 0x6982 || sw == 0x6985 || sw == 0x6283) {
            // There, but not talking to us without proper authorisation.
            note = tr("present, access denied (%1)")
                .arg(QString("%1").arg(sw, 4, 16, QChar('0')).toUpper());
        } else {
            continue;   // 6A82 and friends: not on this card
        }

        addField(g, QString::fromLatin1(p.name), note);
        iFoundApps.append(QString::fromLatin1(p.kind));
    }

    if (iFoundApps.isEmpty()) {
        addField(g, tr("Applications"), tr("none of the known AIDs answered"));
    }
}

void NfcBackend::readNdef(const QStringList& recordPaths)
{
    for (int i = 0; i < recordPaths.count(); i++) {
        QDBusMessage r = call(recordPaths.at(i), IFACE_NDEF,
            QStringLiteral("GetAll"));
        if (r.type() == QDBusMessage::ErrorMessage) continue;

        const QVariantList a = r.arguments();
        QVariantMap rec = NdefUtil::describe(
            a.value(2).toUInt(),                // tnf
            a.value(4).toByteArray(),           // type
            a.value(5).toByteArray(),           // id
            a.value(6).toByteArray());          // payload
        rec.insert(QStringLiteral("index"), i);
        iRecords.append(rec);
    }

    if (!iRecords.isEmpty()) {
        // NDEF content is the one thing we can genuinely re-emit, so it is
        // green throughout the UI.
        addField(tr("Content"), tr("NDEF records"),
            tr("%n record(s)", "", iRecords.count()), true);
    }
}

void NfcBackend::decodeBrushHead()
{
    // Recognise a Philips BrushSync head by its NDEF URL.
    bool isBrush = false;
    for (int i = 0; i < iRecords.count(); i++) {
        const QString uri = iRecords.at(i).toMap()
            .value(QStringLiteral("uri")).toString();
        if (uri.contains(QStringLiteral("philips.com/nfcbrushhead"),
                Qt::CaseInsensitive)) {
            isBrush = true;
            break;
        }
    }
    if (!isBrush) return;

    const QString g = tr("Brush head (Philips BrushSync)");

    // Physical page 36 is the wear register. The dump begins at physical page
    // 4, so the register sits at offset (36 - 4) * 4 = 128. Confirmed by
    // diffing a factory-new head against a used one: this is the only page
    // that changes with use.
    const int wearOffset = 128;
    if (iType2Dump.size() < wearOffset + 4) {
        addField(g, tr("Wear register"), tr("not in this read"));
        return;
    }

    const QByteArray wear = iType2Dump.mid(wearOffset, 4);
    addField(g, tr("Wear register (page 36)"), hexPlain(wear), false, true);

    // Bytes 0-1 big-endian is the usage counter; byte 2 is a constant format
    // marker (0x02). A new head reads zero here.
    const quint16 counter = ((quint8)wear.at(0) << 8) | (quint8)wear.at(1);

    if (counter == 0) {
        addField(g, tr("Usage"), tr("factory-new - nothing recorded yet"));
    } else {
        // Rated life is about three months; at two 2-minute sessions a day
        // that is roughly 21600 seconds of brushing. Reading the counter as
        // seconds gives a usable estimate. The exact scale would need a
        // half-worn sample to pin down, so this is labelled an estimate.
        const int nominalLife = 21600;
        const int pct = qBound(0, (int)(100.0 * counter / nominalLife), 100);
        const int brushings = counter / 120;
        addField(g, tr("Usage counter"), QString::number(counter));
        addField(g, tr("Estimated brushings"),
            tr("about %1 sessions of 2 minutes").arg(brushings));
        addField(g, tr("Estimated wear"),
            tr("about %1 % of a 3-month life").arg(pct));
    }

    // The printable type/date code sits on the three pages just before the
    // wear register (physical 33-35, offset 116-127).
    if (iType2Dump.size() >= 128) {
        const QByteArray code = iType2Dump.mid(116, 12);
        QString ascii;
        for (int i = 0; i < code.size(); i++) {
            const quint8 c = (quint8)code.at(i);
            if (c >= 0x20 && c < 0x7f) ascii.append(QChar::fromLatin1(c));
        }
        ascii = ascii.trimmed();
        if (!ascii.isEmpty()) {
            addField(g, tr("Type / date code"), ascii);
        }
    }
}

void NfcBackend::classify(uint proto, const QStringList& interfaces)
{
    // Most specific hint wins.
    if (iFoundApps.contains(QStringLiteral("egk"))) {
        iKind = QStringLiteral("egk");
        iKindLabel = tr("Health card");
        iKindPanel = PanelHealthCard;
    } else if (iFoundApps.contains(QStringLiteral("eid"))) {
        iKind = QStringLiteral("eid");
        iKindLabel = tr("Identity card (eID)");
        iKindPanel = PanelEid;
    } else if (iFoundApps.contains(QStringLiteral("epass"))) {
        iKind = QStringLiteral("epass");
        iKindLabel = tr("Travel document");
        iKindPanel = PanelEid;
    } else if (iFoundApps.contains(QStringLiteral("payment"))) {
        iKind = QStringLiteral("payment");
        iKindLabel = tr("Payment card");
        iKindPanel = PanelPayment;
    } else if (iFoundApps.contains(QStringLiteral("token"))) {
        iKind = QStringLiteral("token");
        iKindLabel = tr("Security token");
        iKindPanel = PanelTransponder;
    } else if (!iRecords.isEmpty()) {
        iKind = QStringLiteral("ndef");
        iKindLabel = tr("NDEF tag");
        iKindPanel = PanelNdef;
    } else if (iHasCardAccess) {
        iKind = QStringLiteral("pace");
        iKindLabel = tr("PACE protected document");
        iKindPanel = PanelEid;
    } else if (interfaces.contains(QLatin1String(IFACE_TAG_T2))) {
        iKind = QStringLiteral("type2");
        iKindLabel = tr("Type 2 tag");
        iKindPanel = PanelTransponder;
    } else if (iSak == 0x08 || iSak == 0x18 || iSak == 0x88 || iSak == 0x28) {
        // nfcd will not have given us a Type 2 interface for these: MIFARE
        // Classic needs Crypto1, which the SailfishOS stack does not do.
        iKind = QStringLiteral("classic");
        iKindLabel = tr("MIFARE Classic");
        iKindPanel = PanelTransponder;
    } else if (iSak == 0x20 && proto == ProtoType4A) {
        iKind = QStringLiteral("desfire");
        iKindLabel = tr("ISO 14443-4 card");
        iKindPanel = PanelTransponder;
    } else {
        iKind = QStringLiteral("unknown");
        iKindLabel = tr("Unidentified card");
        iKindPanel = PanelOther;
    }
}

QVariantMap NfcBackend::resultMap() const
{
    // Saving works from either side: whatever is on screen is what gets
    // saved, so the fresh read wins and the handed over one stands in when
    // the Read panel has already emptied.
    const bool live = !iFields.isEmpty();

    QVariantMap m;
    m.insert(QStringLiteral("kind"), live ? iKind : iDetailKind);
    m.insert(QStringLiteral("kindLabel"), live ? iKindLabel : iDetailKindLabel);
    m.insert(QStringLiteral("uid"), live ? iUid : iDetailUid);
    m.insert(QStringLiteral("frequency"), live ? iFrequency : iDetailFrequency);
    m.insert(QStringLiteral("fields"), live ? iFields : iDetailFields);
    m.insert(QStringLiteral("records"), live ? iRecords : iDetailRecords);
    m.insert(QStringLiteral("type2Dump"), QString::fromLatin1(
        (live ? iType2Dump : iDetailType2Dump).toHex()));
    return m;
}

void NfcBackend::restore(const QVariantMap& stored)
{
    clear();
    iKind = stored.value(QStringLiteral("kind")).toString();
    iKindLabel = stored.value(QStringLiteral("kindLabel")).toString();
    iUid = stored.value(QStringLiteral("uid")).toString();
    iFrequency = stored.value(QStringLiteral("frequency")).toString();
    iFields = stored.value(QStringLiteral("fields")).toList();
    iRecords = stored.value(QStringLiteral("records")).toList();
    iType2Dump = QByteArray::fromHex(
        stored.value(QStringLiteral("type2Dump")).toString().toLatin1());
    emit resultChanged();
    setState(Done, tr("Loaded %1").arg(iKindLabel));
}
