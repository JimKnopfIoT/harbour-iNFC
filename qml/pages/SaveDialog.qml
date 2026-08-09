import QtQuick 2.0
import Sailfish.Silica 1.0

/*
 * Archiving without a name produces a pile, not an archive. So the label is
 * asked for at save time and pre-filled with card type plus the current date
 * and time - editable, but never empty.
 *
 * Label and comment both go to notes/, next to the card data and never into
 * it. Nothing typed here can end up on a card.
 */
Dialog {
    id: dialog

    // A pre-built card map to save. Null means "take the current NFC read" -
    // that is the common case. The Proxmark passes its own map so LF tags can
    // be archived and exported just like anything the phone reads.
    property var cardMap: null

    readonly property string kindLabel: cardMap
        ? (cardMap.kindLabel || "")
        : (Nfc.hasResult ? Nfc.kindLabel : Nfc.detailKindLabel)
    readonly property string uid: cardMap
        ? (cardMap.uid || "")
        : (Nfc.hasResult ? Nfc.uid : Nfc.detailUid)

    canAccept: label.text.length > 0

    onOpened: label.text = Store.suggestedLabel(kindLabel)
    onAccepted: {
        var id = Store.save(cardMap ? cardMap : Nfc.resultMap())
        if (id) {
            Store.setLabel(id, label.text)
            if (note.text.length > 0) Store.setComment(id, note.text)
            app.lastSavedId = id
            app.log(qsTr("Archived as “%1”").arg(label.text))
        } else {
            app.log(qsTr("Save failed"))
        }
    }

    Column {
        width: parent.width

        DialogHeader {
            title: qsTr("Save to archive")
            acceptText: qsTr("Save")
        }

        Label {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.Wrap
            text: dialog.kindLabel + (dialog.uid.length > 0
                ? "  ·  " + dialog.uid : "")
            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeSmall
        }

        TextField {
            id: label
            width: parent.width
            label: qsTr("Name in the archive")
            placeholderText: qsTr("How you will recognise it again")
            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: note.focus = true
        }

        TextArea {
            id: note
            width: parent.width
            height: Math.max(implicitHeight, Theme.itemSizeHuge)
            label: qsTr("Comment (optional)")
            placeholderText: qsTr("Where it came from, what it opens, what you tried")
        }

        Label {
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.Wrap
            text: qsTr("Name and comment are stored beside the card data, never inside it.")
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeExtraSmall
        }
    }
}
