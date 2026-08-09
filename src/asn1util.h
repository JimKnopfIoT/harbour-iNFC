/*
 * iNFC - just enough DER to make EF.CardAccess readable.
 *
 * EF.CardAccess is a SET OF SecurityInfo (BSI TR-03110). Every entry starts
 * with an object identifier saying which protocol the card offers, usually
 * followed by a version and a parameter or key id. Decoding it costs a few
 * hundred lines and turns an unreadable hex blob into the one thing that
 * actually matters: what this document is willing to talk.
 */
#ifndef ASN1UTIL_H
#define ASN1UTIL_H

#include <QByteArray>
#include <QString>
#include <QVariantList>

namespace Asn1 {

// Returns one entry per SecurityInfo:
//   { "label": <protocol name>, "value": <details>, "oid": <dotted> }
QVariantList decodeSecurityInfos(const QByteArray& der);

// Dotted decimal form of a DER encoded OID value (contents, no tag/length).
QString oidToString(const QByteArray& contents);

} // namespace Asn1

#endif // ASN1UTIL_H
