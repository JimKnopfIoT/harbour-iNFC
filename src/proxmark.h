/*
 * iNFC - Ultimate Mode: the Proxmark.
 *
 * Everything the phone's own radio cannot do lives here, because it needs
 * external hardware: 125 kHz LF, MIFARE Classic with Crypto1, sniffing.
 *
 * We speak the Proxmark's own wire protocol directly over /dev/ttyACM0 via
 * termios - no client binary, no QtSerialPort. The framing is the Iceman NG
 * format (pm3_cmd.h):
 *
 *   command  : "PM3a"(4) | length:15 ng:1 (2) | cmd(2) | data | crc(2)
 *   response : "PM3b"(4) | length:15 ng:1 (2) | status(1) reason(1) | cmd(2)
 *              | data | crc(2)
 *
 * Over USB the CRC is not computed; both sides use the postamble magic
 * (command 0x3361 "a3", response 0x3362 "b3") in its place.
 */
#ifndef PROXMARK_H
#define PROXMARK_H

#include <QByteArray>
#include <QObject>
#include <QString>

class Proxmark : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool present READ present NOTIFY presenceChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(QString portPath READ portPath CONSTANT)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(QString lastTag READ lastTag NOTIFY lastTagChanged)
    Q_PROPERTY(QString lastTagType READ lastTagType NOTIFY lastTagChanged)
    Q_PROPERTY(QString hfUid READ hfUid NOTIFY hfChanged)
    Q_PROPERTY(QString hfType READ hfType NOTIFY hfChanged)
    Q_PROPERTY(QString hfDetails READ hfDetails NOTIFY hfChanged)
    Q_PROPERTY(QString mfResult READ mfResult NOTIFY mfChanged)
    Q_PROPERTY(int mfFound READ mfFound NOTIFY mfChanged)
    Q_PROPERTY(QString mfDump READ mfDump NOTIFY mfDumpChanged)
    Q_PROPERTY(int mfDumpBlocks READ mfDumpBlocks NOTIFY mfDumpChanged)

public:
    explicit Proxmark(QObject* parent = Q_NULLPTR);
    ~Proxmark();

    // Iceman NG command ids (pm3_cmd.h). Only the handshake set for now; the
    // LF/HF command ids come in once the pipe is proven against the device.
    enum Cmd {
        CMD_PING            = 0x0109,
        CMD_VERSION         = 0x0107,
        CMD_CAPABILITIES    = 0x0112,
        CMD_DEBUG_PRINT     = 0x0100,
        CMD_BREAK_LOOP      = 0x0118,
        CMD_ACK             = 0x00ff,
        CMD_NACK            = 0x00fe,
        CMD_LF_HID_WATCH    = 0x020B,
        CMD_LF_EM410X_CLONE = 0x0211,
        CMD_LF_IO_WATCH     = 0x021A,
        CMD_LF_EM410X_WATCH = 0x021C,
        CMD_HF_ISO14443A_READER = 0x0385,
        CMD_HF_MIFARE_READBL    = 0x0620,
        CMD_HF_MIFARE_CHKKEYS   = 0x0623
    };

    // ISO14A reader flags (mifare.h)
    enum Iso14aFlag {
        ISO14A_CONNECT    = (1 << 0),
        ISO14A_CLEARTRACE = (1 << 17)
    };

    bool present() const;                 // is /dev/ttyACM0 there at all
    bool connected() const { return iFd >= 0; }
    QString portPath() const;
    QString version() const { return iVersion; }
    QString status() const { return iStatus; }
    bool busy() const { return iBusy; }
    QString lastTag() const { return iLastTag; }
    QString lastTagType() const { return iLastTagType; }
    QString hfUid() const { return iHfUid; }
    QString hfType() const { return iHfType; }
    QString hfDetails() const { return iHfDetails; }
    QString mfResult() const { return iMfResult; }
    int mfFound() const { return iMfFound; }
    QString mfDump() const { return iMfDump; }
    int mfDumpBlocks() const { return iMfDumpBlocks; }

public slots:
    // Open the port and prove the pipe with a PING, then read the version.
    void connectDevice();
    void disconnect();
    // Round-trip a PING with a known payload; true if it echoed back.
    bool ping();
    // Ask the device for its human-readable firmware version.
    void readVersion();
    // Search 125 kHz badges: try EM4100, then HID Prox, then IO Prox. Each
    // format's watch command makes the device demodulate on its own and print
    // the id as a debug line, which we harvest, then stop the loop.
    void readLf();
    // Write an EM4100 id (10 hex digits) onto a blank T5577. This permanently
    // reconfigures the tag. The device does the writing; we just send the id.
    void writeEm4100(const QString& hexId);
    // Read a 13.56 MHz ISO 14443-A card: UID, ATQA, SAK. From the SAK we can
    // finally say whether a card is MIFARE Classic (which the phone's own
    // stack refuses). Device-side, returns the data in the reply.
    void readHf();
    // MIFARE Classic: try a dictionary of default keys against every sector
    // (key A and key B). Reports which sectors open with a known key - the
    // first, non-destructive step toward a dump.
    void checkMifareKeys();
    // Dump every block of a MIFARE Classic using the keys found by the check.
    // Data blocks are read; trailers are rebuilt from the known keys.
    void dumpMifare();

signals:
    void presenceChanged();
    void connectedChanged();
    void versionChanged();
    void statusChanged();
    void busyChanged();
    void lastTagChanged();
    void hfChanged();
    void mfChanged();
    void mfDumpChanged();
    void logLine(const QString& line);

private:
    void setStatus(const QString& s);
    void setBusy(bool b);
    bool openPort();
    void closePort();

    // Framing
    QByteArray frameCommand(quint16 cmd, const QByteArray& data, bool ng = true);
    bool sendCommand(quint16 cmd, const QByteArray& data, bool ng = true);
    // Wait for one NG response; fills cmd/status/data. false on timeout.
    bool readResponse(quint16* cmd, qint8* status, QByteArray* data,
        int timeoutMs = 1500);

    // Run one watch command, harvest the debug line that carries the id.
    // marker is the substring that flags a hit; hexAfterTagId picks the hex
    // run after "TAG ID:" (EM/HID), otherwise the rest of the line is the id
    // (IO Prox). Returns true and fills outId on a hit.
    bool watchProbe(quint16 cmd, const QString& marker, bool hexAfterTagId,
        QString* outId, int budgetMs = 3500);

    // Try the default-key dictionary against one block/keytype. Returns the
    // matching 6-byte key, or an empty array.
    QByteArray mfTryKeys(quint8 block, quint8 keyType, const QByteArray& keyBlock);
    // Read one block with a known key. Returns 16 bytes, or an empty array.
    QByteArray mfReadBlock(quint8 block, quint8 keyType, const QByteArray& key);

    int iFd;
    QString iVersion;
    QString iStatus;
    QString iLastTag;
    QString iLastTagType;
    QString iHfUid;
    QString iHfType;
    QString iHfDetails;
    QString iMfResult;
    int iMfFound;
    QByteArray iMfKeyA[16];   // keys found by the last check, per sector
    QByteArray iMfKeyB[16];
    QString iMfDump;          // full dump as hex, block per line
    int iMfDumpBlocks;
    bool iBusy;
    QByteArray iRx;   // accumulates bytes across reads
};

#endif // PROXMARK_H
