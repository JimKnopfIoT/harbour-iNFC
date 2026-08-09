import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

SilicaFlickable {
    id: panel

    property var carousel

    flickableDirection: Flickable.VerticalFlick
    contentHeight: column.height + Theme.paddingLarge * 3

    PanelPulley { carousel: panel.carousel }

    Column {
        id: column
        width: panel.width
        spacing: Theme.paddingSmall

        PageHeader {
            title: qsTr("Log")
            description: qsTr("%1 entries").arg(app.activity.length)
        }

        Row {
            x: Theme.horizontalPageMargin
            spacing: Theme.paddingMedium

            Button {
                text: qsTr("Clear")
                enabled: app.activity.length > 0
                onClicked: app.activity = []
            }
        }

        Repeater {
            model: app.activity
            Label {
                x: Theme.horizontalPageMargin
                width: column.width - 2 * Theme.horizontalPageMargin
                text: modelData
                wrapMode: Text.Wrap
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeExtraSmall
                font.family: "monospace"
            }
        }

        Label {
            visible: app.activity.length === 0
            x: Theme.horizontalPageMargin
            width: column.width - 2 * Theme.horizontalPageMargin
            wrapMode: Text.Wrap
            text: qsTr("Nothing has happened yet.")
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeSmall
        }

        InfoBlock {
            x: Theme.horizontalPageMargin
            width: column.width - 2 * Theme.horizontalPageMargin
            title: qsTr("About this log")
            works: [
                qsTr("Everything this app does: arming the radio, reads, classifications, APDUs answered while emulating")
            ]
            blocked: [
                qsTr("nfcd's own internal trace is not shown here yet. It is available through the daemon's logging plugin and is on the list.")
            ]
        }
    }

    VerticalScrollDecorator { }
}
