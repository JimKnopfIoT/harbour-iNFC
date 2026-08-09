#include "proxmark.h"

#include <QDebug>
#include <QFile>

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

#define PORT "/dev/ttyACM0"

#define CMD_MAGIC   0x61334d50u   // "PM3a"
#define RESP_MAGIC  0x62334d50u   // "PM3b"
#define CMD_POST    0x3361        // "a3"
#define RESP_POST   0x3362        // "b3"

Proxmark::Proxmark(QObject* parent) :
    QObject(parent),
    iFd(-1),
    iMfFound(0),
    iMfDumpBlocks(0),
    iBusy(false)
{
    iStatus = tr("Not connected");
}

Proxmark::~Proxmark()
{
    closePort();
}

QString Proxmark::portPath() const
{
    return QStringLiteral(PORT);
}

bool Proxmark::present() const
{
    return QFile::exists(QStringLiteral(PORT));
}

void Proxmark::setStatus(const QString& s)
{
    iStatus = s;
    // Also to the journal, so the first connection attempt can be diagnosed
    // from the outside - the sailjail-vs-open question is answered here.
    qWarning() << "[proxmark]" << s;
    emit statusChanged();
    emit logLine(s);
}

void Proxmark::setBusy(bool b)
{
    if (iBusy == b) return;
    iBusy = b;
    emit busyChanged();
}

// --------------------------------------------------------------------------
// Port
// --------------------------------------------------------------------------

bool Proxmark::openPort()
{
    if (iFd >= 0) return true;

    // O_NOCTTY: this is not our controlling terminal. No O_NONBLOCK - we want
    // a normal blocking fd and drive timeouts with select().
    iFd = ::open(PORT, O_RDWR | O_NOCTTY);
    if (iFd < 0) {
        const int e = errno;
        // errno is the whole story here: EACCES means sailjail or group,
        // ENOENT means unplugged. This is exactly what the first run needs
        // to tell us.
        setStatus(tr("open(%1) failed: %2 (errno %3)")
            .arg(QStringLiteral(PORT))
            .arg(QString::fromLocal8Bit(strerror(e)))
            .arg(e));
        return false;
    }

    struct termios tio;
    if (tcgetattr(iFd, &tio) != 0) {
        setStatus(tr("tcgetattr failed: %1").arg(QString::fromLocal8Bit(strerror(errno))));
        closePort();
        return false;
    }
    cfmakeraw(&tio);
    // CDC-ACM ignores the line rate, but set something sane anyway.
    cfsetispeed(&tio, B115200);
    cfsetospeed(&tio, B115200);
    tio.c_cflag |= (CLOCAL | CREAD);
    tio.c_cflag &= ~CRTSCTS;
    tio.c_cc[VMIN] = 0;
    tio.c_cc[VTIME] = 0;
    if (tcsetattr(iFd, TCSANOW, &tio) != 0) {
        setStatus(tr("tcsetattr failed: %1").arg(QString::fromLocal8Bit(strerror(errno))));
        closePort();
        return false;
    }

    tcflush(iFd, TCIOFLUSH);
    iRx.clear();
    return true;
}

void Proxmark::closePort()
{
    if (iFd >= 0) {
        ::close(iFd);
        iFd = -1;
    }
}

// --------------------------------------------------------------------------
// Framing
// --------------------------------------------------------------------------

QByteArray Proxmark::frameCommand(quint16 cmd, const QByteArray& data, bool ng)
{
    QByteArray f;
    const quint32 magic = CMD_MAGIC;
    f.append((char)(magic & 0xff));
    f.append((char)((magic >> 8) & 0xff));
    f.append((char)((magic >> 16) & 0xff));
    f.append((char)((magic >> 24) & 0xff));

    // length is 15 bits, ng is bit 15
    quint16 lenng = (quint16)(data.size() & 0x7fff);
    if (ng) lenng |= 0x8000;
    f.append((char)(lenng & 0xff));
    f.append((char)((lenng >> 8) & 0xff));

    f.append((char)(cmd & 0xff));
    f.append((char)((cmd >> 8) & 0xff));

    f.append(data);

    // No CRC over USB: the postamble magic stands in for it.
    f.append((char)(CMD_POST & 0xff));
    f.append((char)((CMD_POST >> 8) & 0xff));
    return f;
}

bool Proxmark::sendCommand(quint16 cmd, const QByteArray& data, bool ng)
{
    if (iFd < 0) return false;
    const QByteArray f = frameCommand(cmd, data, ng);
    int off = 0;
    while (off < f.size()) {
        const ssize_t n = ::write(iFd, f.constData() + off, f.size() - off);
        if (n < 0) {
            if (errno == EINTR) continue;
            setStatus(tr("write failed: %1").arg(QString::fromLocal8Bit(strerror(errno))));
            return false;
        }
        off += (int)n;
    }
    return true;
}

bool Proxmark::readResponse(quint16* cmd, qint8* status, QByteArray* data,
    int timeoutMs)
{
    // Preamble is 10 bytes: magic(4) len/ng(2) status(1) reason(1) cmd(2).
    const int PRE = 10;

    struct timeval start;
    gettimeofday(&start, Q_NULLPTR);

    for (;;) {
        // Do we already have a full frame buffered?
        int magicAt = -1;
        for (int i = 0; i + 4 <= iRx.size(); i++) {
            const quint32 m = (quint8)iRx.at(i)
                | ((quint8)iRx.at(i + 1) << 8)
                | ((quint8)iRx.at(i + 2) << 16)
                | ((quint32)(quint8)iRx.at(i + 3) << 24);
            if (m == RESP_MAGIC) { magicAt = i; break; }
        }
        if (magicAt >= 0 && iRx.size() >= magicAt + PRE) {
            const int p = magicAt;
            const quint16 lenng = (quint8)iRx.at(p + 4) | ((quint8)iRx.at(p + 5) << 8);
            const int len = lenng & 0x7fff;
            const qint8 st = (qint8)iRx.at(p + 6);
            const quint16 rcmd = (quint8)iRx.at(p + 8) | ((quint8)iRx.at(p + 9) << 8);
            const int total = PRE + len + 2;   // + postamble/crc
            if (iRx.size() >= magicAt + total) {
                if (cmd) *cmd = rcmd;
                if (status) *status = st;
                if (data) *data = iRx.mid(p + PRE, len);
                iRx.remove(0, magicAt + total);
                return true;
            }
        }

        // Timeout check
        struct timeval now;
        gettimeofday(&now, Q_NULLPTR);
        const long elapsed = (now.tv_sec - start.tv_sec) * 1000
            + (now.tv_usec - start.tv_usec) / 1000;
        if (elapsed >= timeoutMs) return false;

        // Wait for more bytes
        fd_set rf;
        FD_ZERO(&rf);
        FD_SET(iFd, &rf);
        struct timeval tv;
        const long remain = timeoutMs - elapsed;
        tv.tv_sec = remain / 1000;
        tv.tv_usec = (remain % 1000) * 1000;
        const int s = ::select(iFd + 1, &rf, Q_NULLPTR, Q_NULLPTR, &tv);
        if (s < 0) {
            if (errno == EINTR) continue;
            return false;
        }
        if (s == 0) return false;   // timeout

        char buf[512];
        const ssize_t n = ::read(iFd, buf, sizeof(buf));
        if (n > 0) {
            iRx.append(buf, (int)n);
        } else if (n < 0 && errno != EINTR && errno != EAGAIN) {
            return false;
        }
    }
}

// --------------------------------------------------------------------------
// Public operations
// --------------------------------------------------------------------------

void Proxmark::connectDevice()
{
    if (connected()) return;
    setBusy(true);

    if (!present()) {
        setStatus(tr("No %1 - is the Proxmark plugged in?").arg(QStringLiteral(PORT)));
        setBusy(false);
        return;
    }
    if (!openPort()) {
        setBusy(false);
        return;
    }

    emit connectedChanged();
    setStatus(tr("Port open, probing..."));

    if (!ping()) {
        setStatus(tr("Port opened but the Proxmark did not answer a PING. "
                     "It may run older firmware that only speaks the legacy frame format."));
        // Keep the port open; the version attempt or a retry may still help.
    } else {
        setStatus(tr("Proxmark answered. Reading version..."));
        readVersion();
    }

    setBusy(false);
}

void Proxmark::disconnect()
{
    closePort();
    iVersion.clear();
    emit versionChanged();
    emit connectedChanged();
    setStatus(tr("Disconnected"));
}

bool Proxmark::ping()
{
    if (iFd < 0) return false;

    const QByteArray payload = QByteArrayLiteral("iNFCping");
    if (!sendCommand(CMD_PING, payload)) return false;

    quint16 rcmd = 0;
    qint8 st = 0;
    QByteArray data;
    // The device may emit an unsolicited debug line first; loop a few times.
    for (int i = 0; i < 4; i++) {
        if (!readResponse(&rcmd, &st, &data, 1500)) return false;
        if (rcmd == CMD_PING) {
            return data.startsWith(payload);
        }
        if (rcmd == CMD_DEBUG_PRINT) {
            emit logLine(tr("PM3: %1").arg(QString::fromLatin1(data)));
        }
    }
    return false;
}

void Proxmark::readVersion()
{
    if (iFd < 0) return;
    if (!sendCommand(CMD_VERSION, QByteArray())) return;

    quint16 rcmd = 0;
    qint8 st = 0;
    QByteArray data;
    for (int i = 0; i < 6; i++) {
        if (!readResponse(&rcmd, &st, &data, 2000)) break;
        if (rcmd == CMD_VERSION || rcmd == CMD_ACK) {
            // Dump the raw payload once so the exact struct layout can be
            // parsed properly instead of guessed.
            qWarning() << "[proxmark] version payload" << data.size()
                       << "bytes:" << data.toHex();
            // The payload is a struct followed by a multi-line ASCII banner
            // with ANSI colour escapes. Strip the escapes, then pick the
            // "Iceman/..." build line - that is the one worth showing.
            QString text;
            for (int j = 0; j < data.size(); j++) {
                const quint8 c = (quint8)data.at(j);
                if (c == 0x1b) {                 // ESC - skip until the 'm'
                    while (j < data.size() && data.at(j) != 'm') j++;
                    continue;
                }
                text.append((c == '\n' || (c >= 0x20 && c < 0x7f))
                    ? QChar::fromLatin1(c) : QChar::fromLatin1(' '));
            }

            iVersion.clear();
            const QStringList lines = text.split(QLatin1Char('\n'));
            for (int j = 0; j < lines.size(); j++) {
                const QString ln = lines.at(j).simplified();
                if (ln.contains(QStringLiteral("Iceman/")) ||
                    ln.contains(QStringLiteral("/v"), Qt::CaseInsensitive)) {
                    iVersion = ln;   // e.g. "Bootrom.... Iceman/master/v4...."
                    break;
                }
            }
            if (iVersion.isEmpty()) iVersion = text.simplified().left(120);
            emit versionChanged();
            setStatus(tr("Connected"));
            return;
        }
        if (rcmd == CMD_DEBUG_PRINT) {
            emit logLine(tr("PM3: %1").arg(QString::fromLatin1(data)));
        }
    }
    setStatus(tr("Connected (no version reply)"));
}

bool Proxmark::watchProbe(quint16 cmd, const QString& marker,
    bool hexAfterTagId, QString* outId, int budgetMs)
{
    tcflush(iFd, TCIFLUSH);
    iRx.clear();
    if (!sendCommand(cmd, QByteArray())) return false;

    struct timeval start;
    gettimeofday(&start, Q_NULLPTR);
    bool found = false;

    for (;;) {
        struct timeval now;
        gettimeofday(&now, Q_NULLPTR);
        const long elapsed = (now.tv_sec - start.tv_sec) * 1000
            + (now.tv_usec - start.tv_usec) / 1000;
        if (elapsed >= budgetMs) break;

        quint16 rcmd = 0;
        qint8 st = 0;
        QByteArray data;
        if (!readResponse(&rcmd, &st, &data, budgetMs - elapsed)) break;
        if (rcmd != CMD_DEBUG_PRINT) continue;

        // Strip ANSI escapes.
        QString s;
        for (int i = 0; i < data.size(); i++) {
            const quint8 c = (quint8)data.at(i);
            if (c == 0x1b) { while (i < data.size() && data.at(i) != 'm') i++; continue; }
            if (c >= 0x20 && c < 0x7f) s.append(QChar::fromLatin1(c));
        }
        emit logLine(QStringLiteral("PM3: ") + s);

        if (!s.contains(marker)) continue;

        if (hexAfterTagId) {
            // EM4100 / HID: the id is the first hex run after "TAG ID:".
            const int p = s.indexOf(QStringLiteral("TAG ID:"));
            if (p < 0) continue;
            QString hex;
            for (int i = p + 7; i < s.size(); i++) {
                const QChar ch = s.at(i);
                if (ch.isSpace()) { if (!hex.isEmpty()) break; else continue; }
                if (isxdigit(ch.toLatin1())) hex.append(ch);
                else if (!hex.isEmpty()) break;
            }
            if (!hex.isEmpty()) { *outId = hex.toUpper(); found = true; break; }
        } else {
            // IO Prox: the descriptive tail after the marker is the id.
            const int p = s.indexOf(marker);
            *outId = s.mid(p + marker.length()).simplified();
            if (!outId->isEmpty()) { found = true; break; }
        }
    }

    // Stop the watch loop and drain its final reply.
    sendCommand(CMD_BREAK_LOOP, QByteArray());
    quint16 rcmd = 0;
    qint8 st = 0;
    QByteArray data;
    for (int i = 0; i < 3; i++) {
        if (!readResponse(&rcmd, &st, &data, 700)) break;
        if (rcmd == cmd) break;
    }
    return found;
}

static QString hexColon(const QByteArray& b)
{
    QString s;
    for (int i = 0; i < b.size(); i++) {
        if (i) s.append(QLatin1Char(':'));
        s.append(QString("%1").arg((quint8)b.at(i), 2, 16, QChar('0')).toUpper());
    }
    return s;
}

// SAK (SEL_RES) -> human readable type. This is the same table the Reference
// glossary uses; here it decides whether we are looking at MIFARE Classic.
static QString sakType(quint8 sak, bool* isClassic)
{
    if (isClassic) *isClassic = false;
    switch (sak) {
    case 0x00: return QObject::tr("MIFARE Ultralight / NTAG (Type 2)");
    case 0x08: if (isClassic) *isClassic = true; return QObject::tr("MIFARE Classic 1K");
    case 0x09: if (isClassic) *isClassic = true; return QObject::tr("MIFARE Classic Mini");
    case 0x18: if (isClassic) *isClassic = true; return QObject::tr("MIFARE Classic 4K");
    case 0x88: if (isClassic) *isClassic = true; return QObject::tr("Infineon MIFARE Classic 1K");
    case 0x28: if (isClassic) *isClassic = true; return QObject::tr("MIFARE Classic + ISO-DEP");
    case 0x10: return QObject::tr("MIFARE Plus 2K (SL2)");
    case 0x11: return QObject::tr("MIFARE Plus 4K (SL2)");
    case 0x20: return QObject::tr("ISO 14443-4 (DESFire, Plus SL3, JCOP…)");
    default:   return QObject::tr("unknown (SAK %1)")
                   .arg(QString("%1").arg(sak, 2, 16, QChar('0')).toUpper());
    }
}

// The well-known MIFARE Classic default keys. A field card that was never
// re-keyed opens with one of these on at least some sectors.
static const char* MF_DEFAULT_KEYS[] = {
    "FFFFFFFFFFFF", "000000000000", "A0A1A2A3A4A5", "D3F7D3F7D3F7",
    "A0B0C0D0E0F0", "B0B1B2B3B4B5", "AABBCCDDEEFF", "1A2B3C4D5E6F",
    "123456789ABC", "010203040506", "112233445566", "4D3A99C351DD",
    "1A982C7E459A", "714C5C886E97", "587EE5F9350F", "A0478CC39091",
    "533CB6C723F6", "8FD0A4F256E9"
};
static const int MF_KEYCNT = sizeof(MF_DEFAULT_KEYS) / sizeof(MF_DEFAULT_KEYS[0]);

// Try the whole dictionary against one block for one key type (0=A, 1=B).
// Returns the matching 6-byte key, or an empty array.
QByteArray Proxmark::mfTryKeys(quint8 block, quint8 keyType,
    const QByteArray& keyBlock)
{
    QByteArray p;
    p.append((char)keyType);
    p.append((char)block);
    p.append((char)0);                 // clear_trace
    p.append((char)0);                 // key_count high byte
    p.append((char)MF_KEYCNT);         // key_count low byte
    p.append(keyBlock);

    tcflush(iFd, TCIFLUSH);
    iRx.clear();
    if (!sendCommand(CMD_HF_MIFARE_CHKKEYS, p)) return QByteArray();

    quint16 rcmd = 0;
    qint8 st = 0;
    QByteArray data;
    for (int i = 0; i < 6; i++) {
        if (!readResponse(&rcmd, &st, &data, 4000)) break;
        if (rcmd == CMD_DEBUG_PRINT) continue;
        if (rcmd == CMD_HF_MIFARE_CHKKEYS) {
            // Reply: key[6] + found(1). status 0 = ok.
            if (st == 0 && data.size() >= 7 && (quint8)data.at(6) != 0) {
                return data.left(6);
            }
            return QByteArray();
        }
    }
    return QByteArray();
}

void Proxmark::checkMifareKeys()
{
    if (iFd < 0) return;
    setBusy(true);
    iMfResult.clear();
    iMfFound = 0;
    iMfDump.clear();
    iMfDumpBlocks = 0;
    emit mfChanged();
    emit mfDumpChanged();

    // Concatenate the dictionary into one 6-byte-per-key block.
    QByteArray keyBlock;
    for (int i = 0; i < MF_KEYCNT; i++) {
        keyBlock.append(QByteArray::fromHex(MF_DEFAULT_KEYS[i]));
    }

    // MIFARE Classic 1K: 16 sectors, 4 blocks each, trailer at sector*4+3.
    const int sectors = 16;
    QStringList lines;
    int found = 0;

    for (int s = 0; s < sectors; s++) {
        setStatus(tr("Checking sector %1/%2...").arg(s + 1).arg(sectors));
        const quint8 trailer = (quint8)(s * 4 + 3);
        iMfKeyA[s] = mfTryKeys(trailer, 0, keyBlock);
        iMfKeyB[s] = mfTryKeys(trailer, 1, keyBlock);
        if (!iMfKeyA[s].isEmpty() || !iMfKeyB[s].isEmpty()) found++;
        lines << tr("Sector %1  A:%2  B:%3")
            .arg(s, 2, 10, QChar('0'))
            .arg(iMfKeyA[s].isEmpty() ? QStringLiteral("--")
                 : QString::fromLatin1(iMfKeyA[s].toHex()).toUpper())
            .arg(iMfKeyB[s].isEmpty() ? QStringLiteral("--")
                 : QString::fromLatin1(iMfKeyB[s].toHex()).toUpper());
    }

    iMfResult = lines.join(QStringLiteral("\n"));
    iMfFound = found;
    emit mfChanged();
    setStatus(tr("%1 of %2 sectors open with a default key").arg(found).arg(sectors));
    setBusy(false);
}

QByteArray Proxmark::mfReadBlock(quint8 block, quint8 keyType,
    const QByteArray& key)
{
    if (key.size() != 6) return QByteArray();

    QByteArray p;
    p.append((char)block);
    p.append((char)keyType);
    p.append(key);                     // mf_readblock_t: blockno, keytype, key[6]

    tcflush(iFd, TCIFLUSH);
    iRx.clear();
    if (!sendCommand(CMD_HF_MIFARE_READBL, p)) return QByteArray();

    quint16 rcmd = 0;
    qint8 st = 0;
    QByteArray data;
    for (int i = 0; i < 6; i++) {
        if (!readResponse(&rcmd, &st, &data, 2000)) break;
        if (rcmd == CMD_DEBUG_PRINT) continue;
        if (rcmd == CMD_HF_MIFARE_READBL) {
            if (st == 0 && data.size() >= 16) return data.left(16);
            return QByteArray();
        }
    }
    return QByteArray();
}

void Proxmark::dumpMifare()
{
    if (iFd < 0) return;
    if (iMfFound == 0) {
        setStatus(tr("Run the key check first"));
        return;
    }
    setBusy(true);
    iMfDump.clear();
    iMfDumpBlocks = 0;
    emit mfDumpChanged();

    const int sectors = 16;
    QStringList lines;
    int ok = 0;

    for (int s = 0; s < sectors; s++) {
        setStatus(tr("Dumping sector %1/%2...").arg(s + 1).arg(sectors));
        // Prefer key A for reading; fall back to key B.
        const bool useA = !iMfKeyA[s].isEmpty();
        const QByteArray key = useA ? iMfKeyA[s] : iMfKeyB[s];
        const quint8 keyType = useA ? 0 : 1;

        for (int b = 0; b < 4; b++) {
            const quint8 block = (quint8)(s * 4 + b);
            QByteArray blk = key.isEmpty() ? QByteArray()
                                           : mfReadBlock(block, keyType, key);
            if (blk.size() == 16) {
                if (b == 3) {
                    // Trailer: the read masks the key bytes. Rebuild from the
                    // keys we already know, keep the access bytes (6..9).
                    if (iMfKeyA[s].size() == 6) blk.replace(0, 6, iMfKeyA[s]);
                    if (iMfKeyB[s].size() == 6) blk.replace(10, 6, iMfKeyB[s]);
                }
                ok++;
            } else {
                blk = QByteArray(16, 0);   // unreadable block -> zeros
            }
            lines << QStringLiteral("%1  %2")
                .arg(block, 3, 10, QChar('0'))
                .arg(QString::fromLatin1(blk.toHex()).toUpper());
        }
    }

    iMfDump = lines.join(QStringLiteral("\n"));
    iMfDumpBlocks = ok;
    emit mfDumpChanged();
    setStatus(tr("Dumped %1 of %2 blocks").arg(ok).arg(sectors * 4));
    setBusy(false);
}

void Proxmark::readHf()
{
    if (iFd < 0) return;
    setBusy(true);
    iHfUid.clear(); iHfType.clear(); iHfDetails.clear();
    emit hfChanged();

    tcflush(iFd, TCIFLUSH);
    iRx.clear();
    setStatus(tr("Reading 13.56 MHz - hold the card on the HF antenna..."));

    // MIX command: args packed as three little-endian uint64 in the data.
    const quint64 arg0 = ISO14A_CONNECT | ISO14A_CLEARTRACE;
    QByteArray p;
    for (int i = 0; i < 8; i++) p.append((char)((arg0 >> (8 * i)) & 0xff));   // arg0
    for (int i = 0; i < 16; i++) p.append((char)0);                           // arg1, arg2
    if (!sendCommand(CMD_HF_ISO14443A_READER, p, /*ng=*/false)) {
        setStatus(tr("Could not start HF read"));
        setBusy(false);
        return;
    }

    quint16 rcmd = 0;
    qint8 st = 0;
    QByteArray data;
    bool got = false;
    for (int i = 0; i < 8; i++) {
        if (!readResponse(&rcmd, &st, &data, 2500)) break;
        if (rcmd == CMD_DEBUG_PRINT) {
            emit logLine(QStringLiteral("PM3: ") + QString::fromLatin1(data));
            continue;
        }
        if (rcmd == CMD_ACK) { got = true; break; }
    }

    // Response data: [arg0 arg1 arg2 (3x u64)] + iso14a_card_select_t.
    // select_status is the low byte of arg0: 0 = no card selected.
    if (!got || data.size() < 24 + 38) {
        setStatus(tr("No card in the HF field"));
        setBusy(false);
        return;
    }
    const quint8 selectStatus = (quint8)data.at(0);
    if (selectStatus == 0) {
        setStatus(tr("No 13.56 MHz card found"));
        setBusy(false);
        return;
    }

    const int base = 24;                         // struct starts after the args
    const int uidlen = (quint8)data.at(base + 10);
    QByteArray uid = data.mid(base, qBound(0, uidlen, 10));
    const quint8 atqa0 = (quint8)data.at(base + 11);
    const quint8 atqa1 = (quint8)data.at(base + 12);
    const quint8 sak = (quint8)data.at(base + 13);

    bool isClassic = false;
    const QString type = sakType(sak, &isClassic);

    iHfUid = hexColon(uid);
    iHfType = type;
    iHfDetails = tr("ATQA %1%2 · SAK %3")
        .arg(QString("%1").arg(atqa1, 2, 16, QChar('0')).toUpper())
        .arg(QString("%1").arg(atqa0, 2, 16, QChar('0')).toUpper())
        .arg(QString("%1").arg(sak, 2, 16, QChar('0')).toUpper());
    if (isClassic) {
        iHfDetails += QStringLiteral(" · ") +
            tr("Crypto1 - the phone's own stack cannot read this; the Proxmark can");
    }
    emit hfChanged();
    setStatus(tr("%1 · UID %2").arg(type).arg(iHfUid));
    setBusy(false);
}

void Proxmark::writeEm4100(const QString& hexId)
{
    if (iFd < 0) return;
    setBusy(true);

    bool ok = false;
    const quint64 id = hexId.trimmed().toULongLong(&ok, 16);
    if (!ok) {
        setStatus(tr("Not a valid hex id: %1").arg(hexId));
        setBusy(false);
        return;
    }

    // CMD_LF_EM410X_CLONE payload (pm3_cmd.h, PACKED):
    //   bool Q5; bool EM; bool add_electra; uint8_t clock; u32 high; u32 low
    // Q5=0, EM=0 -> a plain T5577. clock 64 = the EM4100 RF/64 standard.
    QByteArray p;
    p.append((char)0);            // Q5
    p.append((char)0);            // EM
    p.append((char)0);            // add_electra
    p.append((char)64);           // clock
    const quint32 high = (quint32)((id >> 32) & 0xffffffff);
    const quint32 low  = (quint32)(id & 0xffffffff);
    for (int i = 0; i < 4; i++) p.append((char)((high >> (8 * i)) & 0xff));
    for (int i = 0; i < 4; i++) p.append((char)((low  >> (8 * i)) & 0xff));

    tcflush(iFd, TCIFLUSH);
    iRx.clear();
    setStatus(tr("Writing %1 to T5577 - hold the tag on the LF antenna...").arg(hexId.toUpper()));

    if (!sendCommand(CMD_LF_EM410X_CLONE, p)) {
        setStatus(tr("Could not start the write"));
        setBusy(false);
        return;
    }

    quint16 rcmd = 0;
    qint8 st = 0;
    QByteArray data;
    bool done = false, success = false;
    for (int i = 0; i < 8; i++) {
        if (!readResponse(&rcmd, &st, &data, 3000)) break;
        if (rcmd == CMD_DEBUG_PRINT) {
            emit logLine(QStringLiteral("PM3: ") + QString::fromLatin1(data));
            continue;
        }
        if (rcmd == CMD_LF_EM410X_CLONE) {
            done = true;
            success = (st == 0);   // PM3_SUCCESS
            break;
        }
    }

    if (success) {
        setStatus(tr("Written: T5577 now answers as EM4100 %1").arg(hexId.toUpper()));
    } else if (done) {
        setStatus(tr("Write rejected by the device (no tag, or not a T5577?)"));
    } else {
        setStatus(tr("No reply to the write - was a tag on the antenna?"));
    }
    setBusy(false);
}

void Proxmark::readLf()
{
    if (iFd < 0) return;
    setBusy(true);
    iLastTag.clear();
    iLastTagType.clear();
    emit lastTagChanged();
    setStatus(tr("Searching 125 kHz - hold the tag on the LF antenna..."));

    struct Probe { quint16 cmd; const char* type; const char* marker; bool hex; };
    static const Probe probes[] = {
        { CMD_LF_EM410X_WATCH, "EM4100",   "EM TAG ID:", true  },
        { CMD_LF_HID_WATCH,    "HID Prox", "TAG ID:",    true  },
        { CMD_LF_IO_WATCH,     "IO Prox",  "IO Prox",    false }
    };

    QString id;
    QString type;
    for (int i = 0; i < 3; i++) {
        setStatus(tr("Trying %1...").arg(QString::fromLatin1(probes[i].type)));
        if (watchProbe(probes[i].cmd, QString::fromLatin1(probes[i].marker),
                probes[i].hex, &id)) {
            type = QString::fromLatin1(probes[i].type);
            break;
        }
    }

    if (!type.isEmpty()) {
        iLastTag = id;
        iLastTagType = type;
        emit lastTagChanged();
        setStatus(tr("%1: %2").arg(type).arg(id));
    } else {
        setStatus(tr("No 125 kHz badge found (blank tag, or an unsupported format)"));
    }
    setBusy(false);
}
