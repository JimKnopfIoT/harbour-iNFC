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
        canEmulate: true
        canRead: !Emulator.active
    }

    Column {
        id: column
        width: panel.width
        spacing: Theme.paddingMedium

        PageHeader {
            title: qsTr("Emulate")
            description: Nfc.cardEmulationSupported
                ? qsTr("Type 4A tag, host card emulation")
                : qsTr("not supported on this device")
        }

        Item {
            width: parent.width
            height: stateCol.height + Theme.paddingMedium

            Column {
                id: stateCol
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                spacing: Theme.paddingSmall

                Label {
                    width: parent.width
                    wrapMode: Text.Wrap
                    text: Emulator.active ? qsTr("Sending") : qsTr("Stopped")
                    color: Emulator.active ? app.colorEmit : Theme.secondaryColor
                    font.pixelSize: Theme.fontSizeMedium
                }

                DataField {
                    width: parent.width
                    label: qsTr("Payload")
                    value: Emulator.summary
                    emittable: true
                }

                DataField {
                    width: parent.width
                    label: qsTr("Last reader event")
                    value: Emulator.lastEvent ? Emulator.lastEvent : qsTr("none yet")
                }
            }
        }

        Row {
            x: Theme.horizontalPageMargin
            spacing: Theme.paddingMedium

            Button {
                text: Emulator.active ? qsTr("Stop") : qsTr("Start")
                enabled: Nfc.cardEmulationSupported
                onClicked: Emulator.active ? Emulator.stop() : Emulator.start()
            }
        }

        SectionHeader { text: qsTr("Presented identity") }

        TextField {
            width: parent.width
            label: qsTr("NFCID1, 4 bytes hex, blank for the default")
            placeholderText: "04A32F11"
            text: Emulator.nfcid1
            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
            onTextChanged: Emulator.nfcid1 = text
            EnterKey.iconSource: "image://theme/icon-m-enter-close"
            EnterKey.onClicked: focus = false
        }

        Label {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.Wrap
            text: qsTr("nfcd 1.2.2 and later lets the listening NFCID1 be set. Only single size (4 byte) identifiers are meaningful, and the controller may still override it.")
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeExtraSmall
        }

        InfoBlock {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            title: qsTr("What emulation is")
            works: [
                qsTr("The phone answers as a Type 4A tag holding an NDEF message - readers, other phones and NDEF terminals accept it"),
                qsTr("An NDEF message captured from a real tag can be served again unchanged"),
                qsTr("The presented NFCID1 and the ATS historical bytes are configurable")
            ]
            blocked: [
                qsTr("Emulating a Type 2 tag. Host card emulation is always ISO-DEP."),
                qsTr("Emulating MIFARE Classic, or any card whose security rests on a key the phone does not have."),
                qsTr("Payment. See the Payment card panel for the full reason.")
            ]
            footnote: qsTr("Card emulation needs nfcd 1.2.0 or newer, which arrived with SailfishOS 5.0.")
        }
    }

    VerticalScrollDecorator { }
}
