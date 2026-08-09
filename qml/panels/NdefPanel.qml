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
        canEmulate: Nfc.detailRecords.length > 0 || composer.text.length > 0
        onEmulateRequested: {
            if (composer.text.length > 0) {
                uriMode.checked ? Emulator.setUri(composer.text)
                                : Emulator.setText(composer.text)
            } else if (Nfc.detailRecords.length > 0) {
                Emulator.setRawMessage(Nfc.detailRecords[0].raw)
            }
        }
    }

    Column {
        id: column
        width: panel.width
        spacing: Theme.paddingMedium

        PageHeader {
            title: qsTr("NDEF")
            description: qsTr("Records read, written and sent")
        }

        // Frequency and the full read, same as every other detail panel.
        // An NDEF tag is still a tag: its transport, memory and any product
        // decode (a brush head's wear, for one) belong here too, not only
        // the records.
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

            Column {
                width: column.width
                spacing: Theme.paddingSmall

                SectionHeader {
                    text: modelData.group
                    visible: index === 0
                        || Nfc.detailFields[index - 1].group !== modelData.group
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

        Repeater {
            model: Nfc.detailRecords

            Column {
                width: column.width
                spacing: Theme.paddingSmall

                SectionHeader {
                    text: qsTr("Record %1 · %2").arg(index + 1).arg(modelData.title)
                }

                DataField {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    label: qsTr("Type name format")
                    value: modelData.tnfName + " · " + modelData.type
                }

                DataField {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    label: qsTr("Content")
                    value: modelData.text
                    // NDEF content is the one thing this phone can re-emit.
                    emittable: true
                }

                DataField {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    label: qsTr("Payload")
                    value: modelData.raw
                    mono: true
                }
            }
        }

        Label {
            visible: Nfc.detailRecords.length === 0
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.Wrap
            text: qsTr("No NDEF records in the current read.")
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeSmall
        }

        SectionHeader { text: qsTr("Compose") }

        TextField {
            id: composer
            width: parent.width
            placeholderText: uriMode.checked ? qsTr("https://example.org")
                                             : qsTr("Text to store")
            label: uriMode.checked ? qsTr("URI record") : qsTr("Text record")
            EnterKey.iconSource: "image://theme/icon-m-enter-close"
            EnterKey.onClicked: focus = false
        }

        TextSwitch {
            id: uriMode
            text: qsTr("Treat as URI")
            description: qsTr("Off means a plain text record")
        }

        Row {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            spacing: Theme.paddingMedium

            Button {
                text: qsTr("Write to tag")
                enabled: composer.text.length > 0 && Nfc.state !== 1
                onClicked: {
                    uriMode.checked ? Nfc.armWriteUri(composer.text)
                                    : Nfc.armWriteText(composer.text)
                }
            }

            Button {
                text: qsTr("Send it")
                enabled: composer.text.length > 0 && Nfc.cardEmulationSupported
                onClicked: {
                    uriMode.checked ? Emulator.setUri(composer.text)
                                    : Emulator.setText(composer.text)
                    panel.carousel.currentIndex = 7
                }
            }
        }

        InfoBlock {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            title: qsTr("NDEF on this phone")
            works: [
                qsTr("Reading records from Type 2 and Type 4 tags"),
                qsTr("Writing text and URI records to writeable Type 2 tags (NTAG21x, Ultralight)"),
                qsTr("Sending a record as an emulated Type 4A tag, so another phone or a reader can pick it up")
            ]
            blocked: [
                qsTr("Writing to Type 4 cards - that needs the card's own write access conditions, which consumer cards do not grant"),
                qsTr("Locking or setting a password on a tag: nfcd exposes no lock bits"),
                qsTr("Anything above 64 kB, the Type 4 specification's own ceiling")
            ]
        }

        Glossary {
            x: Theme.horizontalPageMargin
            width: column.width - 2 * Theme.horizontalPageMargin
        }
    }

    VerticalScrollDecorator { }
}
