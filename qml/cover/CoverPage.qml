import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    id: cover

    // Card corner with the antenna running outside it - only hinted, sitting
    // behind the text rather than competing with it.
    Image {
        anchors {
            right: parent.right
            bottom: parent.bottom
            rightMargin: -parent.width * 0.18
            bottomMargin: -parent.height * 0.12
        }
        width: parent.width * 1.05
        height: width
        fillMode: Image.PreserveAspectFit
        source: appShareDir + "/cover.png"
        opacity: Emulator.active ? 0.30 : 0.16
        Behavior on opacity { FadeAnimation { duration: 400 } }
    }

    Column {
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            topMargin: Theme.paddingLarge
            leftMargin: Theme.paddingMedium
            rightMargin: Theme.paddingMedium
        }
        spacing: Theme.paddingSmall

        Label {
            text: "iNFC"
            font.pixelSize: Theme.fontSizeLarge
            color: Theme.primaryColor
        }

        Label {
            width: parent.width
            wrapMode: Text.Wrap
            font.pixelSize: Theme.fontSizeExtraSmall
            color: Emulator.active ? "#63d16a"
                : (Nfc.state === 1 ? Theme.highlightColor : Theme.secondaryColor)
            text: Emulator.active ? qsTr("sending")
                : (Nfc.state === 1 ? qsTr("waiting for tag")
                : (Nfc.hasResult ? Nfc.kindLabel : qsTr("ready")))
        }
    }

    CoverActionList {
        enabled: !Emulator.active
        CoverAction {
            iconSource: "image://theme/icon-cover-search"
            onTriggered: Nfc.arm()
        }
    }
}
