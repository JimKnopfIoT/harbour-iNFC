import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    id: dialog

    canAccept: path.text.length > 0
    onAccepted: app.log(Store.importFrom(path.text))

    Column {
        width: parent.width

        DialogHeader {
            title: qsTr("Import")
            acceptText: qsTr("Import")
        }

        TextField {
            id: path
            width: parent.width
            label: qsTr("Path to an iNFC export file")
            placeholderText: "/home/defaultuser/Documents/infc-export-....json"
            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
            EnterKey.iconSource: "image://theme/icon-m-enter-close"
            EnterKey.onClicked: focus = false
        }

        Label {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.Wrap
            text: qsTr("Imported cards are added, never merged over existing ones. Comments in the file are restored into your notes.")
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeExtraSmall
        }
    }
}
