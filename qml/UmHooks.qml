import QtQuick 2.0

/*
 * um-build only. Wires the Proxmark's log into the shared activity log.
 * Loaded through a guard in main.qml, so the base app never references the
 * Proxmark context object (which it does not have).
 */
Item {
    Connections {
        target: Proxmark
        onLogLine: app.log("PM3: " + line)
    }
}
