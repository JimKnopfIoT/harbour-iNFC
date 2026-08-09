/*
 * iNFC - the sending side.
 *
 * We register a Type 4 Tag NDEF application with nfcd and answer the reader's
 * APDUs ourselves. That is genuine host card emulation, and it is also the
 * hard limit of what a phone can be: an ISO-DEP smart card. We cannot pretend
 * to be a Type 2 tag, and we cannot manufacture EMV cryptograms. See the
 * Emulate panel for the long version.
 */
#ifndef EMULATOR_H
#define EMULATOR_H

#include <QByteArray>
#include <QDBusAbstractAdaptor>
#include <QDBusObjectPath>
#include <QObject>
#include <QString>

class Emulator;

// org.sailfishos.nfc.LocalHostApp, exported on our own connection.
class HostAppAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.sailfishos.nfc.LocalHostApp")

public:
    explicit HostAppAdaptor(Emulator* parent);

public slots:
    int GetInterfaceVersion() const { return 1; }
    void Start(const QDBusObjectPath& host);
    void Restart(const QDBusObjectPath& host);
    void Stop(const QDBusObjectPath& path);
    void ImplicitSelect(const QDBusObjectPath& host);
    void Select(const QDBusObjectPath& host);
    void Deselect(const QDBusObjectPath& path);
    QByteArray Process(const QDBusObjectPath& host, uchar cla, uchar ins,
        uchar p1, uchar p2, const QByteArray& data, uint le,
        uchar& sw1, uchar& sw2, uint& responseId);
    void ResponseStatus(uint responseId, bool ok);

private:
    Emulator* iEmulator;
};

class Emulator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ active NOTIFY activeChanged)
    Q_PROPERTY(bool supported READ supported CONSTANT)
    Q_PROPERTY(QString summary READ summary NOTIFY contentChanged)
    Q_PROPERTY(QString lastEvent READ lastEvent NOTIFY lastEventChanged)
    Q_PROPERTY(QString nfcid1 READ nfcid1 WRITE setNfcid1 NOTIFY nfcid1Changed)

public:
    explicit Emulator(QObject* parent = Q_NULLPTR);
    ~Emulator();

    bool active() const { return iActive; }
    bool supported() const;
    QString summary() const { return iSummary; }
    QString lastEvent() const { return iLastEvent; }
    QString nfcid1() const { return iNfcid1; }
    void setNfcid1(const QString& hex);

public slots:
    // Load what will be served. Both replace the current message.
    void setText(const QString& text);
    void setUri(const QString& uri);
    // Serve a message captured from a real tag (hex encoded).
    void setRawMessage(const QString& hex);

    void start();
    void stop();

signals:
    void activeChanged();
    void contentChanged();
    void lastEventChanged();
    void nfcid1Changed();

private:
    friend class HostAppAdaptor;
    void note(const QString& event);
    QByteArray ccFile() const;
    QByteArray ndefFile() const;

private:
    HostAppAdaptor* iAdaptor;
    bool iActive;
    uint iModeId;           // Daemon.RequestMode id
    uint iParamId;          // Adapter.RequestParams id
    QByteArray iMessage;    // the NDEF message we serve
    QString iSummary;
    QString iLastEvent;
    QString iNfcid1;
    int iSelectedFile;      // 0 = none, 1 = CC, 2 = NDEF
};

#endif // EMULATOR_H
