import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    id: dialog

    canAccept: Store.count > 0
    onOpened: name.text = Store.suggestedExportName()
    onAccepted: app.log(Store.exportAll(name.text, withNotes.checked))

    Column {
        width: parent.width

        DialogHeader {
            title: qsTr("Export")
            acceptText: qsTr("Export")
        }

        Label {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.Wrap
            text: qsTr("%n saved card(s) will be written as a single JSON file.", "", Store.count)
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeSmall
        }

        TextField {
            id: name
            width: parent.width
            label: qsTr("File name")
            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
            EnterKey.iconSource: "image://theme/icon-m-enter-close"
            EnterKey.onClicked: focus = false
        }

        Label {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.WrapAnywhere
            text: qsTr("Target folder: %1").arg(Store.exportDir())
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeExtraSmall
        }

        TextSwitch {
            id: withNotes
            text: qsTr("Include comments")
            description: qsTr("Off keeps your notes out of the exported file")
        }
    }
}
