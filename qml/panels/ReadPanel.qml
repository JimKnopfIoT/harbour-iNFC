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
        canEmulate: Nfc.records.length > 0
        onEmulateRequested: {
            if (Nfc.records.length > 0) {
                Emulator.setRawMessage(Nfc.records[0].raw)
            }
        }
    }

    Column {
        id: column
        width: panel.width
        spacing: Theme.paddingMedium

        PageHeader {
            title: qsTr("Read")
            description: Nfc.daemonPresent
                ? qsTr("nfcd %1 · %2").arg(Nfc.daemonVersion).arg(Nfc.supportedTechs)
                : qsTr("nfcd not available")
        }

        // ---------------------------------------------------------------
        // Status. The radio is off until you ask for it, and this says so.
        // ---------------------------------------------------------------
        Item {
            width: parent.width
            height: statusCol.height + Theme.paddingLarge

            Column {
                id: statusCol
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                spacing: Theme.paddingSmall

                Label {
                    width: parent.width
                    wrapMode: Text.Wrap
                    text: Nfc.statusText
                    color: Nfc.state === 4 ? Theme.errorColor
                        : (Nfc.state === 1 ? Theme.highlightColor : Theme.primaryColor)
                    font.pixelSize: Theme.fontSizeSmall
                }

                BusyIndicator {
                    running: Nfc.state === 1 || Nfc.state === 2
                    size: BusyIndicatorSize.Small
                    visible: running
                }

                Label {
                    width: parent.width
                    visible: Nfc.state === 0
                    wrapMode: Text.Wrap
                    text: qsTr("The radio stays off until you pull down and choose Read. Nothing is scanned in the background.")
                    color: Theme.secondaryColor
                    font.pixelSize: Theme.fontSizeExtraSmall
                }
            }
        }

        // ---------------------------------------------------------------
        // Classification button - the jump into the panel that explains
        // what we are holding.
        // ---------------------------------------------------------------
        Item {
            width: parent.width
            height: Nfc.hasResult ? kindButton.height + Theme.paddingLarge : 0
            visible: Nfc.hasResult

            BackgroundItem {
                id: kindButton
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                height: Theme.itemSizeMedium
                onClicked: panel.carousel.currentIndex = Nfc.kindPanel

                Rectangle {
                    anchors.fill: parent
                    radius: Theme.paddingSmall
                    color: kindButton.highlighted
                        ? Theme.rgba(Theme.highlightBackgroundColor, 0.3)
                        : Theme.rgba(Theme.highlightBackgroundColor, 0.15)
                    border.width: 1
                    border.color: Theme.rgba(Theme.highlightColor, 0.4)
                }

                Column {
                    anchors {
                        left: parent.left
                        right: arrow.left
                        verticalCenter: parent.verticalCenter
                        leftMargin: Theme.paddingLarge
                    }
                    Label {
                        text: qsTr("Identified as")
                        color: Theme.secondaryHighlightColor
                        font.pixelSize: Theme.fontSizeExtraSmall
                    }
                    Label {
                        text: Nfc.kindLabel
                        color: Theme.highlightColor
                        font.pixelSize: Theme.fontSizeMedium
                    }
                }

                Label {
                    id: arrow
                    anchors {
                        right: parent.right
                        rightMargin: Theme.paddingLarge
                        verticalCenter: parent.verticalCenter
                    }
                    text: "→"
                    color: Theme.highlightColor
                    font.pixelSize: Theme.fontSizeLarge
                }
            }
        }

        // ---------------------------------------------------------------
        // Which band this read lives on, and which it does not.
        // ---------------------------------------------------------------
        Column {
            width: column.width
            spacing: Theme.paddingSmall
            visible: Nfc.hasResult

            SectionHeader { text: qsTr("Frequency") }

            FrequencyBand {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                active: Nfc.frequency
            }
        }

        // ---------------------------------------------------------------
        // Everything we got off the tag, grouped as it was collected.
        // ---------------------------------------------------------------
        Repeater {
            model: Nfc.fields

            Column {
                width: column.width
                spacing: Theme.paddingSmall

                SectionHeader {
                    text: modelData.group
                    visible: index === 0
                        || Nfc.fields[index - 1].group !== modelData.group
                    height: visible ? implicitHeight : 0
                }

                DataField {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    label: modelData.label
                    value: modelData.value
                    emittable: modelData.emittable
                    mono: modelData.mono
                }
            }
        }

        Label {
            visible: !Nfc.hasResult && Nfc.state !== 1
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.Wrap
            text: qsTr("Swipe left for the other panels. Each card type has its own page explaining what this phone can and cannot do with it.")
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeExtraSmall
        }
    }

    VerticalScrollDecorator { }
}
