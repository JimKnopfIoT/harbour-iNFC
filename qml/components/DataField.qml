import QtQuick 2.0
import Sailfish.Silica 1.0

/*
 * One label/value row. `emittable` is the whole point: green means this phone
 * can send that value back out via card emulation, white means it can only
 * ever read it.
 */
Column {
    id: field

    property string label
    property string value
    property bool emittable: false
    property bool mono: false

    // Long dumps get folded away. EF.CardAccess alone is 292 bytes, which
    // buries everything else on the panel.
    readonly property int foldAt: 128
    readonly property bool foldable: value.length > foldAt
    property bool expanded: false

    width: parent ? parent.width : 0
    spacing: 0

    Row {
        width: parent.width
        spacing: Theme.paddingSmall

        Label {
            text: label
            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeExtraSmall
            width: parent.width - (emittable ? sendable.width + Theme.paddingSmall : 0)
            truncationMode: TruncationMode.Fade
        }

        Label {
            id: sendable
            visible: emittable
            text: "⤳ " + qsTr("sendable")
            color: "#63d16a"
            font.pixelSize: Theme.fontSizeExtraSmall
        }
    }

    Label {
        id: valueLabel
        width: parent.width
        text: (field.foldable && !field.expanded)
            ? field.value.substring(0, field.foldAt) + "…"
            : field.value
        wrapMode: mono ? Text.WrapAnywhere : Text.Wrap
        color: emittable ? "#63d16a" : Theme.primaryColor
        font.pixelSize: mono ? Theme.fontSizeExtraSmall : Theme.fontSizeSmall
        font.family: mono ? "monospace" : Theme.fontFamily

        MouseArea {
            anchors.fill: parent
            enabled: field.foldable
            onClicked: field.expanded = !field.expanded
        }
    }

    Label {
        visible: field.foldable
        text: field.expanded
            ? qsTr("tap to fold")
            : qsTr("%1 characters · tap to expand").arg(field.value.length)
        color: Theme.secondaryColor
        font.pixelSize: Theme.fontSizeExtraSmall
        font.italic: true
    }
}
