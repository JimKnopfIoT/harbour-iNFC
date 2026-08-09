import QtQuick 2.0
import Sailfish.Silica 1.0

/*
 * Comments live in their own file, next to the card data but never inside it.
 * That is a storage decision, not a UI one - see CardStore.
 */
Dialog {
    id: dialog

    property string entryId

    canAccept: entryId.length > 0

    onOpened: field.text = entryId.length > 0 ? Store.comment(entryId) : ""
    onAccepted: {
        Store.setComment(entryId, field.text)
        app.log(qsTr("Comment saved for %1").arg(entryId))
    }

    Column {
        width: parent.width

        DialogHeader {
            title: qsTr("Comment")
            acceptText: qsTr("Save")
        }

        TextArea {
            id: field
            width: parent.width
            height: Math.max(implicitHeight, Theme.itemSizeHuge * 2)
            label: qsTr("Your notes about this card")
            placeholderText: qsTr("Where it came from, what it opens, what you tried")
        }

        Label {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.Wrap
            text: entryId.length > 0
                ? qsTr("Stored separately from the card data. Writing to a tag never includes this text.")
                : qsTr("Save the card first, then a comment can be attached to it.")
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeExtraSmall
        }
    }
}
