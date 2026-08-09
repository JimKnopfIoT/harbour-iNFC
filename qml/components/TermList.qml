import QtQuick 2.0
import Sailfish.Silica 1.0

/*
 * A list of term/definition pairs, used as the body of a glossary section.
 *
 * Its properties are declared here rather than on the enclosing section on
 * purpose: a Component instantiated through a Loader does not see properties
 * of the object holding the Loader, so passing them implicitly silently
 * yields empty sections.
 */
Column {
    id: list

    property string intro
    property string outro
    property var terms: []

    width: parent ? parent.width : 0
    spacing: Theme.paddingMedium

    Label {
        width: parent.width
        visible: list.intro.length > 0
        text: list.intro
        wrapMode: Text.Wrap
        color: Theme.secondaryColor
        font.pixelSize: Theme.fontSizeExtraSmall
    }

    Repeater {
        model: list.terms

        Row {
            width: list.width
            spacing: Theme.paddingMedium

            Label {
                id: term
                width: Math.round(Theme.itemSizeSmall * 1.15)
                text: modelData.term
                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeExtraSmall
                font.family: "monospace"
                wrapMode: Text.WrapAnywhere
            }

            Label {
                width: parent.width - term.width - Theme.paddingMedium
                text: modelData.text
                wrapMode: Text.Wrap
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }
        }
    }

    Label {
        width: parent.width
        visible: list.outro.length > 0
        text: list.outro
        wrapMode: Text.Wrap
        color: Theme.secondaryColor
        font.pixelSize: Theme.fontSizeExtraSmall
        font.italic: true
    }

    // Breathing room below the section; Column.bottomPadding needs QtQuick 2.6.
    Item {
        width: 1
        height: Theme.paddingLarge
    }
}
