import QtQuick 2.0
import Sailfish.Silica 1.0
import "../panels"

/*
 * The whole app is one page holding a horizontal carousel of panels. Silica's
 * page stack is vertical in spirit - push and pop - which is the wrong shape
 * for a set of equal siblings you want to leaf through.
 *
 * PathView, not ListView: the carousel is a closed ring, so it wraps both
 * ways - swipe right off the first panel and the last one comes round. A flat
 * horizontal path across the view makes it read as a plain pager, not a
 * coverflow. Each panel keeps its own vertical flicking and pull-down menu.
 */
Page {
    id: page

    allowedOrientations: Orientation.Portrait | Orientation.PortraitInverted

    PathView {
        id: carousel

        anchors.fill: parent
        clip: true

        // The Log panel's index, so navigation works whether or not the uv
        // Ultraviolence panel is present (it shifts Log and Archive by one).
        property int logIndex: 0

        // Three delegates alive at a time: the current panel and its two
        // neighbours, enough for a smooth swipe in either direction without
        // instantiating all the heavy pages at once.
        pathItemCount: 3
        snapMode: PathView.SnapOneItem
        highlightRangeMode: PathView.StrictlyEnforceRange
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        highlightMoveDuration: 220
        maximumFlickVelocity: width * 5

        // A straight line one view-width to each side of centre. With the
        // highlight pinned at 0.5 the current panel sits centred and its
        // neighbours wait just off screen left and right.
        path: Path {
            startX: -carousel.width
            startY: carousel.height / 2
            PathLine {
                x: carousel.width * 2
                y: carousel.height / 2
            }
        }

        // Built at load: the base panels always, the Ultraviolence panel only
        // in the uv build. The first eight indices (Read..Emulate) are the
        // same in both, so Nfc.kindPanel jumps stay valid.
        model: ListModel { id: panelModel }

        Component.onCompleted: {
            var base = ["ReadPanel.qml", "NdefPanel.qml", "HealthCardPanel.qml",
                "EidPanel.qml", "PaymentPanel.qml", "TransponderPanel.qml",
                "OtherPanel.qml", "EmulatePanel.qml"]
            for (var i = 0; i < base.length; i++) panelModel.append({ src: base[i] })
            if (uvBuild) panelModel.append({ src: "UltraviolencePanel.qml" })
            carousel.logIndex = panelModel.count
            panelModel.append({ src: "LogPanel.qml" })
            panelModel.append({ src: "ArchivePanel.qml" })
        }

        delegate: Loader {
            width: carousel.width
            height: carousel.height
            source: Qt.resolvedUrl("../panels/" + model.src)
            onLoaded: item.carousel = carousel
        }

        // Leaving the Read panel hands the fresh result over to wherever it
        // belongs. The Read panel is then empty again, so the next card can
        // never be confused with the last one.
        onCurrentIndexChanged: {
            if (currentIndex !== 0 && Nfc.hasResult) {
                Nfc.handOff()
            }
        }
    }

    // Page indicator, kept out of the way of the pull-down menu.
    Row {
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: Theme.paddingMedium
        }
        spacing: Theme.paddingSmall

        Repeater {
            model: carousel.count
            Rectangle {
                width: Theme.paddingSmall
                height: width
                radius: width / 2
                color: index === carousel.currentIndex
                    ? Theme.highlightColor : Theme.secondaryColor
                opacity: index === carousel.currentIndex ? 1.0 : 0.4
                Behavior on opacity { FadeAnimation { } }
            }
        }
    }

    // A finished read jumps straight to the panel that explains the card.
    // The user asked for a button rather than an automatic jump, so this only
    // remembers where to go; ReadPanel draws the button.
    Connections {
        target: Nfc
        onTagRead: app.lastSavedId = ""
    }
}
