import QtQuick 2.0
import Sailfish.Silica 1.0

/*
 * The one pull-down menu, used by every panel.
 *
 * Same seven entries in the same order everywhere, so the menu never moves
 * under your thumb. Entries that cannot do anything useful on the current
 * panel are disabled rather than hidden.
 */
PullDownMenu {
    id: pulley

    property var carousel
    property bool canRead: true
    property bool canEmulate: false
    // Either side counts: the fresh read, or the one handed to a detail panel.
    property bool canSave: Nfc.hasResult || Nfc.hasDetail
    property bool canComment: Nfc.hasResult || Nfc.hasDetail
    property bool canExport: true
    property bool canImport: true

    signal emulateRequested()

    function goTo(index) {
        if (carousel) carousel.currentIndex = index
    }

    // Declared bottom-up: the last entry is the one nearest your thumb.
    MenuItem {
        text: qsTr("Import")
        enabled: pulley.canImport
        onClicked: pageStack.push(Qt.resolvedUrl("../pages/ImportDialog.qml"))
    }
    MenuItem {
        text: qsTr("Export")
        enabled: pulley.canExport
        onClicked: pageStack.push(Qt.resolvedUrl("../pages/ExportDialog.qml"))
    }
    MenuItem {
        text: qsTr("Log")
        onClicked: pulley.goTo(carousel ? carousel.logIndex : 0)
    }
    MenuItem {
        text: qsTr("Comment")
        enabled: pulley.canComment
        onClicked: {
            if (!app.lastSavedId) {
                app.lastSavedId = Store.save(Nfc.resultMap())
            }
            pageStack.push(Qt.resolvedUrl("../pages/CommentDialog.qml"),
                { entryId: app.lastSavedId })
        }
    }
    MenuItem {
        // Named so it is clear no file dialog follows: this puts the card
        // into the app's own archive. Export is the one that writes a file.
        text: qsTr("Save to archive")
        enabled: pulley.canSave
        onClicked: pageStack.push(Qt.resolvedUrl("../pages/SaveDialog.qml"))
    }
    MenuItem {
        text: Emulator.active ? qsTr("Stop sending") : qsTr("Send / emulate")
        enabled: pulley.canEmulate && Nfc.cardEmulationSupported
        onClicked: {
            if (Emulator.active) {
                Emulator.stop()
            } else {
                pulley.emulateRequested()
                pulley.goTo(7)
            }
        }
    }
    MenuItem {
        text: Nfc.state === 1 ? qsTr("Cancel reading") : qsTr("Read")
        enabled: pulley.canRead
        onClicked: {
            if (Nfc.state === 1) {
                Nfc.disarm()
            } else {
                // Reading always happens on the entry panel. Going there
                // first hands off whatever was still open and leaves an
                // empty page, so the arriving card has the stage to itself.
                pulley.goTo(0)
                Nfc.arm()
            }
        }
    }
}
