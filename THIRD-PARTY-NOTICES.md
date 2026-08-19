# Third-party notices

iNFC is original work under GPLv3. It contains no third-party source code, but
it builds on the following external work and public specifications.

## Runtime platform

* **nfcd** — the SailfishOS NFC daemon by Slava Monich / Jolla. iNFC is a
  D-Bus client of its `org.sailfishos.nfc.*` interfaces.
  <https://github.com/sailfishos/nfcd>
* **SailfishOS** and its Silica UI.

## Protocols and reference material (no code copied)

* **Proxmark 3 / Iceman firmware** (RfidResearchGroup) — the `um` variant
  speaks the Proxmark wire protocol directly. Command ids and frame layout were
  taken from the public headers (`pm3_cmd.h`, `mifare.h`) and client sources;
  no Proxmark code is included. GPLv3.
  <https://github.com/RfidResearchGroup/proxmark3>
* **MIFARE Classic default keys** — the well-known factory key list, public
  knowledge used by every RFID tool.
* **BSI TR-03110 / ICAO 9303** — the `EF.CardAccess` (PACE SecurityInfos)
  decoder follows these public specifications.
* **gematik** specifications for the electronic health card and e-prescription
  (informational text shown on the health-card panel).
* **NFC Forum** NDEF, RTD-URI and Type 2/Type 4 Tag specifications.

## Trademarks

MIFARE and DESFire are trademarks of NXP. Proxmark is a community project.
Philips, Sonicare and BrushSync are trademarks of Koninklijke Philips N.V.
All product names are used for identification only; iNFC is not affiliated with
or endorsed by any of them.
