import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

SilicaFlickable {
    id: panel

    property var carousel

    flickableDirection: Flickable.VerticalFlick
    contentHeight: column.height + Theme.paddingLarge * 3

    PanelPulley { carousel: panel.carousel }

    Column {
        id: column
        width: panel.width
        spacing: Theme.paddingMedium

        PageHeader {
            title: qsTr("Health card")
            description: qsTr("eGK generation G2.1")
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
            model: Nfc.detailKind === "egk" ? Nfc.detailFields : []
            DataField {
                x: Theme.horizontalPageMargin
                width: column.width - 2 * Theme.horizontalPageMargin
                label: modelData.label
                value: modelData.value
                emittable: modelData.emittable
                mono: modelData.mono
            }
        }

        Label {
            visible: Nfc.detailKind !== "egk"
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.Wrap
            text: qsTr("No health card in the current read. The reference below applies whenever one is found.")
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeSmall
        }

        InfoBlock {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            title: qsTr("What an eGK is for")
            works: [
                qsTr("Detecting the card and its applications over NFC - every G2.1 card issued since January 2021 has a contactless antenna"),
                qsTr("Reading EF.CardAccess, which is free by design: it is how a terminal learns the PACE parameters"),
                qsTr("Opening a PACE channel with the six digit CAN printed on the front - the protocol is public and OpenSSL has the brainpool curves")
            ]
            blocked: [
                qsTr("Reading the insurance master data (VSD). That needs card-to-card authentication against a health professional card or an SMC-B, which no citizen device has."),
                qsTr("Emergency data and the medication plan - same lock, same reason."),
                qsTr("Acting as a full e-prescription client: the flow itself is specified in public and gematik's reference code is open, but the central IDP only issues tokens to registered clients.")
            ]
            footnote: qsTr("Sources: gematik eGK specification, gemSpec_IDP_Frontend, gematik/api-erp")
        }

        InfoBlock {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            title: qsTr("The prescription is not on the card")
            works: [
                qsTr("The eGK is a key, not a container. Prescriptions live on the e-prescription server, the Fachdienst."),
                qsTr("The pharmacy reads the card only to prove you authorised it to fetch your open prescriptions.")
            ]
            blocked: [
                qsTr("Copying a prescription off the card - there is nothing there to copy, on any device, with any software.")
            ]
            footnote: qsTr("This is stated identically by the Federal Ministry of Health, gematik and ABDA.")
        }

        Glossary {
            x: Theme.horizontalPageMargin
            width: column.width - 2 * Theme.horizontalPageMargin
        }
    }

    VerticalScrollDecorator { }
}
