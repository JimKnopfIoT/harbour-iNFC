import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

SilicaFlickable {
    id: panel

    property var carousel

    // Filtered in JS rather than with a proxy model: the archive is a handful
    // of entries, not a database.
    function matching() {
        var q = filter.text.toLowerCase()
        if (q.length === 0) return Store.entries
        return Store.entries.filter(function(e) {
            return (e.label + " " + e.comment + " " + e.kindLabel + " " + e.uid)
                .toLowerCase().indexOf(q) >= 0
        })
    }

    flickableDirection: Flickable.VerticalFlick
    contentHeight: column.height + Theme.paddingLarge * 3

    PanelPulley { carousel: panel.carousel }

    Column {
        id: column
        width: panel.width
        spacing: Theme.paddingMedium

        PageHeader {
            title: qsTr("Archive")
            description: qsTr("%n saved card(s)", "", Store.count)
        }

        SearchField {
            id: filter
            width: parent.width
            visible: Store.count > 3
            placeholderText: qsTr("Search name, comment, type or UID")
            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
            EnterKey.iconSource: "image://theme/icon-m-enter-close"
            EnterKey.onClicked: focus = false
        }

        Repeater {
            model: panel.matching()

            ListItem {
                width: column.width
                contentHeight: entryCol.height + Theme.paddingMedium

                menu: ContextMenu {
                    MenuItem {
                        text: qsTr("Load into Read")
                        onClicked: {
                            Nfc.restore(Store.load(modelData.id))
                            app.lastSavedId = modelData.id
                            panel.carousel.currentIndex = 0
                        }
                    }
                    MenuItem {
                        text: qsTr("Comment")
                        onClicked: pageStack.push(
                            Qt.resolvedUrl("../pages/CommentDialog.qml"),
                            { entryId: modelData.id })
                    }
                    MenuItem {
                        text: qsTr("Delete")
                        onClicked: Store.remove(modelData.id)
                    }
                }

                Column {
                    id: entryCol
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    anchors.verticalCenter: parent.verticalCenter

                    Label {
                        width: parent.width
                        text: modelData.label ? modelData.label : modelData.kindLabel
                        truncationMode: TruncationMode.Fade
                        color: Theme.primaryColor
                        font.pixelSize: Theme.fontSizeSmall
                    }
                    Label {
                        text: modelData.kindLabel + "  ·  "
                            + (modelData.uid ? modelData.uid : qsTr("no UID"))
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeExtraSmall
                    }
                    Label {
                        text: modelData.saved
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeExtraSmall
                    }
                    Label {
                        visible: modelData.comment.length > 0
                        width: parent.width
                        text: modelData.comment
                        wrapMode: Text.Wrap
                        color: Theme.highlightColor
                        font.pixelSize: Theme.fontSizeExtraSmall
                        font.italic: true
                    }
                }
            }
        }

        Label {
            visible: Store.count === 0
            x: Theme.horizontalPageMargin
            width: column.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.Wrap
            text: qsTr("Nothing saved yet. Read a tag and pull down to Save.")
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeSmall
        }

        InfoBlock {
            x: Theme.horizontalPageMargin
            width: column.width - 2 * Theme.horizontalPageMargin
            title: qsTr("How this is stored")
            works: [
                qsTr("Card data goes to cards/, your comments go to notes/, in two separate files"),
                qsTr("Writing to a tag only ever draws from the card data, so a comment cannot end up on a card"),
                qsTr("Export can include the comments or leave them behind")
            ]
            blocked: []
            footnote: Store.storagePath()
        }
    }

    VerticalScrollDecorator { }
}
