import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

SilicaFlickable {
    id: panel

    property var carousel

    flickableDirection: Flickable.VerticalFlick
    contentHeight: column.height + Theme.paddingLarge * 3

    PanelPulley {
        carousel: panel.carousel
        canEmulate: Nfc.detailRecords.length > 0
        onEmulateRequested: {
            if (Nfc.detailRecords.length > 0) Emulator.setRawMessage(Nfc.detailRecords[0].raw)
        }
    }

    Column {
        id: column
        width: panel.width
        spacing: Theme.paddingMedium

        PageHeader {
            title: qsTr("Transponder")
            description: qsTr("Type 2 tags, ISO 14443-4 cards, tokens")
        }

        Column {
            width: column.width
            spacing: Theme.paddingSmall
            visible: Nfc.detailFrequency.length > 0

            SectionHeader { text: qsTr("Frequency") }

            FrequencyBand {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                active: Nfc.detailFrequency
            }
        }

        Repeater {
            model: Nfc.detailFields
            DataField {
                x: Theme.horizontalPageMargin
                width: column.width - 2 * Theme.horizontalPageMargin
                label: modelData.label
                value: modelData.value
                emittable: modelData.emittable
                mono: modelData.mono
            }
        }

        InfoBlock {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            title: qsTr("What this phone reaches")
            works: [
                qsTr("NTAG213/215/216 and MIFARE Ultralight: full memory dump, and writing when the tag allows it"),
                qsTr("Any ISO 14443-4 card via free APDUs - DESFire with the right keys, security tokens, transit cards"),
                qsTr("UID, ATQA, SAK, ATS and the historical bytes of everything that answers")
            ]
            blocked: [
                qsTr("MIFARE Classic. The chip could do it, but the SailfishOS stack implements no Crypto1, so nfcd never offers a usable interface. This is Proxmark territory."),
                qsTr("125 kHz altogether - EM4100, HID Prox, T5577. Different frequency, different antenna, not present in any phone."),
                qsTr("Sniffing the conversation between someone else's card and reader."),
                qsTr("FeliCa: this device polls NFC-A and NFC-B only.")
            ]
        }

        InfoBlock {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            title: qsTr("Cloning a tag")
            works: [
                qsTr("If the content is NDEF, it can be captured here and served again from the Emulate panel")
            ]
            blocked: [
                qsTr("A Type 2 tag cannot be impersonated. Host card emulation always presents an ISO-DEP smart card, never an NTAG - a reader expecting Type 2 commands simply finds no matching target."),
                qsTr("Readers that only check the UID of an access badge therefore cannot be satisfied by this phone, even though the NFCID1 itself is configurable.")
            ]
        }

        Glossary {
            x: Theme.horizontalPageMargin
            width: column.width - 2 * Theme.horizontalPageMargin
        }
    }

    VerticalScrollDecorator { }
}
