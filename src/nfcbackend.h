/*
 * iNFC - the reading side.
 *
 * Design rule: the radio is quiet unless the user asked for it. On startup we
 * register a "disallow all technologies" request with nfcd, which suppresses
 * the system wide polling loop. arm() releases that request for exactly one
 * read, then we put the block back. Nothing is ever read in the background.
 */
#ifndef NFCBACKEND_H
#define NFCBACKEND_H

#include <QDBusObjectPath>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QVariantList>
#include <QVariantMap>

class NfcBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool daemonPresent READ daemonPresent NOTIFY daemonChanged)
    Q_PROPERTY(QString daemonVersion READ daemonVersion NOTIFY daemonChanged)
    Q_PROPERTY(bool adapterEnabled READ adapterEnabled NOTIFY daemonChanged)
    Q_PROPERTY(bool cardEmulationSupported READ cardEmulationSupported NOTIFY daemonChanged)
    Q_PROPERTY(QString supportedTechs READ supportedTechs NOTIFY daemonChanged)

    Q_PROPERTY(int state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString statusText READ statusText NOTIFY stateChanged)

    Q_PROPERTY(bool hasResult READ hasResult NOTIFY resultChanged)
    Q_PROPERTY(QVariantList fields READ fields NOTIFY resultChanged)
    Q_PROPERTY(QVariantList records READ records NOTIFY resultChanged)
    Q_PROPERTY(QString kind READ kind NOTIFY resultChanged)
    Q_PROPERTY(QString kindLabel READ kindLabel NOTIFY resultChanged)
    Q_PROPERTY(int kindPanel READ kindPanel NOTIFY resultChanged)
    Q_PROPERTY(QString uid READ uid NOTIFY resultChanged)
    Q_PROPERTY(QString frequency READ frequency NOTIFY resultChanged)

    // The handed over result. A read belongs to exactly one place at a time:
    // while it is fresh it sits on the Read panel, and the moment you leave
    // for a detail panel it moves there and the Read panel empties. That way
    // there is never a doubt whether what you see is the card you just held
    // against the phone or the one before it.
    Q_PROPERTY(bool hasDetail READ hasDetail NOTIFY detailChanged)
    Q_PROPERTY(QVariantList detailFields READ detailFields NOTIFY detailChanged)
    Q_PROPERTY(QVariantList detailRecords READ detailRecords NOTIFY detailChanged)
    Q_PROPERTY(QString detailKind READ detailKind NOTIFY detailChanged)
    Q_PROPERTY(QString detailKindLabel READ detailKindLabel NOTIFY detailChanged)
    Q_PROPERTY(QString detailUid READ detailUid NOTIFY detailChanged)
    Q_PROPERTY(QString detailFrequency READ detailFrequency NOTIFY detailChanged)

public:
    // Keep in sync with the panel order in MainPage.qml
    enum Panel {
        PanelRead        = 0,
        PanelNdef        = 1,
        PanelHealthCard  = 2,
        PanelEid         = 3,
        PanelPayment     = 4,
        PanelTransponder = 5,
        PanelOther        = 6,
        PanelEmulate      = 7,
        PanelUltraviolence = 8,
        PanelLog          = 9,
        PanelArchive      = 10
    };
    Q_ENUM(Panel)

    enum State { Idle, Armed, Reading, Done, Failed };
    Q_ENUM(State)

    explicit NfcBackend(QObject* parent = Q_NULLPTR);
    ~NfcBackend();

    bool daemonPresent() const { return iDaemonPresent; }
    QString daemonVersion() const { return iDaemonVersion; }
    bool adapterEnabled() const { return iAdapterEnabled; }
    bool cardEmulationSupported() const { return iSupportedModes & 0x08; }
    QString supportedTechs() const;

    int state() const { return iState; }
    QString statusText() const { return iStatusText; }

    bool hasResult() const { return !iFields.isEmpty(); }
    QVariantList fields() const { return iFields; }
    QVariantList records() const { return iRecords; }
    QString kind() const { return iKind; }
    QString kindLabel() const { return iKindLabel; }
    int kindPanel() const { return iKindPanel; }
    QString uid() const { return iUid; }
    QString frequency() const { return iFrequency; }

    bool hasDetail() const { return !iDetailFields.isEmpty(); }
    QVariantList detailFields() const { return iDetailFields; }
    QVariantList detailRecords() const { return iDetailRecords; }
    QString detailKind() const { return iDetailKind; }
    QString detailKindLabel() const { return iDetailKindLabel; }
    QString detailUid() const { return iDetailUid; }
    QString detailFrequency() const { return iDetailFrequency; }

    QString adapterPath() const { return iAdapterPath; }

public slots:
    // Open the radio and wait for a single tag.
    void arm();
    // Arm for a write instead of a read. The next tag presented gets the
    // given NDEF message written to it, if it is a writeable Type 2 tag.
    void armWriteText(const QString& text);
    void armWriteUri(const QString& uri);
    // Close it again without reading.
    void disarm();
    // Throw away the current result (does not touch saved cards).
    void clear();
    // Move the fresh read onto the detail side and empty the Read panel.
    void handOff();
    // Everything we know about the current read, for CardStore.
    QVariantMap resultMap() const;
    // Re-populate the UI from a stored card.
    void restore(const QVariantMap& stored);

signals:
    void daemonChanged();
    void stateChanged();
    void resultChanged();
    void detailChanged();
    void tagRead();

private slots:
    void onTagsChanged(const QList<QDBusObjectPath>& tags);
    void onTargetPresentChanged(bool present);
    void onArmTimeout();
    void onUnreadableTarget();
    void onNameOwnerChanged(const QString& name, const QString& oldOwner,
        const QString& newOwner);

private:
    void connectDaemon();
    void refreshAdapter();
    void blockPolling();
    void unblockPolling();
    void setState(State state, const QString& text = QString());
    void readTag(const QString& tagPath);
    void writeTag(const QString& tagPath);

    // Read helpers, all blocking. Individual failures are not fatal: whatever
    // could be read is shown, the rest is simply absent.
    void readTransport(const QString& tagPath, uint tech, uint proto,
        const QVariantMap& pollParams);
    void readType2(const QString& tagPath);
    void readIsoDep(const QString& tagPath);
    void readNdef(const QStringList& recordPaths);
    void probeApplications(const QString& tagPath);
    // Product specific decoders that turn a raw Type 2 dump into something
    // readable. Each recognises its own tag and does nothing otherwise.
    void decodeBrushHead();
    void classify(uint proto, const QStringList& interfaces);

    void addField(const QString& group, const QString& label,
        const QString& value, bool emittable = false, bool mono = false);

private:
    bool iDaemonPresent;
    QString iDaemonVersion;
    QString iAdapterPath;
    bool iAdapterEnabled;
    uint iSupportedModes;
    uint iSupportedTechs;

    State iState;
    QString iStatusText;
    uint iBlockId;      // id returned by Daemon.RequestTechs, 0 = not blocking

    QVariantList iFields;
    QVariantList iRecords;
    QStringList iFoundApps;     // human readable names of applications found
    QString iKind;
    QString iKindLabel;
    int iKindPanel;
    QString iUid;
    QString iFrequency;

    QVariantList iDetailFields;
    QVariantList iDetailRecords;
    QString iDetailKind;
    QString iDetailKindLabel;
    QString iDetailUid;
    QString iDetailFrequency;
    QByteArray iDetailType2Dump;
    uint iSak;
    bool iHasCardAccess;
    QByteArray iType2Dump;
    QByteArray iPendingWrite;   // non-empty while armed for a write

    // Waiting for a tag must always end, one way or another.
    QTimer* iArmTimer;          // gives up if nothing shows up at all
    QTimer* iTargetTimer;       // something is in the field but no tag object
};

#endif // NFCBACKEND_H
