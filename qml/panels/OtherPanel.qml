import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

/*
 * Where everything lands that the stack could not place: unidentified cards,
 * and the ones nfcd refuses to hand us at all. The second case is the more
 * interesting one, because it is not a failure - it is a boundary.
 */
SilicaFlickable {
    id: panel

    property var carousel
    readonly property bool unsupported: Nfc.detailKind === "unsupported"
    readonly property bool mine: unsupported || Nfc.detailKind === "unknown"

    flickableDirection: Flickable.VerticalFlick
    contentHeight: column.height + Theme.paddingLarge * 3

    PanelPulley { carousel: panel.carousel }

    Column {
        id: column
        width: panel.width
        spacing: Theme.paddingMedium

        PageHeader {
            title: qsTr("Other")
            description: qsTr("Unidentified and unreachable cards")
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

        Label {
            visible: panel.unsupported
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.Wrap
            text: qsTr("Something was in the field, but nfcd offered no interface for it. Nothing was read - there was nothing to read from.")
            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeSmall
        }

        Repeater {
            model: panel.mine ? Nfc.detailFields : []
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
            visible: !panel.mine
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.Wrap
            text: qsTr("The current read was placed on another panel.")
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeSmall
        }

        InfoBlock {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            title: qsTr("A card in the field but no tag object")
            works: [
                qsTr("The reader saw a target - so the card is intact, correctly placed and at the right frequency")
            ]
            blocked: [
                qsTr("By far the most common cause is MIFARE Classic. Access badges, parking cards, canteen and laundry cards are full of them. The controller could read one, but the SailfishOS stack implements no Crypto1, so nfcd never creates a tag object at all."),
                qsTr("Other candidates: a card that only answers proprietary commands, or one that broke off activation.")
            ]
            footnote: qsTr("Check the Log panel - if a target appeared and vanished without a tag, this is what happened.")
        }

        InfoBlock {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            title: qsTr("What would get you further")
            works: [
                qsTr("A Proxmark reads and writes MIFARE Classic, recovers keys, and clones onto magic cards - that is exactly the gap this phone leaves open"),
                qsTr("If the card also carries an ISO 14443-4 application, this app will still find it: the AID probe runs independently of the MIFARE part")
            ]
            blocked: [
                qsTr("No app on this phone can close that gap. It is missing below the driver, not above it.")
            ]
        }

        Glossary {
            x: Theme.horizontalPageMargin
            width: column.width - 2 * Theme.horizontalPageMargin
        }
    }

    VerticalScrollDecorator { }
}
