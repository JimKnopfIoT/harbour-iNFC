<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="de">
<context>
    <name>ArchivePanel</name>
    <message>
        <source>Archive</source>
        <translation>Archiv</translation>
    </message>
    <message numerus="yes">
        <source>%n saved card(s)</source>
        <translation>
            <numerusform>%n gespeicherte Karte</numerusform>
            <numerusform>%n gespeicherte Karten</numerusform>
        </translation>
    </message>
    <message>
        <source>Load into Read</source>
        <translation>In Eingangsseite laden</translation>
    </message>
    <message>
        <source>Comment</source>
        <translation>Kommentar</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Löschen</translation>
    </message>
    <message>
        <source>no UID</source>
        <translation>keine UID</translation>
    </message>
    <message>
        <source>Nothing saved yet. Read a tag and pull down to Save.</source>
        <translation>Noch nichts gespeichert. Einen Tag lesen und im Pulley-Menü „Speichern“ wählen.</translation>
    </message>
    <message>
        <source>How this is stored</source>
        <translation>Wie das gespeichert wird</translation>
    </message>
    <message>
        <source>Card data goes to cards/, your comments go to notes/, in two separate files</source>
        <translation>Kartendaten liegen in cards/, deine Kommentare in notes/ — zwei getrennte Dateien</translation>
    </message>
    <message>
        <source>Writing to a tag only ever draws from the card data, so a comment cannot end up on a card</source>
        <translation>Beim Schreiben auf einen Tag werden ausschließlich die Kartendaten verwendet, ein Kommentar kann also nicht auf eine Karte geraten</translation>
    </message>
    <message>
        <source>Export can include the comments or leave them behind</source>
        <translation>Der Export kann die Kommentare enthalten oder weglassen</translation>
    </message>
    <message>
        <source>Search name, comment, type or UID</source>
        <translation>Name, Kommentar, Typ oder UID suchen</translation>
    </message>
</context>
<context>
    <name>CardStore</name>
    <message>
        <source>Export failed: cannot write %1</source>
        <translation>Export fehlgeschlagen: %1 nicht schreibbar</translation>
    </message>
    <message numerus="yes">
        <source>Exported %n card(s) to %1</source>
        <translation>
            <numerusform>%n Karte nach %1 exportiert</numerusform>
            <numerusform>%n Karten nach %1 exportiert</numerusform>
        </translation>
    </message>
    <message>
        <source>Import failed: cannot read %1</source>
        <translation>Import fehlgeschlagen: %1 nicht lesbar</translation>
    </message>
    <message numerus="yes">
        <source>Imported %n card(s)</source>
        <translation>
            <numerusform>%n Karte importiert</numerusform>
            <numerusform>%n Karten importiert</numerusform>
        </translation>
    </message>
</context>
<context>
    <name>CommentDialog</name>
    <message>
        <source>Comment saved for %1</source>
        <translation>Kommentar zu %1 gespeichert</translation>
    </message>
    <message>
        <source>Comment</source>
        <translation>Kommentar</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Speichern</translation>
    </message>
    <message>
        <source>Your notes about this card</source>
        <translation>Deine Notizen zu dieser Karte</translation>
    </message>
    <message>
        <source>Where it came from, what it opens, what you tried</source>
        <translation>Woher sie stammt, was sie öffnet, was du versucht hast</translation>
    </message>
    <message>
        <source>Stored separately from the card data. Writing to a tag never includes this text.</source>
        <translation>Getrennt von den Kartendaten gespeichert. Beim Schreiben auf einen Tag ist dieser Text nie dabei.</translation>
    </message>
    <message>
        <source>Save the card first, then a comment can be attached to it.</source>
        <translation>Erst die Karte speichern, dann lässt sich ein Kommentar anhängen.</translation>
    </message>
</context>
<context>
    <name>CoverPage</name>
    <message>
        <source>sending</source>
        <translation>sendet</translation>
    </message>
    <message>
        <source>waiting for tag</source>
        <translation>wartet auf Tag</translation>
    </message>
    <message>
        <source>ready</source>
        <translation>bereit</translation>
    </message>
</context>
<context>
    <name>DataField</name>
    <message>
        <source>sendable</source>
        <translation>sendefähig</translation>
    </message>
    <message>
        <source>tap to fold</source>
        <translation>tippen zum Einklappen</translation>
    </message>
    <message>
        <source>%1 characters · tap to expand</source>
        <translation>%1 Zeichen · tippen zum Ausklappen</translation>
    </message>
</context>
<context>
    <name>EidPanel</name>
    <message>
        <source>Identity documents</source>
        <translation>Ausweisdokumente</translation>
    </message>
    <message>
        <source>eID, ePassport, PACE protected cards</source>
        <translation>eID, ePass, PACE-geschützte Karten</translation>
    </message>
    <message>
        <source>No identity document in the current read.</source>
        <translation>Kein Ausweisdokument im aktuellen Lesevorgang.</translation>
    </message>
    <message>
        <source>German identity card</source>
        <translation>Personalausweis</translation>
    </message>
    <message>
        <source>Detecting the card and reading EF.CardAccess without any secret</source>
        <translation>Die Karte erkennen und EF.CardAccess ohne jedes Geheimnis lesen</translation>
    </message>
    <message>
        <source>Establishing a PACE channel with the CAN, the MRZ or the PIN</source>
        <translation>Einen PACE-Kanal mit CAN, MRZ oder PIN aufbauen</translation>
    </message>
    <message>
        <source>Reading any personal data. The eID application only releases fields after Terminal Authentication, where the chip verifies an authorisation certificate issued by the Federal Office of Administration under §21b PAuswG - and then only the fields that certificate names.</source>
        <translation>Personendaten lesen. Die eID-Anwendung gibt Felder erst nach der Terminal Authentication frei, bei der der Chip ein Berechtigungszertifikat der Vergabestelle beim Bundesverwaltungsamt nach §21b PAuswG prüft — und dann nur die dort genannten Felder.</translation>
    </message>
    <message>
        <source>The ePassport application (photo, fingerprints) is reserved for sovereign inspection systems under EAC.</source>
        <translation>Die ePass-Anwendung (Lichtbild, Fingerabdrücke) ist hoheitlichen Kontrollsystemen unter EAC vorbehalten.</translation>
    </message>
    <message>
        <source>There is no stable UID to recognise the card by: the NFCID1 is randomised on every activation, on purpose.</source>
        <translation>Es gibt keine feste UID, an der man die Karte wiedererkennen könnte: die NFCID1 wird bei jeder Aktivierung neu zufällig erzeugt, mit Absicht.</translation>
    </message>
    <message>
        <source>This is not a SailfishOS limitation. No self-written app on any platform can read an eID without a certificate and an eID server.</source>
        <translation>Das ist keine Einschränkung von SailfishOS. Keine selbstgeschriebene App kann auf irgendeiner Plattform eine eID ohne Zertifikat und eID-Server lesen.</translation>
    </message>
    <message>
        <source>Travel documents</source>
        <translation>Reisedokumente</translation>
    </message>
    <message>
        <source>Passports follow ICAO 9303: the keys are derived from the MRZ or the CAN, which you can read off the document yourself</source>
        <translation>Reisepässe folgen ICAO 9303: die Schlüssel werden aus der MRZ oder der CAN abgeleitet, die du selbst vom Dokument ablesen kannst</translation>
    </message>
    <message>
        <source>With those, DG1 (the MRZ data) and DG2 (the face image) are readable - the classic hobby project</source>
        <translation>Damit sind DG1 (die MRZ-Daten) und DG2 (das Gesichtsbild) lesbar — das klassische Bastelprojekt</translation>
    </message>
    <message>
        <source>DG3 and DG4, fingerprints and iris, need Extended Access Control with a certificate</source>
        <translation>DG3 und DG4, Fingerabdrücke und Iris, brauchen Extended Access Control mit Zertifikat</translation>
    </message>
    <message>
        <source>The identity card&apos;s own ePassport application is restricted the same way</source>
        <translation>Die ePass-Anwendung des Personalausweises ist genauso beschränkt</translation>
    </message>
    <message>
        <source>Using this phone as a card reader for software that already has the authorisation - the vpcd relay is planned for the next version</source>
        <translation>Dieses Telefon als Kartenleser für Software nutzen, die die Berechtigung bereits hat — das vpcd-Relay ist für die nächste Version geplant</translation>
    </message>
    <message>
        <source>Frequency</source>
        <translation>Frequenz</translation>
    </message>
</context>
<context>
    <name>EmulatePanel</name>
    <message>
        <source>Emulate</source>
        <translation>Emulieren</translation>
    </message>
    <message>
        <source>Type 4A tag, host card emulation</source>
        <translation>Type-4A-Tag, Host Card Emulation</translation>
    </message>
    <message>
        <source>not supported on this device</source>
        <translation>auf diesem Gerät nicht unterstützt</translation>
    </message>
    <message>
        <source>Sending</source>
        <translation>Sendet</translation>
    </message>
    <message>
        <source>Stopped</source>
        <translation>Gestoppt</translation>
    </message>
    <message>
        <source>Payload</source>
        <translation>Nutzdaten</translation>
    </message>
    <message>
        <source>Last reader event</source>
        <translation>Letztes Leser-Ereignis</translation>
    </message>
    <message>
        <source>none yet</source>
        <translation>noch keines</translation>
    </message>
    <message>
        <source>Stop</source>
        <translation>Stopp</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Start</translation>
    </message>
    <message>
        <source>Presented identity</source>
        <translation>Präsentierte Kennung</translation>
    </message>
    <message>
        <source>NFCID1, 4 bytes hex, blank for the default</source>
        <translation>NFCID1, 4 Byte hex, leer für den Standard</translation>
    </message>
    <message>
        <source>nfcd 1.2.2 and later lets the listening NFCID1 be set. Only single size (4 byte) identifiers are meaningful, and the controller may still override it.</source>
        <translation>Ab nfcd 1.2.2 lässt sich die NFCID1 im Listen-Modus setzen. Sinnvoll sind nur Kennungen einfacher Länge (4 Byte), und der Controller kann sie trotzdem überschreiben.</translation>
    </message>
    <message>
        <source>What emulation is</source>
        <translation>Was Emulation ist</translation>
    </message>
    <message>
        <source>The phone answers as a Type 4A tag holding an NDEF message - readers, other phones and NDEF terminals accept it</source>
        <translation>Das Telefon antwortet als Type-4A-Tag mit einer NDEF-Nachricht — Leser, andere Telefone und NDEF-Terminals nehmen das an</translation>
    </message>
    <message>
        <source>An NDEF message captured from a real tag can be served again unchanged</source>
        <translation>Eine von einem echten Tag gelesene NDEF-Nachricht lässt sich unverändert wieder ausgeben</translation>
    </message>
    <message>
        <source>The presented NFCID1 and the ATS historical bytes are configurable</source>
        <translation>Die präsentierte NFCID1 und die Historical Bytes des ATS sind einstellbar</translation>
    </message>
    <message>
        <source>Emulating a Type 2 tag. Host card emulation is always ISO-DEP.</source>
        <translation>Ein Type-2-Tag emulieren. Host Card Emulation ist immer ISO-DEP.</translation>
    </message>
    <message>
        <source>Emulating MIFARE Classic, or any card whose security rests on a key the phone does not have.</source>
        <translation>MIFARE Classic emulieren, oder jede Karte, deren Sicherheit auf einem Schlüssel beruht, den das Telefon nicht hat.</translation>
    </message>
    <message>
        <source>Payment. See the Payment card panel for the full reason.</source>
        <translation>Bezahlen. Die vollständige Begründung steht auf der Seite „Bezahlkarte“.</translation>
    </message>
    <message>
        <source>Card emulation needs nfcd 1.2.0 or newer, which arrived with SailfishOS 5.0.</source>
        <translation>Kartenemulation braucht nfcd 1.2.0 oder neuer, verfügbar seit SailfishOS 5.0.</translation>
    </message>
</context>
<context>
    <name>Emulator</name>
    <message>
        <source>Text: %1</source>
        <translation>Text: %1</translation>
    </message>
    <message>
        <source>URI: %1</source>
        <translation>URI: %1</translation>
    </message>
    <message>
        <source>Captured message, %1 bytes</source>
        <translation>Aufgezeichnete Nachricht, %1 Byte</translation>
    </message>
    <message>
        <source>Cannot enter card emulation: %1</source>
        <translation>Kartenemulation nicht möglich: %1</translation>
    </message>
    <message>
        <source>Cannot register NDEF application: %1</source>
        <translation>NDEF-Anwendung konnte nicht registriert werden: %1</translation>
    </message>
    <message>
        <source>Emulating - hold the phone against a reader</source>
        <translation>Emuliert — Telefon an einen Leser halten</translation>
    </message>
    <message>
        <source>Stopped</source>
        <translation>Gestoppt</translation>
    </message>
</context>
<context>
    <name>ExportDialog</name>
    <message>
        <source>Export</source>
        <translation>Exportieren</translation>
    </message>
    <message>
        <source>Include comments</source>
        <translation>Kommentare einschließen</translation>
    </message>
    <message>
        <source>Off keeps your notes out of the exported file</source>
        <translation>Aus bedeutet: deine Notizen bleiben aus der Exportdatei heraus</translation>
    </message>
    <message numerus="yes">
        <source>%n saved card(s) will be written as a single JSON file.</source>
        <translation>
            <numerusform>%n gespeicherte Karte wird als eine JSON-Datei geschrieben.</numerusform>
            <numerusform>%n gespeicherte Karten werden als eine JSON-Datei geschrieben.</numerusform>
        </translation>
    </message>
    <message>
        <source>File name</source>
        <translation>Dateiname</translation>
    </message>
    <message>
        <source>Target folder: %1</source>
        <translation>Zielordner: %1</translation>
    </message>
</context>
<context>
    <name>FrequencyBand</name>
    <message>
        <source>EM4100, HID Prox, T5577</source>
        <translation>EM4100, HID Prox, T5577</translation>
    </message>
    <message>
        <source>animal ID, FDX-B</source>
        <translation>Tierkennung, FDX-B</translation>
    </message>
    <message>
        <source>ISO 14443, ISO 15693, NFC</source>
        <translation>ISO 14443, ISO 15693, NFC</translation>
    </message>
    <message>
        <source>EPC Gen2, long range</source>
        <translation>EPC Gen2, Fernbereich</translation>
    </message>
    <message>
        <source>active tags</source>
        <translation>aktive Tags</translation>
    </message>
</context>
<context>
    <name>Glossary</name>
    <message>
        <source>Reference</source>
        <translation>Nachschlagen</translation>
    </message>
    <message>
        <source>Technology and protocol</source>
        <translation>Technologie und Protokoll</translation>
    </message>
    <message>
        <source>Two separate questions. The technology is the radio layer - how bits are put on the 13.56 MHz carrier. The protocol is what runs on top of it once the card is activated.</source>
        <translation>Zwei verschiedene Fragen. Die Technologie ist die Funkebene — wie Bits auf den 13,56-MHz-Träger kommen. Das Protokoll ist das, was darauf läuft, sobald die Karte aktiviert ist.</translation>
    </message>
    <message>
        <source>So &quot;Type 4A (ISO-DEP)&quot; means: activated over the Type A air interface, and a full smart card underneath. That combination is what makes selecting applications by AID possible at all - a Type 2 tag has no such notion, it is just memory.</source>
        <translation>„Type 4A (ISO-DEP)“ heißt also: über die Typ-A-Luftschnittstelle aktiviert, und darunter eine vollwertige Smartcard. Erst diese Kombination macht das Auswählen von Anwendungen per AID überhaupt möglich — ein Type-2-Tag kennt so etwas nicht, das ist nur Speicher.</translation>
    </message>
    <message>
        <source>ISO/IEC 14443 Type A. The common case: NTAG, MIFARE, most bank cards, most phones doing card emulation.</source>
        <translation>ISO/IEC 14443 Typ A. Der Normalfall: NTAG, MIFARE, die meisten Bankkarten, die meisten Telefone bei der Kartenemulation.</translation>
    </message>
    <message>
        <source>ISO/IEC 14443 Type B. Different modulation and a different anticollision scheme, same 13.56 MHz. German identity cards and a good share of payment cards are Type B.</source>
        <translation>ISO/IEC 14443 Typ B. Andere Modulation, anderes Anticollision-Verfahren, dieselben 13,56 MHz. Deutsche Personalausweise und ein guter Teil der Bezahlkarten sind Typ B.</translation>
    </message>
    <message>
        <source>FeliCa, mostly Japan - transit and payment. This phone does not poll for it at all: the controller reports NFC-A and NFC-B only.</source>
        <translation>FeliCa, überwiegend Japan — Nahverkehr und Bezahlen. Dieses Telefon sucht gar nicht danach: der Controller meldet nur NFC-A und NFC-B.</translation>
    </message>
    <message>
        <source>Topaz/Jewel. Simple memory, rare today.</source>
        <translation>Topaz/Jewel. Einfacher Speicher, heute selten.</translation>
    </message>
    <message>
        <source>NTAG21x, MIFARE Ultralight. Memory in 4 byte blocks, read and written directly. No applications, no APDUs - the whole tag is one flat address space.</source>
        <translation>NTAG21x, MIFARE Ultralight. Speicher in 4-Byte-Blöcken, direkt gelesen und geschrieben. Keine Anwendungen, keine APDUs — der ganze Tag ist ein flacher Adressraum.</translation>
    </message>
    <message>
        <source>FeliCa based. Not reachable here, see NFC-F.</source>
        <translation>Auf FeliCa aufgesetzt. Hier nicht erreichbar, siehe NFC-F.</translation>
    </message>
    <message>
        <source>A full smart card, spoken to with APDUs over ISO-DEP (ISO/IEC 14443-4), activated over the Type A interface. It holds applications selected by identifier, each with its own files and access rules. Bank cards, DESFire, identity documents, security tokens.</source>
        <translation>Eine vollwertige Smartcard, angesprochen mit APDUs über ISO-DEP (ISO/IEC 14443-4), aktiviert über die Typ-A-Schnittstelle. Sie enthält Anwendungen, die per Kennung ausgewählt werden, jede mit eigenen Dateien und Zugriffsregeln. Bankkarten, DESFire, Ausweisdokumente, Sicherheitstoken.</translation>
    </message>
    <message>
        <source>The same smart card layer, activated over the Type B interface instead. Everything above the air interface is identical.</source>
        <translation>Dieselbe Smartcard-Ebene, nur über die Typ-B-Schnittstelle aktiviert. Alles oberhalb der Luftschnittstelle ist identisch.</translation>
    </message>
    <message>
        <source>Peer to peer, ISO 18092. Two devices talking as equals rather than card and reader. Effectively dead since Android dropped it.</source>
        <translation>Peer-to-Peer, ISO 18092. Zwei Geräte reden als Gleichrangige statt als Karte und Leser. Faktisch tot, seit Android es aufgegeben hat.</translation>
    </message>
    <message>
        <source>Anticollision: NFCID1 and SAK</source>
        <translation>Anticollision: NFCID1 und SAK</translation>
    </message>
    <message>
        <source>What a card reveals before any application is selected. This is the only information available for cards the stack cannot otherwise reach.</source>
        <translation>Was eine Karte preisgibt, bevor irgendeine Anwendung ausgewählt wird. Bei Karten, die der Stack sonst nicht erreicht, ist das die einzige verfügbare Information.</translation>
    </message>
    <message>
        <source>SAK is the fastest way to tell what you are holding: bit 0x20 means ISO 14443-4 and an ATS follows, 0x08 and 0x18 mean MIFARE Classic, 0x00 means a Type 2 tag.</source>
        <translation>SAK ist der schnellste Weg zu erkennen, was man in der Hand hält: Bit 0x20 heißt ISO 14443-4 und es folgt ein ATS, 0x08 und 0x18 heißen MIFARE Classic, 0x00 heißt Type-2-Tag.</translation>
    </message>
    <message>
        <source>The identifier used during anticollision, 4, 7 or 10 bytes. 4 bytes is a single size UID, 7 a double, 10 a triple. A 4 byte value starting with 08 is not an identity at all - it is randomly generated for this session, which passports, identity cards and payment cards all do on purpose.</source>
        <translation>Die bei der Anticollision verwendete Kennung, 4, 7 oder 10 Byte. 4 Byte sind eine einfache UID, 7 eine doppelte, 10 eine dreifache. Ein 4-Byte-Wert, der mit 08 beginnt, ist überhaupt keine Identität — er wird für diese Sitzung zufällig erzeugt, was Reisepässe, Personalausweise und Bezahlkarten alle mit Absicht tun.</translation>
    </message>
    <message>
        <source>Type 2 tag - MIFARE Ultralight, NTAG213/215/216. Read and write works here.</source>
        <translation>Type-2-Tag — MIFARE Ultralight, NTAG213/215/216. Lesen und Schreiben funktioniert hier.</translation>
    </message>
    <message>
        <source>MIFARE Classic 1K / Mini. Not reachable from SailfishOS - see the Other panel.</source>
        <translation>MIFARE Classic 1K / Mini. Von SailfishOS aus nicht erreichbar — siehe Seite „Sonstige“.</translation>
    </message>
    <message>
        <source>MIFARE Classic 4K. Same story.</source>
        <translation>MIFARE Classic 4K. Dasselbe.</translation>
    </message>
    <message>
        <source>Plain ISO 14443-4. DESFire, bank cards, identity documents, another phone doing card emulation.</source>
        <translation>Reines ISO 14443-4. DESFire, Bankkarten, Ausweisdokumente, ein anderes Telefon bei der Kartenemulation.</translation>
    </message>
    <message>
        <source>Both at once: a Classic part plus an ISO 14443-4 part. This app reaches the second half and not the first.</source>
        <translation>Beides zugleich: ein Classic-Teil und ein ISO-14443-4-Teil. Diese App erreicht die zweite Hälfte, die erste nicht.</translation>
    </message>
    <message>
        <source>Infineon SLE, Classic compatible. Same limitation.</source>
        <translation>Infineon SLE, Classic-kompatibel. Dieselbe Einschränkung.</translation>
    </message>
    <message>
        <source>Reading the ATS</source>
        <translation>Das ATS lesen</translation>
    </message>
    <message>
        <source>These four bytes come from the Answer To Select, with which an ISO 14443-4 card introduces itself after activation. They say nothing about what is on the card - only how one is allowed to talk to it.</source>
        <translation>Diese vier Byte stammen aus dem Answer To Select, mit dem sich eine ISO-14443-4-Karte nach der Aktivierung vorstellt. Sie sagen nichts darüber, was auf der Karte ist — nur, wie man mit ihr reden darf.</translation>
    </message>
    <message>
        <source>In short: T0 says how large, TA how fast, TB how long, TC which addressing. FSCI and FWI are the two that matter in practice - a card with a small frame and a long waiting time is the one that makes probing slow.</source>
        <translation>Kurz: T0 sagt wie groß, TA wie schnell, TB wie lange, TC welche Adressierung. FSCI und FWI sind die beiden, die in der Praxis zählen — eine Karte mit kleinem Frame und langer Wartezeit ist die, bei der das Abfragen zäh wird.</translation>
    </message>
    <message>
        <source>Format byte. The lower four bits are the FSCI, the largest frame the card accepts (0 → 16 bytes … 8 → 256). The upper bits say which of the following bytes are present at all.</source>
        <translation>Formatbyte. Die unteren vier Bit sind das FSCI, der größte Frame, den die Karte annimmt (0 → 16 Byte … 8 → 256). Die oberen Bit sagen, welche der folgenden Byte überhaupt vorhanden sind.</translation>
    </message>
    <message>
        <source>Transmission rates. Which multiples of 106 kbit/s the card can do - 212, 424, 848 - separately per direction, plus one bit demanding the same rate both ways.</source>
        <translation>Übertragungsraten. Welche Vielfachen von 106 kbit/s die Karte kann — 212, 424, 848 — getrennt je Richtung, dazu ein Bit, das für beide Richtungen dieselbe Rate verlangt.</translation>
    </message>
    <message>
        <source>Timing. The upper nibble FWI sets how long the card may take to answer (FWT ≈ 302 µs × 2^FWI), the lower nibble SFGI the grace period after activation before the first command may arrive.</source>
        <translation>Zeiten. Das obere Nibble FWI legt fest, wie lange die Karte für eine Antwort brauchen darf (FWT ≈ 302 µs × 2^FWI), das untere Nibble SFGI die Schonfrist nach der Aktivierung, bevor das erste Kommando kommen darf.</translation>
    </message>
    <message>
        <source>Protocol options, two bits only: whether the card supports CID (card identifier, several cards in the field) and NAD (node addressing).</source>
        <translation>Protokolloptionen, nur zwei Bit: ob die Karte CID (Kartenkennung, mehrere Karten im Feld) und NAD (Knotenadressierung) unterstützt.</translation>
    </message>
    <message>
        <source>NFC-B parameters</source>
        <translation>NFC-B-Parameter</translation>
    </message>
    <message>
        <source>Type B cards answer with a different set. German identity cards and many payment cards are Type B.</source>
        <translation>Typ-B-Karten antworten mit einem anderen Satz. Deutsche Personalausweise und viele Bezahlkarten sind Typ B.</translation>
    </message>
    <message>
        <source>The Type B identifier, four bytes, taken from the answer to the request command. Usually random per session, for the same privacy reason as a random NFCID1.</source>
        <translation>Die Typ-B-Kennung, vier Byte, aus der Antwort auf das Request-Kommando. Meist pro Sitzung zufällig, aus demselben Datenschutzgrund wie eine zufällige NFCID1.</translation>
    </message>
    <message>
        <source>Application data from the same answer: which application families the card carries and how many there are. Coarse, but free.</source>
        <translation>Anwendungsdaten aus derselben Antwort: welche Anwendungsfamilien die Karte trägt und wie viele es sind. Grob, aber kostenlos.</translation>
    </message>
    <message>
        <source>Protocol info: maximum frame size, supported bit rates, and the waiting time - the Type B equivalent of T0, TA and TB rolled into three bytes.</source>
        <translation>Protokollinfo: maximale Framegröße, unterstützte Bitraten und die Wartezeit — das Typ-B-Gegenstück zu T0, TA und TB in drei Byte.</translation>
    </message>
    <message>
        <source>Maximum Buffer Length Index, sent at activation. Says how much the card can buffer beyond a single frame, as a power of two.</source>
        <translation>Maximum Buffer Length Index, bei der Aktivierung gesendet. Sagt als Zweierpotenz, wie viel die Karte über einen einzelnen Frame hinaus puffern kann.</translation>
    </message>
    <message>
        <source>Status words</source>
        <translation>Statuswörter</translation>
    </message>
    <message>
        <source>6A82 means the application is genuinely not there. 6982 means it is there and will not talk to you - that is what a German eID says to any terminal without an authorisation certificate.</source>
        <translation>6A82 heißt, die Anwendung ist tatsächlich nicht da. 6982 heißt, sie ist da und redet nicht mit dir — genau das antwortet ein deutscher Personalausweis jedem Terminal ohne Berechtigungszertifikat.</translation>
    </message>
    <message>
        <source>Success. Nothing further to say.</source>
        <translation>Erfolg. Mehr ist nicht zu sagen.</translation>
    </message>
    <message>
        <source>File or application not found. The probe treats this as absent.</source>
        <translation>Datei oder Anwendung nicht gefunden. Die Abfrage wertet das als nicht vorhanden.</translation>
    </message>
    <message>
        <source>Security status not satisfied. Present, but authentication is missing.</source>
        <translation>Sicherheitsstatus nicht erfüllt. Vorhanden, aber die Authentisierung fehlt.</translation>
    </message>
    <message>
        <source>Conditions of use not satisfied - right command, wrong moment.</source>
        <translation>Nutzungsbedingungen nicht erfüllt — richtiges Kommando, falscher Moment.</translation>
    </message>
    <message>
        <source>Selected file is blocked or invalidated.</source>
        <translation>Ausgewählte Datei ist gesperrt oder ungültig.</translation>
    </message>
    <message>
        <source>Verification failed. With a PIN, the retry counter has just gone down.</source>
        <translation>Prüfung fehlgeschlagen. Bei einer PIN ist gerade der Fehlbedienungszähler heruntergezählt worden.</translation>
    </message>
    <message>
        <source>Wrong length - Lc or Le did not fit the command.</source>
        <translation>Falsche Länge — Lc oder Le passten nicht zum Kommando.</translation>
    </message>
    <message>
        <source>Wrong P1/P2 parameters.</source>
        <translation>Falsche Parameter P1/P2.</translation>
    </message>
    <message>
        <source>Offset outside the file.</source>
        <translation>Offset außerhalb der Datei.</translation>
    </message>
    <message>
        <source>Class not supported: the card does not know this CLA at all.</source>
        <translation>Klasse nicht unterstützt: die Karte kennt dieses CLA gar nicht.</translation>
    </message>
    <message>
        <source>Success, and xx more bytes are waiting - fetch them with GET RESPONSE.</source>
        <translation>Erfolg, und xx weitere Byte warten — mit GET RESPONSE abholen.</translation>
    </message>
    <message>
        <source>Wrong Le; repeat the command with xx as the expected length.</source>
        <translation>Falsches Le; das Kommando mit xx als erwarteter Länge wiederholen.</translation>
    </message>
    <message>
        <source>Select Acknowledge, also called SEL_RES: one byte, the card&apos;s answer to being singled out. Bit 0x20 says it speaks ISO 14443-4 and an ATS follows; bit 0x40 says it speaks NFC-DEP; bit 0x04 says the identifier is not complete and another cascade level follows. The rest is manufacturer specific.</source>
        <translation>Select Acknowledge, auch SEL_RES: ein Byte, die Antwort der Karte darauf, ausgewählt worden zu sein. Bit 0x20 sagt, sie spricht ISO 14443-4 und es folgt ein ATS; Bit 0x40 sagt, sie spricht NFC-DEP; Bit 0x04 sagt, die Kennung ist noch nicht vollständig und eine weitere Kaskadenstufe folgt. Der Rest ist herstellerspezifisch.</translation>
    </message>
    <message>
        <source>Every APDU answer ends in two bytes, SW1 and SW2. A card that refuses is not a card that is absent - which is why the application probe reports the two differently.</source>
        <translation>Jede APDU-Antwort endet auf zwei Byte, SW1 und SW2. Eine Karte, die sich verweigert, ist keine Karte, die fehlt — deshalb meldet die Anwendungsabfrage beides unterschiedlich.</translation>
    </message>
</context>
<context>
    <name>HealthCardPanel</name>
    <message>
        <source>Health card</source>
        <translation>Gesundheitskarte</translation>
    </message>
    <message>
        <source>eGK generation G2.1</source>
        <translation>eGK der Generation G2.1</translation>
    </message>
    <message>
        <source>No health card in the current read. The reference below applies whenever one is found.</source>
        <translation>Keine Gesundheitskarte im aktuellen Lesevorgang. Die Erläuterung unten gilt, sobald eine erkannt wird.</translation>
    </message>
    <message>
        <source>What an eGK is for</source>
        <translation>Wofür eine eGK da ist</translation>
    </message>
    <message>
        <source>Detecting the card and its applications over NFC - every G2.1 card issued since January 2021 has a contactless antenna</source>
        <translation>Die Karte und ihre Anwendungen über NFC erkennen — jede seit Januar 2021 ausgegebene G2.1-Karte hat eine kontaktlose Antenne</translation>
    </message>
    <message>
        <source>Reading EF.CardAccess, which is free by design: it is how a terminal learns the PACE parameters</source>
        <translation>EF.CardAccess lesen, das absichtlich frei ist: so erfährt ein Terminal die PACE-Parameter</translation>
    </message>
    <message>
        <source>Opening a PACE channel with the six digit CAN printed on the front - the protocol is public and OpenSSL has the brainpool curves</source>
        <translation>Einen PACE-Kanal mit der sechsstelligen CAN von der Vorderseite aufbauen — das Verfahren ist öffentlich und OpenSSL hat die Brainpool-Kurven</translation>
    </message>
    <message>
        <source>Reading the insurance master data (VSD). That needs card-to-card authentication against a health professional card or an SMC-B, which no citizen device has.</source>
        <translation>Die Versichertenstammdaten (VSD) lesen. Das braucht eine Card-to-Card-Authentisierung gegen einen Heilberufsausweis oder eine SMC-B, die kein Bürgergerät besitzt.</translation>
    </message>
    <message>
        <source>Emergency data and the medication plan - same lock, same reason.</source>
        <translation>Notfalldaten und Medikationsplan — dieselbe Sperre, derselbe Grund.</translation>
    </message>
    <message>
        <source>Acting as a full e-prescription client: the flow itself is specified in public and gematik&apos;s reference code is open, but the central IDP only issues tokens to registered clients.</source>
        <translation>Als vollwertiger E-Rezept-Client auftreten: der Ablauf selbst ist öffentlich spezifiziert und der Referenzcode der gematik offen, aber der zentrale IDP stellt Token nur für registrierte Clients aus.</translation>
    </message>
    <message>
        <source>Sources: gematik eGK specification, gemSpec_IDP_Frontend, gematik/api-erp</source>
        <translation>Quellen: gematik-Spezifikation zur eGK, gemSpec_IDP_Frontend, gematik/api-erp</translation>
    </message>
    <message>
        <source>The prescription is not on the card</source>
        <translation>Das Rezept liegt nicht auf der Karte</translation>
    </message>
    <message>
        <source>The eGK is a key, not a container. Prescriptions live on the e-prescription server, the Fachdienst.</source>
        <translation>Die eGK ist ein Schlüssel, kein Behälter. Rezepte liegen auf dem E-Rezept-Server, dem Fachdienst.</translation>
    </message>
    <message>
        <source>The pharmacy reads the card only to prove you authorised it to fetch your open prescriptions.</source>
        <translation>Die Apotheke liest die Karte nur, um nachzuweisen, dass du sie zum Abruf deiner offenen Rezepte berechtigt hast.</translation>
    </message>
    <message>
        <source>Copying a prescription off the card - there is nothing there to copy, on any device, with any software.</source>
        <translation>Ein Rezept von der Karte kopieren — dort ist nichts zu kopieren, auf keinem Gerät, mit keiner Software.</translation>
    </message>
    <message>
        <source>This is stated identically by the Federal Ministry of Health, gematik and ABDA.</source>
        <translation>So steht es übereinstimmend beim Bundesgesundheitsministerium, bei der gematik und bei der ABDA.</translation>
    </message>
    <message>
        <source>Frequency</source>
        <translation>Frequenz</translation>
    </message>
</context>
<context>
    <name>HostAppAdaptor</name>
    <message>
        <source>Reader field detected</source>
        <translation>Leserfeld erkannt</translation>
    </message>
    <message>
        <source>Session restarted</source>
        <translation>Sitzung neu gestartet</translation>
    </message>
    <message>
        <source>Reader gone</source>
        <translation>Leser weg</translation>
    </message>
    <message>
        <source>Implicitly selected</source>
        <translation>Implizit ausgewählt</translation>
    </message>
    <message>
        <source>NDEF application selected</source>
        <translation>NDEF-Anwendung ausgewählt</translation>
    </message>
    <message>
        <source>SELECT CC file</source>
        <translation>SELECT CC-Datei</translation>
    </message>
    <message>
        <source>SELECT NDEF file</source>
        <translation>SELECT NDEF-Datei</translation>
    </message>
    <message>
        <source>READ BINARY %1 bytes @ %2</source>
        <translation>READ BINARY %1 Byte ab %2</translation>
    </message>
</context>
<context>
    <name>ImportDialog</name>
    <message>
        <source>Import</source>
        <translation>Importieren</translation>
    </message>
    <message>
        <source>Path to an iNFC export file</source>
        <translation>Pfad zu einer iNFC-Exportdatei</translation>
    </message>
    <message>
        <source>Imported cards are added, never merged over existing ones. Comments in the file are restored into your notes.</source>
        <translation>Importierte Karten werden hinzugefügt, nie über vorhandene gelegt. Kommentare aus der Datei landen wieder in deinen Notizen.</translation>
    </message>
</context>
<context>
    <name>InfoBlock</name>
    <message>
        <source>What works here</source>
        <translation>Was hier geht</translation>
    </message>
    <message>
        <source>What does not, and why</source>
        <translation>Was nicht geht, und warum</translation>
    </message>
</context>
<context>
    <name>LogPanel</name>
    <message>
        <source>Log</source>
        <translation>Protokoll</translation>
    </message>
    <message>
        <source>%1 entries</source>
        <translation>%1 Einträge</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation>Leeren</translation>
    </message>
    <message>
        <source>Nothing has happened yet.</source>
        <translation>Bisher ist nichts passiert.</translation>
    </message>
    <message>
        <source>About this log</source>
        <translation>Über dieses Protokoll</translation>
    </message>
    <message>
        <source>Everything this app does: arming the radio, reads, classifications, APDUs answered while emulating</source>
        <translation>Alles, was diese App tut: Funk scharfschalten, Lesevorgänge, Einstufungen, beim Emulieren beantwortete APDUs</translation>
    </message>
    <message>
        <source>nfcd&apos;s own internal trace is not shown here yet. It is available through the daemon&apos;s logging plugin and is on the list.</source>
        <translation>Das interne Protokoll von nfcd steht hier noch nicht. Es ist über das Logging-Plugin des Daemons verfügbar und steht auf der Liste.</translation>
    </message>
</context>
<context>
    <name>NdefPanel</name>
    <message>
        <source>NDEF</source>
        <translation>NDEF</translation>
    </message>
    <message>
        <source>Records read, written and sent</source>
        <translation>Records lesen, schreiben und senden</translation>
    </message>
    <message>
        <source>Record %1 · %2</source>
        <translation>Record %1 · %2</translation>
    </message>
    <message>
        <source>Type name format</source>
        <translation>Type Name Format</translation>
    </message>
    <message>
        <source>Content</source>
        <translation>Inhalt</translation>
    </message>
    <message>
        <source>Payload</source>
        <translation>Nutzdaten</translation>
    </message>
    <message>
        <source>No NDEF records in the current read.</source>
        <translation>Keine NDEF-Records im aktuellen Lesevorgang.</translation>
    </message>
    <message>
        <source>Compose</source>
        <translation>Verfassen</translation>
    </message>
    <message>
        <source>https://example.org</source>
        <translation>https://example.org</translation>
    </message>
    <message>
        <source>Text to store</source>
        <translation>Zu speichernder Text</translation>
    </message>
    <message>
        <source>URI record</source>
        <translation>URI-Record</translation>
    </message>
    <message>
        <source>Text record</source>
        <translation>Text-Record</translation>
    </message>
    <message>
        <source>Treat as URI</source>
        <translation>Als URI behandeln</translation>
    </message>
    <message>
        <source>Off means a plain text record</source>
        <translation>Aus bedeutet einen einfachen Text-Record</translation>
    </message>
    <message>
        <source>Write to tag</source>
        <translation>Auf Tag schreiben</translation>
    </message>
    <message>
        <source>Send it</source>
        <translation>Senden</translation>
    </message>
    <message>
        <source>NDEF on this phone</source>
        <translation>NDEF auf diesem Telefon</translation>
    </message>
    <message>
        <source>Reading records from Type 2 and Type 4 tags</source>
        <translation>Records von Type-2- und Type-4-Tags lesen</translation>
    </message>
    <message>
        <source>Writing text and URI records to writeable Type 2 tags (NTAG21x, Ultralight)</source>
        <translation>Text- und URI-Records auf beschreibbare Type-2-Tags schreiben (NTAG21x, Ultralight)</translation>
    </message>
    <message>
        <source>Sending a record as an emulated Type 4A tag, so another phone or a reader can pick it up</source>
        <translation>Einen Record als emuliertes Type-4A-Tag senden, sodass ein anderes Telefon oder ein Leser ihn aufnehmen kann</translation>
    </message>
    <message>
        <source>Writing to Type 4 cards - that needs the card&apos;s own write access conditions, which consumer cards do not grant</source>
        <translation>Auf Type-4-Karten schreiben — dafür bräuchte es die Schreibberechtigungen der Karte selbst, die Verbraucherkarten nicht gewähren</translation>
    </message>
    <message>
        <source>Locking or setting a password on a tag: nfcd exposes no lock bits</source>
        <translation>Einen Tag sperren oder mit Passwort versehen: nfcd legt die Lock-Bits nicht offen</translation>
    </message>
    <message>
        <source>Anything above 64 kB, the Type 4 specification&apos;s own ceiling</source>
        <translation>Alles über 64 kB, die Obergrenze der Type-4-Spezifikation selbst</translation>
    </message>
    <message>
        <source>Frequency</source>
        <translation>Frequenz</translation>
    </message>
</context>
<context>
    <name>NfcBackend</name>
    <message>
        <source>none</source>
        <translation>keine</translation>
    </message>
    <message>
        <source>nfcd went away</source>
        <translation>nfcd ist verschwunden</translation>
    </message>
    <message>
        <source>nfcd is not running</source>
        <translation>nfcd läuft nicht</translation>
    </message>
    <message>
        <source>NFC is off - enable it in Settings</source>
        <translation>NFC ist aus — in den Einstellungen einschalten</translation>
    </message>
    <message>
        <source>Hold a tag against the back of the phone</source>
        <translation>Einen Tag an die Rückseite des Telefons halten</translation>
    </message>
    <message>
        <source>Hold a writeable tag against the phone</source>
        <translation>Einen beschreibbaren Tag an das Telefon halten</translation>
    </message>
    <message>
        <source>Writing...</source>
        <translation>Schreibt …</translation>
    </message>
    <message>
        <source>Write failed: %1</source>
        <translation>Schreiben fehlgeschlagen: %1</translation>
    </message>
    <message>
        <source>Wrote %1 bytes</source>
        <translation>%1 Byte geschrieben</translation>
    </message>
    <message>
        <source>Reading...</source>
        <translation>Liest …</translation>
    </message>
    <message>
        <source>Could not read the tag: %1</source>
        <translation>Tag konnte nicht gelesen werden: %1</translation>
    </message>
    <message>
        <source>Read %1</source>
        <translation>%1 gelesen</translation>
    </message>
    <message>
        <source>Transport</source>
        <translation>Übertragung</translation>
    </message>
    <message>
        <source>Technology</source>
        <translation>Technologie</translation>
    </message>
    <message>
        <source>Protocol</source>
        <translation>Protokoll</translation>
    </message>
    <message>
        <source>NFCID1 (UID)</source>
        <translation>NFCID1 (UID)</translation>
    </message>
    <message>
        <source>UID type</source>
        <translation>UID-Art</translation>
    </message>
    <message>
        <source>random (re-generated per session)</source>
        <translation>zufällig (pro Sitzung neu erzeugt)</translation>
    </message>
    <message>
        <source>SAK (SEL_RES)</source>
        <translation>SAK (SEL_RES)</translation>
    </message>
    <message>
        <source>NFCID0</source>
        <translation>NFCID0</translation>
    </message>
    <message>
        <source>Application data</source>
        <translation>Anwendungsdaten</translation>
    </message>
    <message>
        <source>Protocol info</source>
        <translation>Protokollinfo</translation>
    </message>
    <message>
        <source>Memory</source>
        <translation>Speicher</translation>
    </message>
    <message>
        <source>Block size</source>
        <translation>Blockgröße</translation>
    </message>
    <message>
        <source>Data size</source>
        <translation>Datengröße</translation>
    </message>
    <message>
        <source>%1 bytes</source>
        <translation>%1 Byte</translation>
    </message>
    <message>
        <source>Serial</source>
        <translation>Seriennummer</translation>
    </message>
    <message>
        <source>Dump</source>
        <translation>Abzug</translation>
    </message>
    <message>
        <source>ISO-DEP</source>
        <translation>ISO-DEP</translation>
    </message>
    <message>
        <source>Historical bytes</source>
        <translation>Historical Bytes</translation>
    </message>
    <message>
        <source>Access control</source>
        <translation>Zugriffsschutz</translation>
    </message>
    <message>
        <source>PACE - needs CAN, MRZ or PIN</source>
        <translation>PACE — braucht CAN, MRZ oder PIN</translation>
    </message>
    <message>
        <source>Applications</source>
        <translation>Anwendungen</translation>
    </message>
    <message>
        <source>present</source>
        <translation>vorhanden</translation>
    </message>
    <message>
        <source>present, access denied (%1)</source>
        <translation>vorhanden, Zugriff verweigert (%1)</translation>
    </message>
    <message>
        <source>none of the known AIDs answered</source>
        <translation>keine der bekannten AIDs hat geantwortet</translation>
    </message>
    <message>
        <source>Content</source>
        <translation>Inhalt</translation>
    </message>
    <message>
        <source>NDEF records</source>
        <translation>NDEF-Records</translation>
    </message>
    <message numerus="yes">
        <source>%n record(s)</source>
        <translation>
            <numerusform>%n Record</numerusform>
            <numerusform>%n Records</numerusform>
        </translation>
    </message>
    <message>
        <source>Health card</source>
        <translation>Gesundheitskarte</translation>
    </message>
    <message>
        <source>Identity card (eID)</source>
        <translation>Personalausweis (eID)</translation>
    </message>
    <message>
        <source>Travel document</source>
        <translation>Reisedokument</translation>
    </message>
    <message>
        <source>Payment card</source>
        <translation>Bezahlkarte</translation>
    </message>
    <message>
        <source>Security token</source>
        <translation>Sicherheitstoken</translation>
    </message>
    <message>
        <source>NDEF tag</source>
        <translation>NDEF-Tag</translation>
    </message>
    <message>
        <source>PACE protected document</source>
        <translation>PACE-geschütztes Dokument</translation>
    </message>
    <message>
        <source>Type 2 tag</source>
        <translation>Type-2-Tag</translation>
    </message>
    <message>
        <source>MIFARE Classic</source>
        <translation>MIFARE Classic</translation>
    </message>
    <message>
        <source>ISO 14443-4 card</source>
        <translation>ISO-14443-4-Karte</translation>
    </message>
    <message>
        <source>Loaded %1</source>
        <translation>%1 geladen</translation>
    </message>
    <message>
        <source>Detection</source>
        <translation>Erkennung</translation>
    </message>
    <message>
        <source>Target</source>
        <translation>Ziel</translation>
    </message>
    <message>
        <source>detected in the field</source>
        <translation>im Feld erkannt</translation>
    </message>
    <message>
        <source>Interface</source>
        <translation>Schnittstelle</translation>
    </message>
    <message>
        <source>none offered by nfcd</source>
        <translation>keine von nfcd angeboten</translation>
    </message>
    <message>
        <source>Card the stack cannot talk to</source>
        <translation>Karte, mit der der Stack nicht reden kann</translation>
    </message>
    <message>
        <source>Security infos</source>
        <translation>Security Infos</translation>
    </message>
    <message>
        <source>EF.CardAccess (raw)</source>
        <translation>EF.CardAccess (roh)</translation>
    </message>
    <message>
        <source>Probing</source>
        <translation>Abfrage</translation>
    </message>
    <message>
        <source>stopped early - the card stopped answering</source>
        <translation>vorzeitig beendet — die Karte antwortet nicht mehr</translation>
    </message>
    <message>
        <source>Unidentified card</source>
        <translation>Nicht identifizierte Karte</translation>
    </message>
    <message>
        <source>%1 (format byte %2)</source>
        <comment>ATS interface byte</comment>
        <translation>%1 (Byte %2)</translation>
    </message>
    <message>
        <source>MBLI</source>
        <translation>MBLI</translation>
    </message>
    <message>
        <source>%1 - card buffer is %2x the frame size</source>
        <translation>%1 — Kartenpuffer ist das %2-fache der Framegröße</translation>
    </message>
    <message>
        <source>0 - no buffer information</source>
        <translation>0 — keine Pufferangabe</translation>
    </message>
    <message>
        <source>DID</source>
        <translation>DID</translation>
    </message>
    <message>
        <source>Ready - pull down to read</source>
        <translation>Bereit — im Pulley-Menü „Lesen“ wählen</translation>
    </message>
    <message>
        <source>Nothing showed up. Pull down to try again.</source>
        <translation>Nichts aufgetaucht. Im Pulley-Menü erneut „Lesen“ wählen.</translation>
    </message>
    <message>
        <source>Brush head (Philips BrushSync)</source>
        <translation>Bürstenkopf (Philips BrushSync)</translation>
    </message>
    <message>
        <source>Wear register</source>
        <translation>Verschleißregister</translation>
    </message>
    <message>
        <source>not in this read</source>
        <translation>in dieser Lesung nicht enthalten</translation>
    </message>
    <message>
        <source>Wear register (page 36)</source>
        <translation>Verschleißregister (Seite 36)</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Nutzung</translation>
    </message>
    <message>
        <source>factory-new - nothing recorded yet</source>
        <translation>fabrikneu — noch nichts aufgezeichnet</translation>
    </message>
    <message>
        <source>Usage counter</source>
        <translation>Nutzungszähler</translation>
    </message>
    <message>
        <source>Estimated brushings</source>
        <translation>Geschätzte Putzvorgänge</translation>
    </message>
    <message>
        <source>about %1 sessions of 2 minutes</source>
        <translation>etwa %1 Sitzungen zu 2 Minuten</translation>
    </message>
    <message>
        <source>Estimated wear</source>
        <translation>Geschätzter Verschleiß</translation>
    </message>
    <message>
        <source>about %1 % of a 3-month life</source>
        <translation>etwa %1 % einer 3-Monats-Lebensdauer</translation>
    </message>
    <message>
        <source>Type / date code</source>
        <translation>Typ-/Datumscode</translation>
    </message>
</context>
<context>
    <name>OtherPanel</name>
    <message>
        <source>Other</source>
        <translation>Sonstige</translation>
    </message>
    <message>
        <source>Unidentified and unreachable cards</source>
        <translation>Nicht identifizierte und nicht erreichbare Karten</translation>
    </message>
    <message>
        <source>Something was in the field, but nfcd offered no interface for it. Nothing was read - there was nothing to read from.</source>
        <translation>Etwas war im Feld, aber nfcd hat keine Schnittstelle dafür angeboten. Es wurde nichts gelesen — es gab nichts zu lesen.</translation>
    </message>
    <message>
        <source>The current read was placed on another panel.</source>
        <translation>Der aktuelle Lesevorgang wurde auf einer anderen Seite abgelegt.</translation>
    </message>
    <message>
        <source>A card in the field but no tag object</source>
        <translation>Eine Karte im Feld, aber kein Tag-Objekt</translation>
    </message>
    <message>
        <source>The reader saw a target - so the card is intact, correctly placed and at the right frequency</source>
        <translation>Der Leser hat ein Ziel gesehen — die Karte ist also intakt, richtig positioniert und auf der richtigen Frequenz</translation>
    </message>
    <message>
        <source>By far the most common cause is MIFARE Classic. Access badges, parking cards, canteen and laundry cards are full of them. The controller could read one, but the SailfishOS stack implements no Crypto1, so nfcd never creates a tag object at all.</source>
        <translation>Mit Abstand häufigste Ursache ist MIFARE Classic. Zutrittsausweise, Parkkarten, Kantinen- und Waschkarten sind voll davon. Der Controller könnte so eine lesen, aber der SailfishOS-Stack implementiert kein Crypto1, also legt nfcd gar kein Tag-Objekt an.</translation>
    </message>
    <message>
        <source>Other candidates: a card that only answers proprietary commands, or one that broke off activation.</source>
        <translation>Weitere Kandidaten: eine Karte, die nur proprietäre Kommandos beantwortet, oder eine, bei der die Aktivierung abgebrochen ist.</translation>
    </message>
    <message>
        <source>Check the Log panel - if a target appeared and vanished without a tag, this is what happened.</source>
        <translation>Sieh im Protokoll nach — wenn dort ein Ziel auftauchte und ohne Tag wieder verschwand, ist genau das passiert.</translation>
    </message>
    <message>
        <source>What would get you further</source>
        <translation>Was weiterhelfen würde</translation>
    </message>
    <message>
        <source>A Proxmark reads and writes MIFARE Classic, recovers keys, and clones onto magic cards - that is exactly the gap this phone leaves open</source>
        <translation>Ein Proxmark liest und schreibt MIFARE Classic, rekonstruiert Schlüssel und klont auf Magic Cards — genau die Lücke, die dieses Telefon offen lässt</translation>
    </message>
    <message>
        <source>If the card also carries an ISO 14443-4 application, this app will still find it: the AID probe runs independently of the MIFARE part</source>
        <translation>Trägt die Karte zusätzlich eine ISO-14443-4-Anwendung, findet diese App sie trotzdem: die AID-Abfrage läuft unabhängig vom MIFARE-Teil</translation>
    </message>
    <message>
        <source>No app on this phone can close that gap. It is missing below the driver, not above it.</source>
        <translation>Keine App auf diesem Telefon kann diese Lücke schließen. Sie fehlt unterhalb des Treibers, nicht oberhalb.</translation>
    </message>
    <message>
        <source>Frequency</source>
        <translation>Frequenz</translation>
    </message>
</context>
<context>
    <name>PanelPulley</name>
    <message>
        <source>Import</source>
        <translation>Importieren</translation>
    </message>
    <message>
        <source>Export</source>
        <translation>Exportieren</translation>
    </message>
    <message>
        <source>Log</source>
        <translation>Protokoll</translation>
    </message>
    <message>
        <source>Comment</source>
        <translation>Kommentar</translation>
    </message>
    <message>
        <source>Stop sending</source>
        <translation>Senden beenden</translation>
    </message>
    <message>
        <source>Send / emulate</source>
        <translation>Senden / Emulieren</translation>
    </message>
    <message>
        <source>Cancel reading</source>
        <translation>Lesen abbrechen</translation>
    </message>
    <message>
        <source>Read</source>
        <translation>Lesen</translation>
    </message>
    <message>
        <source>Save to archive</source>
        <translation>Ins Archiv speichern</translation>
    </message>
</context>
<context>
    <name>PaymentPanel</name>
    <message>
        <source>Payment card</source>
        <translation>Bezahlkarte</translation>
    </message>
    <message>
        <source>EMV contactless</source>
        <translation>EMV kontaktlos</translation>
    </message>
    <message>
        <source>No payment card in the current read.</source>
        <translation>Keine Bezahlkarte im aktuellen Lesevorgang.</translation>
    </message>
    <message>
        <source>What is readable</source>
        <translation>Was lesbar ist</translation>
    </message>
    <message>
        <source>The payment system environment (PPSE) and the list of applications the card offers</source>
        <translation>Die Payment System Environment (PPSE) und die Liste der Anwendungen, die die Karte anbietet</translation>
    </message>
    <message>
        <source>Application labels and priorities, and on many cards the primary account number and expiry date</source>
        <translation>Anwendungsnamen und Prioritäten, bei vielen Karten auch Kartennummer und Ablaufdatum</translation>
    </message>
    <message>
        <source>On some cards, the last transactions</source>
        <translation>Bei manchen Karten die letzten Transaktionen</translation>
    </message>
    <message>
        <source>Anything secret. The keys never leave the card&apos;s secure element.</source>
        <translation>Alles Geheime. Die Schlüssel verlassen das Sicherheitselement der Karte nie.</translation>
    </message>
    <message>
        <source>Why paying with this is impossible</source>
        <translation>Warum Bezahlen damit unmöglich ist</translation>
    </message>
    <message>
        <source>Nothing is copied when a phone pays. Google Pay and Apple Pay are provisioned with a device specific token by the card scheme after the bank authenticated you - the real card number is never on the phone.</source>
        <translation>Beim Bezahlen mit dem Telefon wird nichts kopiert. Google Pay und Apple Pay bekommen vom Kartensystem ein gerätespezifisches Token bereitgestellt, nachdem die Bank dich authentifiziert hat — die echte Kartennummer ist nie auf dem Telefon.</translation>
    </message>
    <message>
        <source>A read cannot be replayed. Every transaction needs a fresh cryptogram that the card computes with a key it will not disclose, over an unpredictable number the terminal supplies.</source>
        <translation>Ein Lesevorgang lässt sich nicht wiedergeben. Jede Transaktion braucht ein frisches Kryptogramm, das die Karte mit einem Schlüssel rechnet, den sie nicht herausgibt, über eine unvorhersehbare Zahl vom Terminal.</translation>
    </message>
    <message>
        <source>Getting a token instead requires a certified secure element and attestation to the scheme. SailfishOS has neither, and no bank provisions to it.</source>
        <translation>Stattdessen ein Token zu bekommen setzt ein zertifiziertes Sicherheitselement und Attestierung gegenüber dem Kartensystem voraus. SailfishOS hat weder das eine noch das andere, und keine Bank stellt dorthin bereit.</translation>
    </message>
    <message>
        <source>Protocol-wise this app could register the Visa AID and answer the terminal. It would get exactly as far as the request for a valid cryptogram - and building a working one would be card fraud, not a hobby project.</source>
        <translation>Protokollseitig könnte diese App die Visa-AID registrieren und dem Terminal antworten. Sie käme genau bis zur Anforderung eines gültigen Kryptogramms — und eines zu bauen wäre Kartenbetrug, kein Bastelprojekt.</translation>
    </message>
    <message>
        <source>Green may still appear above: a 4 byte NFCID1 can be presented by this phone. But that identifier is randomly generated by the card for each session anyway - it is not the card, and it buys you nothing at a terminal.</source>
        <translation>Oben kann trotzdem Grün auftauchen: eine 4-Byte-NFCID1 kann dieses Telefon präsentieren. Diesen Wert erzeugt die Karte ohnehin für jede Sitzung neu — er ist nicht die Karte, und am Terminal bringt er nichts.</translation>
    </message>
    <message>
        <source>Frequency</source>
        <translation>Frequenz</translation>
    </message>
</context>
<context>
    <name>Proxmark</name>
    <message>
        <source>Not connected</source>
        <translation>Nicht verbunden</translation>
    </message>
    <message>
        <source>open(%1) failed: %2 (errno %3)</source>
        <translation>open(%1) fehlgeschlagen: %2 (errno %3)</translation>
    </message>
    <message>
        <source>tcgetattr failed: %1</source>
        <translation>tcgetattr fehlgeschlagen: %1</translation>
    </message>
    <message>
        <source>tcsetattr failed: %1</source>
        <translation>tcsetattr fehlgeschlagen: %1</translation>
    </message>
    <message>
        <source>write failed: %1</source>
        <translation>Schreiben fehlgeschlagen: %1</translation>
    </message>
    <message>
        <source>No %1 - is the Proxmark plugged in?</source>
        <translation>Kein %1 — ist der Proxmark angesteckt?</translation>
    </message>
    <message>
        <source>Port open, probing...</source>
        <translation>Port offen, frage ab …</translation>
    </message>
    <message>
        <source>Port opened but the Proxmark did not answer a PING. It may run older firmware that only speaks the legacy frame format.</source>
        <translation>Port geöffnet, aber der Proxmark hat auf ein PING nicht geantwortet. Möglicherweise läuft ältere Firmware, die nur das alte Frameformat spricht.</translation>
    </message>
    <message>
        <source>Proxmark answered. Reading version...</source>
        <translation>Proxmark hat geantwortet. Lese Version …</translation>
    </message>
    <message>
        <source>Disconnected</source>
        <translation>Getrennt</translation>
    </message>
    <message>
        <source>PM3: %1</source>
        <translation>PM3: %1</translation>
    </message>
    <message>
        <source>Connected</source>
        <translation>Verbunden</translation>
    </message>
    <message>
        <source>Connected (no version reply)</source>
        <translation>Verbunden (keine Versionsantwort)</translation>
    </message>
    <message>
        <source>Searching 125 kHz - hold the tag on the LF antenna...</source>
        <translation>Suche 125 kHz — Tag an die LF-Antenne halten …</translation>
    </message>
    <message>
        <source>Trying %1...</source>
        <translation>Versuche %1 …</translation>
    </message>
    <message>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <source>No 125 kHz badge found (blank tag, or an unsupported format)</source>
        <translation>Kein 125-kHz-Badge gefunden (leerer Tag oder ein nicht unterstütztes Format)</translation>
    </message>
    <message>
        <source>Not a valid hex id: %1</source>
        <translation>Keine gültige Hex-ID: %1</translation>
    </message>
    <message>
        <source>Writing %1 to T5577 - hold the tag on the LF antenna...</source>
        <translation>Schreibe %1 auf T5577 — Tag an die LF-Antenne halten …</translation>
    </message>
    <message>
        <source>Could not start the write</source>
        <translation>Schreiben konnte nicht gestartet werden</translation>
    </message>
    <message>
        <source>Written: T5577 now answers as EM4100 %1</source>
        <translation>Geschrieben: T5577 antwortet jetzt als EM4100 %1</translation>
    </message>
    <message>
        <source>Write rejected by the device (no tag, or not a T5577?)</source>
        <translation>Schreiben vom Gerät abgelehnt (kein Tag, oder kein T5577?)</translation>
    </message>
    <message>
        <source>No reply to the write - was a tag on the antenna?</source>
        <translation>Keine Antwort auf das Schreiben — lag ein Tag auf der Antenne?</translation>
    </message>
    <message>
        <source>Reading 13.56 MHz - hold the card on the HF antenna...</source>
        <translation>Lese 13,56 MHz — Karte an die HF-Antenne halten …</translation>
    </message>
    <message>
        <source>Could not start HF read</source>
        <translation>HF-Lesen konnte nicht gestartet werden</translation>
    </message>
    <message>
        <source>No card in the HF field</source>
        <translation>Keine Karte im HF-Feld</translation>
    </message>
    <message>
        <source>No 13.56 MHz card found</source>
        <translation>Keine 13,56-MHz-Karte gefunden</translation>
    </message>
    <message>
        <source>ATQA %1%2 · SAK %3</source>
        <translation>ATQA %1%2 · SAK %3</translation>
    </message>
    <message>
        <source>Crypto1 - the phone&apos;s own stack cannot read this; the Proxmark can</source>
        <translation>Crypto1 — der Stack des Telefons kann das nicht lesen, der Proxmark schon</translation>
    </message>
    <message>
        <source>%1 · UID %2</source>
        <translation>%1 · UID %2</translation>
    </message>
    <message>
        <source>Checking sector %1/%2...</source>
        <translation>Prüfe Sektor %1/%2 …</translation>
    </message>
    <message>
        <source>Sector %1  A:%2  B:%3</source>
        <translation>Sektor %1  A:%2  B:%3</translation>
    </message>
    <message>
        <source>%1 of %2 sectors open with a default key</source>
        <translation>%1 von %2 Sektoren mit Default-Key offen</translation>
    </message>
    <message>
        <source>Run the key check first</source>
        <translation>Erst den Key-Check ausführen</translation>
    </message>
    <message>
        <source>Dumping sector %1/%2...</source>
        <translation>Dumpe Sektor %1/%2 …</translation>
    </message>
    <message>
        <source>Dumped %1 of %2 blocks</source>
        <translation>%1 von %2 Blöcken gedumpt</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Text (%1)</source>
        <translation>Text (%1)</translation>
    </message>
    <message>
        <source>URI</source>
        <translation>URI</translation>
    </message>
    <message>
        <source>Smart poster</source>
        <translation>Smart Poster</translation>
    </message>
    <message>
        <source>%1 bytes of nested records</source>
        <translation>%1 Byte verschachtelte Records</translation>
    </message>
    <message>
        <source>Contact (vCard)</source>
        <translation>Kontakt (vCard)</translation>
    </message>
    <message>
        <source>Wi-Fi credentials</source>
        <translation>WLAN-Zugangsdaten</translation>
    </message>
    <message>
        <source>%1 bytes, WSC encoded</source>
        <translation>%1 Byte, WSC-kodiert</translation>
    </message>
    <message>
        <source>Absolute URI</source>
        <translation>Absolute URI</translation>
    </message>
    <message>
        <source>unknown protocol</source>
        <translation>unbekanntes Protokoll</translation>
    </message>
    <message>
        <source>max frame %1 bytes (FSCI %2)</source>
        <translation>max. Frame %1 Byte (FSCI %2)</translation>
    </message>
    <message>
        <source>%1 follow</source>
        <translation>%1 folgen</translation>
    </message>
    <message>
        <source>106 kbit/s only</source>
        <translation>nur 106 kbit/s</translation>
    </message>
    <message>
        <source>106 plus %1 kbit/s to card, %2 to reader</source>
        <translation>106 plus %1 kbit/s zur Karte, %2 zum Leser</translation>
    </message>
    <message>
        <source>none</source>
        <translation>keine</translation>
    </message>
    <message>
        <source>same rate required both ways</source>
        <translation>für beide Richtungen dieselbe Rate verlangt</translation>
    </message>
    <message>
        <source>waiting time ~%1 ms (FWI %2) · guard time ~%3 ms (SFGI %4)</source>
        <translation>Wartezeit ~%1 ms (FWI %2) · Schonfrist ~%3 ms (SFGI %4)</translation>
    </message>
    <message>
        <source>CID supported</source>
        <translation>CID unterstützt</translation>
    </message>
    <message>
        <source>no CID</source>
        <translation>kein CID</translation>
    </message>
    <message>
        <source>NAD supported</source>
        <translation>NAD unterstützt</translation>
    </message>
    <message>
        <source>no NAD</source>
        <translation>kein NAD</translation>
    </message>
    <message>
        <source>MIFARE Ultralight / NTAG (Type 2)</source>
        <translation>MIFARE Ultralight / NTAG (Type 2)</translation>
    </message>
    <message>
        <source>MIFARE Classic 1K</source>
        <translation>MIFARE Classic 1K</translation>
    </message>
    <message>
        <source>MIFARE Classic Mini</source>
        <translation>MIFARE Classic Mini</translation>
    </message>
    <message>
        <source>MIFARE Classic 4K</source>
        <translation>MIFARE Classic 4K</translation>
    </message>
    <message>
        <source>Infineon MIFARE Classic 1K</source>
        <translation>Infineon MIFARE Classic 1K</translation>
    </message>
    <message>
        <source>MIFARE Classic + ISO-DEP</source>
        <translation>MIFARE Classic + ISO-DEP</translation>
    </message>
    <message>
        <source>MIFARE Plus 2K (SL2)</source>
        <translation>MIFARE Plus 2K (SL2)</translation>
    </message>
    <message>
        <source>MIFARE Plus 4K (SL2)</source>
        <translation>MIFARE Plus 4K (SL2)</translation>
    </message>
    <message>
        <source>ISO 14443-4 (DESFire, Plus SL3, JCOP…)</source>
        <translation>ISO 14443-4 (DESFire, Plus SL3, JCOP …)</translation>
    </message>
    <message>
        <source>unknown (SAK %1)</source>
        <translation>unbekannt (SAK %1)</translation>
    </message>
</context>
<context>
    <name>ReadPanel</name>
    <message>
        <source>Read</source>
        <translation>Lesen</translation>
    </message>
    <message>
        <source>nfcd %1 · %2</source>
        <translation>nfcd %1 · %2</translation>
    </message>
    <message>
        <source>nfcd not available</source>
        <translation>nfcd nicht verfügbar</translation>
    </message>
    <message>
        <source>The radio stays off until you pull down and choose Read. Nothing is scanned in the background.</source>
        <translation>Der Funk bleibt aus, bis du im Pulley-Menü „Lesen“ wählst. Im Hintergrund wird nichts abgetastet.</translation>
    </message>
    <message>
        <source>Identified as</source>
        <translation>Eingestuft als</translation>
    </message>
    <message>
        <source>Swipe left for the other panels. Each card type has its own page explaining what this phone can and cannot do with it.</source>
        <translation>Nach links wischen für die übrigen Seiten. Jeder Kartentyp hat eine eigene Seite, die erklärt, was dieses Telefon damit kann und was nicht.</translation>
    </message>
    <message>
        <source>Frequency</source>
        <translation>Frequenz</translation>
    </message>
</context>
<context>
    <name>SaveDialog</name>
    <message>
        <source>Archived as “%1”</source>
        <translation>Archiviert als „%1“</translation>
    </message>
    <message>
        <source>Save failed</source>
        <translation>Speichern fehlgeschlagen</translation>
    </message>
    <message>
        <source>Save to archive</source>
        <translation>Ins Archiv speichern</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Speichern</translation>
    </message>
    <message>
        <source>Name in the archive</source>
        <translation>Name im Archiv</translation>
    </message>
    <message>
        <source>How you will recognise it again</source>
        <translation>Woran du sie wiedererkennst</translation>
    </message>
    <message>
        <source>Comment (optional)</source>
        <translation>Kommentar (optional)</translation>
    </message>
    <message>
        <source>Where it came from, what it opens, what you tried</source>
        <translation>Woher sie stammt, was sie öffnet, was du versucht hast</translation>
    </message>
    <message>
        <source>Name and comment are stored beside the card data, never inside it.</source>
        <translation>Name und Kommentar liegen neben den Kartendaten, nie darin.</translation>
    </message>
</context>
<context>
    <name>TransponderPanel</name>
    <message>
        <source>Transponder</source>
        <translation>Transponder</translation>
    </message>
    <message>
        <source>Type 2 tags, ISO 14443-4 cards, tokens</source>
        <translation>Type-2-Tags, ISO-14443-4-Karten, Token</translation>
    </message>
    <message>
        <source>What this phone reaches</source>
        <translation>Was dieses Telefon erreicht</translation>
    </message>
    <message>
        <source>NTAG213/215/216 and MIFARE Ultralight: full memory dump, and writing when the tag allows it</source>
        <translation>NTAG213/215/216 und MIFARE Ultralight: vollständiger Speicherabzug, und Schreiben, wenn der Tag es zulässt</translation>
    </message>
    <message>
        <source>Any ISO 14443-4 card via free APDUs - DESFire with the right keys, security tokens, transit cards</source>
        <translation>Jede ISO-14443-4-Karte über freie APDUs — DESFire mit den passenden Schlüsseln, Sicherheitstoken, Verkehrskarten</translation>
    </message>
    <message>
        <source>UID, ATQA, SAK, ATS and the historical bytes of everything that answers</source>
        <translation>UID, ATQA, SAK, ATS und die Historical Bytes von allem, was antwortet</translation>
    </message>
    <message>
        <source>MIFARE Classic. The chip could do it, but the SailfishOS stack implements no Crypto1, so nfcd never offers a usable interface. This is Proxmark territory.</source>
        <translation>MIFARE Classic. Der Chip könnte es, aber der SailfishOS-Stack implementiert kein Crypto1, also bietet nfcd nie eine nutzbare Schnittstelle an. Das ist Proxmark-Gebiet.</translation>
    </message>
    <message>
        <source>125 kHz altogether - EM4100, HID Prox, T5577. Different frequency, different antenna, not present in any phone.</source>
        <translation>125 kHz insgesamt — EM4100, HID Prox, T5577. Andere Frequenz, andere Antenne, in keinem Telefon vorhanden.</translation>
    </message>
    <message>
        <source>Sniffing the conversation between someone else&apos;s card and reader.</source>
        <translation>Das Gespräch zwischen fremder Karte und fremdem Leser mithören.</translation>
    </message>
    <message>
        <source>FeliCa: this device polls NFC-A and NFC-B only.</source>
        <translation>FeliCa: dieses Gerät sucht nur nach NFC-A und NFC-B.</translation>
    </message>
    <message>
        <source>Cloning a tag</source>
        <translation>Einen Tag klonen</translation>
    </message>
    <message>
        <source>If the content is NDEF, it can be captured here and served again from the Emulate panel</source>
        <translation>Ist der Inhalt NDEF, lässt er sich hier aufzeichnen und von der Seite „Emulieren“ wieder ausgeben</translation>
    </message>
    <message>
        <source>A Type 2 tag cannot be impersonated. Host card emulation always presents an ISO-DEP smart card, never an NTAG - a reader expecting Type 2 commands simply finds no matching target.</source>
        <translation>Ein Type-2-Tag lässt sich nicht vortäuschen. Host Card Emulation präsentiert immer eine ISO-DEP-Smartcard, nie ein NTAG — ein Leser, der Type-2-Kommandos erwartet, findet schlicht kein passendes Ziel.</translation>
    </message>
    <message>
        <source>Readers that only check the UID of an access badge therefore cannot be satisfied by this phone, even though the NFCID1 itself is configurable.</source>
        <translation>Leser, die nur die UID eines Zutrittsausweises prüfen, lassen sich von diesem Telefon deshalb nicht zufriedenstellen, obwohl die NFCID1 selbst einstellbar ist.</translation>
    </message>
    <message>
        <source>Frequency</source>
        <translation>Frequenz</translation>
    </message>
</context>
<context>
    <name>UltimatePanel</name>
    <message>
        <source>Log</source>
        <translation>Protokoll</translation>
    </message>
    <message>
        <source>Disconnect</source>
        <translation>Trennen</translation>
    </message>
    <message>
        <source>Connect</source>
        <translation>Verbinden</translation>
    </message>
    <message>
        <source>Ultimate Mode</source>
        <translation>Ultimate Mode</translation>
    </message>
    <message>
        <source>Proxmark · external hardware</source>
        <translation>Proxmark · externe Hardware</translation>
    </message>
    <message>
        <source>Beyond this point nothing runs on the phone alone. This is where a Proxmark reads the frequencies and cards the phone cannot: 125 kHz, MIFARE Classic, and more.</source>
        <translation>Ab hier läuft nichts mehr allein auf dem Telefon. Hier liest ein Proxmark die Frequenzen und Karten, die das Telefon nicht kann: 125 kHz, MIFARE Classic und mehr.</translation>
    </message>
    <message>
        <source>Device</source>
        <translation>Gerät</translation>
    </message>
    <message>
        <source>Port</source>
        <translation>Port</translation>
    </message>
    <message>
        <source> · present</source>
        <translation> · vorhanden</translation>
    </message>
    <message>
        <source> · not found</source>
        <translation> · nicht gefunden</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Status</translation>
    </message>
    <message>
        <source>Firmware</source>
        <translation>Firmware</translation>
    </message>
    <message>
        <source>What this unlocks</source>
        <translation>Was das freischaltet</translation>
    </message>
    <message>
        <source>125 kHz LF: EM4100, HID Prox, T5577 - a whole band the phone has no antenna for</source>
        <translation>125 kHz LF: EM4100, HID Prox, T5577 — ein ganzes Band, für das das Telefon keine Antenne hat</translation>
    </message>
    <message>
        <source>MIFARE Classic with Crypto1: the access badges and parking cards that land on the Other panel</source>
        <translation>MIFARE Classic mit Crypto1: die Zutrittsausweise und Parkkarten, die auf der Seite „Sonstige“ landen</translation>
    </message>
    <message>
        <source>Sniffing, and cloning onto magic cards</source>
        <translation>Sniffing und Klonen auf Magic Cards</translation>
    </message>
    <message>
        <source>First the pipe has to be proven: connect, and the firmware version should appear above. If the port opens but nothing answers, the log will say so.</source>
        <translation>Zuerst muss die Verbindung stehen: verbinden, dann sollte oben die Firmware-Version erscheinen. Öffnet der Port, antwortet aber nichts, sagt es das Protokoll.</translation>
    </message>
    <message>
        <source>Reading LF and Classic is the next step, once the connection is confirmed against your device.</source>
        <translation>LF und Classic zu lesen ist der nächste Schritt, sobald die Verbindung mit deinem Gerät bestätigt ist.</translation>
    </message>
    <message>
        <source>125 kHz (LF)</source>
        <translation>125 kHz (LF)</translation>
    </message>
    <message>
        <source>Proxmark · 125 kHz</source>
        <translation>Proxmark · 125 kHz</translation>
    </message>
    <message>
        <source>Tap a device to enter</source>
        <translation>Zum Öffnen ein Gerät antippen</translation>
    </message>
    <message>
        <source>PM3 Easy</source>
        <translation>PM3 Easy</translation>
    </message>
    <message>
        <source>Proxmark 5</source>
        <translation>Proxmark 5</translation>
    </message>
    <message>
        <source> (125 kHz)</source>
        <translation> (125 kHz)</translation>
    </message>
    <message>
        <source> ID</source>
        <translation> ID</translation>
    </message>
    <message>
        <source>Search 125 kHz badge</source>
        <translation>125-kHz-Badge suchen</translation>
    </message>
    <message>
        <source>Tries EM4100, HID Prox and IO Prox in turn - the common access-badge formats.</source>
        <translation>Probiert nacheinander EM4100, HID Prox und IO Prox — die gängigen Zutrittskarten-Formate.</translation>
    </message>
    <message>
        <source>Write to T5577</source>
        <translation>Auf T5577 schreiben</translation>
    </message>
    <message>
        <source>Clone %1 to T5577</source>
        <translation>%1 auf T5577 klonen</translation>
    </message>
    <message>
        <source>Writing %1…</source>
        <translation>Schreibe %1 …</translation>
    </message>
    <message>
        <source>Write test sheep “Dolly” (446F6C6C79)</source>
        <translation>Testschaf „Dolly“ schreiben (446F6C6C79)</translation>
    </message>
    <message>
        <source>Writing Dolly…</source>
        <translation>Schreibe Dolly …</translation>
    </message>
    <message>
        <source>Writing permanently reconfigures the tag, but a T5577 can be rewritten any number of times. After writing, search again to read it back.</source>
        <translation>Schreiben konfiguriert den Tag dauerhaft um, aber ein T5577 lässt sich beliebig oft neu beschreiben. Nach dem Schreiben erneut suchen, um ihn zurückzulesen.</translation>
    </message>
    <message>
        <source>13.56 MHz (HF)</source>
        <translation>13,56 MHz (HF)</translation>
    </message>
    <message>
        <source>UID %1
%2</source>
        <translation>UID %1
%2</translation>
    </message>
    <message>
        <source>Read 13.56 MHz card</source>
        <translation>13,56-MHz-Karte lesen</translation>
    </message>
    <message>
        <source>Reads UID, ATQA and SAK. The SAK says whether it is MIFARE Classic - the type the phone itself cannot read.</source>
        <translation>Liest UID, ATQA und SAK. Der SAK verrät, ob es MIFARE Classic ist — der Typ, den das Telefon selbst nicht lesen kann.</translation>
    </message>
    <message>
        <source> (13.56 MHz)</source>
        <translation> (13,56 MHz)</translation>
    </message>
    <message>
        <source>Proxmark · 13.56 MHz</source>
        <translation>Proxmark · 13,56 MHz</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>UID</source>
        <translation>UID</translation>
    </message>
    <message>
        <source>ATQA / SAK</source>
        <translation>ATQA / SAK</translation>
    </message>
    <message>
        <source>Save HF card to archive</source>
        <translation>HF-Karte ins Archiv speichern</translation>
    </message>
    <message>
        <source>Save LF tag to archive</source>
        <translation>LF-Tag ins Archiv speichern</translation>
    </message>
    <message>
        <source>Check MIFARE Classic default keys</source>
        <translation>MIFARE-Classic-Default-Keys prüfen</translation>
    </message>
    <message>
        <source>%1 sector(s) open with a default key:</source>
        <translation>%1 Sektor(en) mit Default-Key offen:</translation>
    </message>
    <message>
        <source>MIFARE Classic keys</source>
        <translation>MIFARE-Classic-Schlüssel</translation>
    </message>
    <message>
        <source>Sector keys</source>
        <translation>Sektor-Schlüssel</translation>
    </message>
    <message>
        <source>MIFARE Classic dump</source>
        <translation>MIFARE-Classic-Dump</translation>
    </message>
    <message>
        <source>%1 blocks</source>
        <translation>%1 Blöcke</translation>
    </message>
    <message>
        <source>Dump sectors</source>
        <translation>Sektoren dumpen</translation>
    </message>
    <message>
        <source>%1 of 64 blocks read - pull down to save it</source>
        <translation>%1 von 64 Blöcken gelesen — im Pulley speichern</translation>
    </message>
</context>
</TS>
