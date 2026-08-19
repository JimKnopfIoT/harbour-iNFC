import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"
import "cover"

ApplicationWindow {
    id: app

    // ------------------------------------------------------------------
    // The colour contract of this app:
    //
    //   white  - data we read off the card
    //   green  - data this phone can actually send back out again
    //
    // Green therefore means exactly one thing: it survives the trip through
    // host card emulation. Everything the hardware cannot re-emit stays
    // white, no matter how interesting it is.
    // ------------------------------------------------------------------
    readonly property color colorData: Theme.primaryColor
    readonly property color colorEmit: "#63d16a"
    readonly property color colorEmitDim: "#3d7f41"

    // App internal activity log, shown by the Log panel.
    property var activity: []

    // Id of the entry the Comment dialog will attach to.
    property string lastSavedId: ""

    // Ultimate Mode is unlocked per session, on purpose: you reach for it
    // deliberately, and a restart puts the guard back. Only the um build has
    // the page at all; umBuild is a context property from C++.
    property bool umUnlocked: false

    function log(line) {
        var now = Qt.formatTime(new Date(), "hh:mm:ss")
        var next = activity.slice(0)
        next.unshift(now + "  " + line)
        if (next.length > 400) next.pop()
        activity = next
    }

    initialPage: Component { MainPage { } }
    cover: Component { CoverPage { } }
    allowedOrientations: defaultAllowedOrientations

    Connections {
        target: Nfc
        onStateChanged: app.log(Nfc.statusText)
        onTagRead: app.log("Identified as " + Nfc.kindLabel)
    }

    Connections {
        target: Emulator
        onLastEventChanged: app.log("HCE: " + Emulator.lastEvent)
    }

    // Everything that touches external hardware (Proxmark, sound) lives in
    // the um build only. Loading it through a guard keeps the base app free
    // of any reference to objects it does not have.
    Loader {
        active: umBuild
        source: umBuild ? Qt.resolvedUrl("UmHooks.qml") : ""
    }
}
