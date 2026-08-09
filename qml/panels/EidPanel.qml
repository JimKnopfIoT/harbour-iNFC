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
            title: qsTr("Identity documents")
            description: qsTr("eID, ePassport, PACE protected cards")
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
            model: (Nfc.detailKind === "eid" || Nfc.detailKind === "epass" || Nfc.detailKind === "pace")
                ? Nfc.detailFields : []
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
            visible: !(Nfc.detailKind === "eid" || Nfc.detailKind === "epass" || Nfc.detailKind === "pace")
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.Wrap
            text: qsTr("No identity document in the current read.")
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeSmall
        }

        InfoBlock {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            title: qsTr("German identity card")
            works: [
                qsTr("Detecting the card and reading EF.CardAccess without any secret"),
                qsTr("Establishing a PACE channel with the CAN, the MRZ or the PIN"),
                qsTr("Using this phone as a card reader for software that already has the authorisation - the vpcd relay is planned for the next version")
            ]
            blocked: [
                qsTr("Reading any personal data. The eID application only releases fields after Terminal Authentication, where the chip verifies an authorisation certificate issued by the Federal Office of Administration under §21b PAuswG - and then only the fields that certificate names."),
                qsTr("The ePassport application (photo, fingerprints) is reserved for sovereign inspection systems under EAC."),
                qsTr("There is no stable UID to recognise the card by: the NFCID1 is randomised on every activation, on purpose.")
            ]
            footnote: qsTr("This is not a SailfishOS limitation. No self-written app on any platform can read an eID without a certificate and an eID server.")
        }

        InfoBlock {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            title: qsTr("Travel documents")
            works: [
                qsTr("Passports follow ICAO 9303: the keys are derived from the MRZ or the CAN, which you can read off the document yourself"),
                qsTr("With those, DG1 (the MRZ data) and DG2 (the face image) are readable - the classic hobby project")
            ]
            blocked: [
                qsTr("DG3 and DG4, fingerprints and iris, need Extended Access Control with a certificate"),
                qsTr("The identity card's own ePassport application is restricted the same way")
            ]
        }

        Glossary {
            x: Theme.horizontalPageMargin
            width: column.width - 2 * Theme.horizontalPageMargin
        }
    }

    VerticalScrollDecorator { }
}
