import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

/*
 * Ultraviolence: the page where the phone stops and external hardware takes
 * over. Locked until you deliberately unlock it - not to gatekeep, but to
 * mark the boundary. Everything here needs a Proxmark on the USB port.
 */
SilicaFlickable {
    id: panel

    property var carousel

    flickableDirection: Flickable.VerticalFlick
    contentHeight: column.height + Theme.paddingLarge * 3

    // Build a card map from the last LF read so it can go into the archive
    // and exports, exactly like a phone read.
    function lfCardMap() {
        return {
            "kind": "lf-badge",
            "kindLabel": Proxmark.lastTagType + qsTr(" (125 kHz)"),
            "uid": Proxmark.lastTag,
            "frequency": "125 kHz",
            "fields": [ {
                "group": qsTr("Proxmark · 125 kHz"),
                "label": Proxmark.lastTagType + qsTr(" ID"),
                "value": Proxmark.lastTag,
                "emittable": false,
                "mono": true
            } ],
            "records": [],
            "type2Dump": ""
        }
    }

    // Same, for the last 13.56 MHz (HF) read.
    function hfCardMap() {
        return {
            "kind": "hf-iso14443a",
            "kindLabel": Proxmark.hfType + qsTr(" (13.56 MHz)"),
            "uid": Proxmark.hfUid,
            "frequency": "13.56 MHz",
            "fields": [
                {
                    "group": qsTr("Proxmark · 13.56 MHz"),
                    "label": qsTr("Type"),
                    "value": Proxmark.hfType,
                    "emittable": false, "mono": false
                },
                {
                    "group": qsTr("Proxmark · 13.56 MHz"),
                    "label": qsTr("UID"),
                    "value": Proxmark.hfUid,
                    "emittable": false, "mono": true
                },
                {
                    "group": qsTr("Proxmark · 13.56 MHz"),
                    "label": qsTr("ATQA / SAK"),
                    "value": Proxmark.hfDetails,
                    "emittable": false, "mono": true
                }
            ].concat(Proxmark.mfResult.length > 0 ? [{
                    "group": qsTr("MIFARE Classic keys"),
                    "label": qsTr("Sector keys"),
                    "value": Proxmark.mfResult,
                    "emittable": false, "mono": true
                }] : []).concat(Proxmark.mfDump.length > 0 ? [{
                    "group": qsTr("MIFARE Classic dump"),
                    "label": qsTr("%1 blocks").arg(Proxmark.mfDumpBlocks),
                    "value": Proxmark.mfDump,
                    "emittable": false, "mono": true
                }] : []),
            "records": [],
            "type2Dump": ""
        }
    }

    PullDownMenu {
        MenuItem {
            text: qsTr("Log")
            onClicked: panel.carousel.currentIndex = panel.carousel.logIndex
        }
        MenuItem {
            visible: app.uvUnlocked && Proxmark.hfUid.length > 0
            text: qsTr("Save HF card to archive")
            onClicked: pageStack.push(Qt.resolvedUrl("../pages/SaveDialog.qml"),
                { cardMap: panel.hfCardMap() })
        }
        MenuItem {
            visible: app.uvUnlocked && Proxmark.lastTag.length > 0
            text: qsTr("Save LF tag to archive")
            onClicked: pageStack.push(Qt.resolvedUrl("../pages/SaveDialog.qml"),
                { cardMap: panel.lfCardMap() })
        }
        MenuItem {
            visible: app.uvUnlocked
            text: Proxmark.connected ? qsTr("Disconnect") : qsTr("Connect")
            enabled: !Proxmark.busy
            onClicked: Proxmark.connected ? Proxmark.disconnect()
                                          : Proxmark.connectDevice()
        }
    }

    Column {
        id: column
        width: panel.width
        spacing: Theme.paddingMedium

        PageHeader {
            title: qsTr("Ultraviolence")
            description: qsTr("Proxmark 3 · external hardware")
        }

        // ---- Locked state ------------------------------------------------
        Column {
            width: parent.width
            spacing: Theme.paddingLarge
            visible: !app.uvUnlocked

            Item { width: 1; height: Theme.paddingLarge }

            // Weathered yellow radiation sign, almost the full width of the
            // page - old, chipped, crumbling at the edges. It is the button:
            // its centre pulses and carries "Enter".
            Item {
                id: sign
                anchors.horizontalCenter: parent.horizontalCenter
                width: panel.width - 2 * Theme.paddingLarge
                height: width

                Image {
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: appShareDir + "/radiation.png"
                }

                // A dark disc over the trefoil's centre (radius 55 of 512).
                // Pulsing its opacity makes the middle slowly breathe darker.
                Rectangle {
                    id: glow
                    anchors.centerIn: parent
                    width: sign.width * 0.235
                    height: width
                    radius: width / 2
                    color: "#1c1500"
                    SequentialAnimation on opacity {
                        loops: Animation.Infinite
                        running: !app.uvUnlocked
                        NumberAnimation { from: 0.0; to: 0.55; duration: 2600
                                          easing.type: Easing.InOutSine }
                        NumberAnimation { from: 0.55; to: 0.0; duration: 2600
                                          easing.type: Easing.InOutSine }
                    }
                }

                Label {
                    anchors.centerIn: parent
                    text: qsTr("Enter")
                    // Light, so it stays readable as the centre darkens under
                    // the pulse.
                    color: "#fff3c4"
                    font.bold: true
                    font.pixelSize: Theme.fontSizeLarge
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        Sfx.play(appShareDir + "/explosion.wav")
                        app.uvUnlocked = true
                    }
                }
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("Beyond this point nothing runs on the phone alone. This is where a Proxmark 3 reads the frequencies and cards the phone cannot: 125 kHz, MIFARE Classic, and more.")
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeSmall
            }

        }

        // ---- Unlocked state ---------------------------------------------
        Column {
            width: parent.width
            spacing: Theme.paddingMedium
            visible: app.uvUnlocked

            SectionHeader { text: qsTr("Device") }

            DataField {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                label: qsTr("Port")
                value: Proxmark.portPath + (Proxmark.present ? qsTr(" · present")
                                                             : qsTr(" · not found"))
            }

            DataField {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                label: qsTr("Status")
                value: Proxmark.status
            }

            DataField {
                visible: Proxmark.version.length > 0
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                label: qsTr("Firmware")
                value: Proxmark.version
                mono: true
            }

            Row {
                x: Theme.horizontalPageMargin
                spacing: Theme.paddingMedium

                Button {
                    text: Proxmark.connected ? qsTr("Disconnect") : qsTr("Connect")
                    enabled: !Proxmark.busy
                    onClicked: Proxmark.connected ? Proxmark.disconnect()
                                                  : Proxmark.connectDevice()
                }
                BusyIndicator {
                    running: Proxmark.busy
                    size: BusyIndicatorSize.Small
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            SectionHeader {
                text: qsTr("125 kHz (LF)")
                visible: Proxmark.connected
            }

            FrequencyBand {
                visible: Proxmark.connected
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                // With the Proxmark, the low band is finally the active one.
                active: "125 kHz"
            }

            DataField {
                visible: Proxmark.lastTag.length > 0
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                label: Proxmark.lastTagType + qsTr(" ID")
                value: Proxmark.lastTag
                mono: true
            }

            Button {
                x: Theme.horizontalPageMargin
                text: qsTr("Search 125 kHz badge")
                enabled: Proxmark.connected && !Proxmark.busy
                onClicked: Proxmark.readLf()
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                wrapMode: Text.Wrap
                text: qsTr("Tries EM4100, HID Prox and IO Prox in turn - the common access-badge formats.")
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }

            // ---- 13.56 MHz (HF) ---------------------------------------------
            SectionHeader {
                text: qsTr("13.56 MHz (HF)")
                visible: Proxmark.connected
            }

            DataField {
                visible: Proxmark.hfUid.length > 0
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                label: Proxmark.hfType
                value: qsTr("UID %1\n%2").arg(Proxmark.hfUid).arg(Proxmark.hfDetails)
                mono: true
            }

            Button {
                x: Theme.horizontalPageMargin
                visible: Proxmark.connected
                text: qsTr("Read 13.56 MHz card")
                enabled: !Proxmark.busy
                onClicked: Proxmark.readHf()
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                visible: Proxmark.connected
                wrapMode: Text.Wrap
                text: qsTr("Reads UID, ATQA and SAK. The SAK says whether it is MIFARE Classic - the type the phone itself cannot read.")
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }

            // MIFARE Classic default-key check.
            Button {
                x: Theme.horizontalPageMargin
                visible: Proxmark.connected
                text: qsTr("Check MIFARE Classic default keys")
                enabled: !Proxmark.busy
                onClicked: Proxmark.checkMifareKeys()
            }

            Label {
                visible: Proxmark.mfResult.length > 0
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                text: qsTr("%1 sector(s) open with a default key:").arg(Proxmark.mfFound)
                color: Proxmark.mfFound > 0 ? "#63d16a" : Theme.secondaryColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }

            Label {
                visible: Proxmark.mfResult.length > 0
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                text: Proxmark.mfResult
                wrapMode: Text.WrapAnywhere
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeExtraSmall
                font.family: "monospace"
            }

            Button {
                x: Theme.horizontalPageMargin
                visible: Proxmark.connected && Proxmark.mfFound > 0
                text: qsTr("Dump sectors")
                enabled: !Proxmark.busy
                onClicked: Proxmark.dumpMifare()
            }

            Label {
                visible: Proxmark.mfDump.length > 0
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                text: qsTr("%1 of 64 blocks read - pull down to save it").arg(Proxmark.mfDumpBlocks)
                color: Proxmark.mfDumpBlocks === 64 ? "#63d16a" : Theme.secondaryColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }

            Label {
                visible: Proxmark.mfDump.length > 0
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                text: Proxmark.mfDump
                wrapMode: Text.WrapAnywhere
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeExtraSmall
                font.family: "monospace"
            }

            // ---- Writing to a blank T5577 -----------------------------------
            SectionHeader {
                text: qsTr("Write to T5577")
                visible: Proxmark.connected
            }

            RemorsePopup { id: writeRemorse }

            // Clone the last-read EM4100 id onto a blank T5577.
            Button {
                x: Theme.horizontalPageMargin
                visible: Proxmark.connected
                text: qsTr("Clone %1 to T5577").arg(Proxmark.lastTag.length > 0
                        ? Proxmark.lastTag : "…")
                enabled: Proxmark.lastTagType === "EM4100" && !Proxmark.busy
                onClicked: writeRemorse.execute(
                    qsTr("Writing %1…").arg(Proxmark.lastTag),
                    function() { Proxmark.writeEm4100(Proxmark.lastTag) })
            }

            // Test write: "Dolly" is exactly five ASCII bytes = a 40-bit EM4100
            // id. 44 6F 6C 6C 79 -> the tag literally becomes "Dolly".
            Button {
                x: Theme.horizontalPageMargin
                visible: Proxmark.connected
                text: qsTr("Write test sheep “Dolly” (446F6C6C79)")
                enabled: !Proxmark.busy
                onClicked: writeRemorse.execute(
                    qsTr("Writing Dolly…"),
                    function() { Proxmark.writeEm4100("446F6C6C79") })
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                visible: Proxmark.connected
                wrapMode: Text.Wrap
                text: qsTr("Writing permanently reconfigures the tag, but a T5577 can be rewritten any number of times. After writing, search again to read it back.")
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }

            InfoBlock {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                title: qsTr("What this unlocks")
                works: [
                    qsTr("125 kHz LF: EM4100, HID Prox, T5577 - a whole band the phone has no antenna for"),
                    qsTr("MIFARE Classic with Crypto1: the access badges and parking cards that land on the Other panel"),
                    qsTr("Sniffing, and cloning onto magic cards")
                ]
                blocked: [
                    qsTr("First the pipe has to be proven: connect, and the firmware version should appear above. If the port opens but nothing answers, the log will say so.")
                ]
                footnote: qsTr("Reading LF and Classic is the next step, once the connection is confirmed against your device.")
            }
        }
    }

    VerticalScrollDecorator { }
}
