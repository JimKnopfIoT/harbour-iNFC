import QtQuick 2.0
import Sailfish.Silica 1.0

/*
 * The "what works / what does not and why" block that every card type panel
 * carries. Two lists, deliberately equal in weight: knowing why something is
 * impossible is worth as much as knowing what is possible.
 */
Column {
    property string title
    property var works: []
    property var blocked: []
    property string footnote

    width: parent ? parent.width : 0
    spacing: Theme.paddingMedium

    SectionHeader { text: title }

    Column {
        width: parent.width
        spacing: Theme.paddingSmall
        visible: works.length > 0

        Label {
            text: qsTr("What works here")
            color: "#63d16a"
            font.pixelSize: Theme.fontSizeSmall
            font.bold: true
        }

        Repeater {
            model: works
            Row {
                width: parent.width
                spacing: Theme.paddingSmall
                Label {
                    text: "✓"
                    color: "#63d16a"
                    font.pixelSize: Theme.fontSizeSmall
                }
                Label {
                    width: parent.width - Theme.paddingSmall - Theme.horizontalPageMargin
                    text: modelData
                    wrapMode: Text.Wrap
                    color: Theme.primaryColor
                    font.pixelSize: Theme.fontSizeSmall
                }
            }
        }
    }

    Column {
        width: parent.width
        spacing: Theme.paddingSmall
        visible: blocked.length > 0

        // Only worth a caption when there is a positive list to contrast with.
        Label {
            visible: works.length > 0
            height: visible ? implicitHeight : 0
            text: qsTr("What does not, and why")
            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeSmall
            font.bold: true
        }

        Repeater {
            model: blocked
            Row {
                width: parent.width
                spacing: Theme.paddingSmall
                Label {
                    text: "✗"
                    color: Theme.secondaryColor
                    font.pixelSize: Theme.fontSizeSmall
                }
                Label {
                    width: parent.width - Theme.paddingSmall - Theme.horizontalPageMargin
                    text: modelData
                    wrapMode: Text.Wrap
                    color: Theme.secondaryColor
                    font.pixelSize: Theme.fontSizeSmall
                }
            }
        }
    }

    Label {
        width: parent.width
        visible: footnote.length > 0
        text: footnote
        wrapMode: Text.Wrap
        color: Theme.secondaryColor
        font.pixelSize: Theme.fontSizeExtraSmall
        font.italic: true
    }
}
