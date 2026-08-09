/*
 * iNFC - NDEF record decoding and message building.
 */
#ifndef NDEFUTIL_H
#define NDEFUTIL_H

#include <QByteArray>
#include <QString>
#include <QVariantMap>

namespace NdefUtil {

enum Tnf {
    TnfEmpty       = 0,
    TnfWellKnown   = 1,
    TnfMediaType   = 2,
    TnfAbsoluteUri = 3,
    TnfExternal    = 4
};

// Turn one record into something the UI can show.
// Keys: tnf, tnfName, type, kind, title, text, uri, raw
QVariantMap describe(uint tnf, const QByteArray& type, const QByteArray& id,
    const QByteArray& payload);

// Build a complete single record NDEF message (MB|ME set), ready to be
// written to a tag or served by the emulator.
QByteArray buildText(const QString& text, const QString& lang = QStringLiteral("en"));
QByteArray buildUri(const QString& uri);

// Expand the well known URI prefix byte back into a readable URI.
QString expandUri(const QByteArray& payload);

} // namespace NdefUtil

#endif // NDEFUTIL_H
