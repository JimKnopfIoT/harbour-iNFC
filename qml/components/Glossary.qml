import QtQuick 2.0
import Sailfish.Silica 1.0

/*
 * Reference for every raw value this app puts on screen, folded away in
 * expanding sections at the end of each card type panel.
 *
 * The rule behind it: wherever a raw value is shown, its reading belongs
 * within reach. Not in the way - within reach.
 */
Column {
    width: parent ? parent.width : 0
    spacing: 0

    SectionHeader { text: qsTr("Reference") }

    ExpandingSectionGroup {
        width: parent.width

        // ------------------------------------------------------------------
        ExpandingSection {
            title: qsTr("Technology and protocol")
            content.sourceComponent: Component {
                TermList {
                    intro: qsTr("Two separate questions. The technology is the radio layer - how bits are put on the 13.56 MHz carrier. The protocol is what runs on top of it once the card is activated.")
                    outro: qsTr("So \"Type 4A (ISO-DEP)\" means: activated over the Type A air interface, and a full smart card underneath. That combination is what makes selecting applications by AID possible at all - a Type 2 tag has no such notion, it is just memory.")
                    terms: [
                        { term: "NFC-A", text: qsTr("ISO/IEC 14443 Type A. The common case: NTAG, MIFARE, most bank cards, most phones doing card emulation.") },
                        { term: "NFC-B", text: qsTr("ISO/IEC 14443 Type B. Different modulation and a different anticollision scheme, same 13.56 MHz. German identity cards and a good share of payment cards are Type B.") },
                        { term: "NFC-F", text: qsTr("FeliCa, mostly Japan - transit and payment. This phone does not poll for it at all: the controller reports NFC-A and NFC-B only.") },
                        { term: "Type 1", text: qsTr("Topaz/Jewel. Simple memory, rare today.") },
                        { term: "Type 2", text: qsTr("NTAG21x, MIFARE Ultralight. Memory in 4 byte blocks, read and written directly. No applications, no APDUs - the whole tag is one flat address space.") },
                        { term: "Type 3", text: qsTr("FeliCa based. Not reachable here, see NFC-F.") },
                        { term: "Type 4A", text: qsTr("A full smart card, spoken to with APDUs over ISO-DEP (ISO/IEC 14443-4), activated over the Type A interface. It holds applications selected by identifier, each with its own files and access rules. Bank cards, DESFire, identity documents, security tokens.") },
                        { term: "Type 4B", text: qsTr("The same smart card layer, activated over the Type B interface instead. Everything above the air interface is identical.") },
                        { term: "NFC-DEP", text: qsTr("Peer to peer, ISO 18092. Two devices talking as equals rather than card and reader. Effectively dead since Android dropped it.") }
                    ]
                }
            }
        }

        // ------------------------------------------------------------------
        ExpandingSection {
            title: qsTr("Anticollision: NFCID1 and SAK")
            content.sourceComponent: Component {
                TermList {
                    intro: qsTr("What a card reveals before any application is selected. This is the only information available for cards the stack cannot otherwise reach.")
                    outro: qsTr("SAK is the fastest way to tell what you are holding: bit 0x20 means ISO 14443-4 and an ATS follows, 0x08 and 0x18 mean MIFARE Classic, 0x00 means a Type 2 tag.")
                    terms: [
                        { term: "NFCID1", text: qsTr("The identifier used during anticollision, 4, 7 or 10 bytes. 4 bytes is a single size UID, 7 a double, 10 a triple. A 4 byte value starting with 08 is not an identity at all - it is randomly generated for this session, which passports, identity cards and payment cards all do on purpose.") },
                        { term: "SAK", text: qsTr("Select Acknowledge, also called SEL_RES: one byte, the card's answer to being singled out. Bit 0x20 says it speaks ISO 14443-4 and an ATS follows; bit 0x40 says it speaks NFC-DEP; bit 0x04 says the identifier is not complete and another cascade level follows. The rest is manufacturer specific.") },
                        { term: "00", text: qsTr("Type 2 tag - MIFARE Ultralight, NTAG213/215/216. Read and write works here.") },
                        { term: "08 / 09", text: qsTr("MIFARE Classic 1K / Mini. Not reachable from SailfishOS - see the Other panel.") },
                        { term: "18", text: qsTr("MIFARE Classic 4K. Same story.") },
                        { term: "20", text: qsTr("Plain ISO 14443-4. DESFire, bank cards, identity documents, another phone doing card emulation.") },
                        { term: "28 / 38", text: qsTr("Both at once: a Classic part plus an ISO 14443-4 part. This app reaches the second half and not the first.") },
                        { term: "88", text: qsTr("Infineon SLE, Classic compatible. Same limitation.") }
                    ]
                }
            }
        }

        // ------------------------------------------------------------------
        ExpandingSection {
            title: qsTr("Reading the ATS")
            content.sourceComponent: Component {
                TermList {
                    intro: qsTr("These four bytes come from the Answer To Select, with which an ISO 14443-4 card introduces itself after activation. They say nothing about what is on the card - only how one is allowed to talk to it.")
                    outro: qsTr("In short: T0 says how large, TA how fast, TB how long, TC which addressing. FSCI and FWI are the two that matter in practice - a card with a small frame and a long waiting time is the one that makes probing slow.")
                    terms: [
                        { term: "T0", text: qsTr("Format byte. The lower four bits are the FSCI, the largest frame the card accepts (0 → 16 bytes … 8 → 256). The upper bits say which of the following bytes are present at all.") },
                        { term: "TA(1)", text: qsTr("Transmission rates. Which multiples of 106 kbit/s the card can do - 212, 424, 848 - separately per direction, plus one bit demanding the same rate both ways.") },
                        { term: "TB(1)", text: qsTr("Timing. The upper nibble FWI sets how long the card may take to answer (FWT ≈ 302 µs × 2^FWI), the lower nibble SFGI the grace period after activation before the first command may arrive.") },
                        { term: "TC(1)", text: qsTr("Protocol options, two bits only: whether the card supports CID (card identifier, several cards in the field) and NAD (node addressing).") }
                    ]
                }
            }
        }

        // ------------------------------------------------------------------
        ExpandingSection {
            title: qsTr("NFC-B parameters")
            content.sourceComponent: Component {
                TermList {
                    intro: qsTr("Type B cards answer with a different set. German identity cards and many payment cards are Type B.")
                    terms: [
                        { term: "NFCID0", text: qsTr("The Type B identifier, four bytes, taken from the answer to the request command. Usually random per session, for the same privacy reason as a random NFCID1.") },
                        { term: "APPDATA", text: qsTr("Application data from the same answer: which application families the card carries and how many there are. Coarse, but free.") },
                        { term: "PROTINFO", text: qsTr("Protocol info: maximum frame size, supported bit rates, and the waiting time - the Type B equivalent of T0, TA and TB rolled into three bytes.") },
                        { term: "MBLI", text: qsTr("Maximum Buffer Length Index, sent at activation. Says how much the card can buffer beyond a single frame, as a power of two.") }
                    ]
                }
            }
        }

        // ------------------------------------------------------------------
        ExpandingSection {
            title: qsTr("Status words")
            content.sourceComponent: Component {
                TermList {
                    intro: qsTr("Every APDU answer ends in two bytes, SW1 and SW2. A card that refuses is not a card that is absent - which is why the application probe reports the two differently.")
                    outro: qsTr("6A82 means the application is genuinely not there. 6982 means it is there and will not talk to you - that is what a German eID says to any terminal without an authorisation certificate.")
                    terms: [
                        { term: "9000", text: qsTr("Success. Nothing further to say.") },
                        { term: "6A82", text: qsTr("File or application not found. The probe treats this as absent.") },
                        { term: "6982", text: qsTr("Security status not satisfied. Present, but authentication is missing.") },
                        { term: "6985", text: qsTr("Conditions of use not satisfied - right command, wrong moment.") },
                        { term: "6283", text: qsTr("Selected file is blocked or invalidated.") },
                        { term: "6300", text: qsTr("Verification failed. With a PIN, the retry counter has just gone down.") },
                        { term: "6700", text: qsTr("Wrong length - Lc or Le did not fit the command.") },
                        { term: "6A86", text: qsTr("Wrong P1/P2 parameters.") },
                        { term: "6B00", text: qsTr("Offset outside the file.") },
                        { term: "6E00", text: qsTr("Class not supported: the card does not know this CLA at all.") },
                        { term: "61xx", text: qsTr("Success, and xx more bytes are waiting - fetch them with GET RESPONSE.") },
                        { term: "6Cxx", text: qsTr("Wrong Le; repeat the command with xx as the expected length.") }
                    ]
                }
            }
        }
    }
}
