import QtQuick 2.0
import Sailfish.Silica 1.0

/*
 * The RFID spectrum as a short ladder, low to high. The band actually used
 * for this read stands out in white; every other band a tag could live on is
 * shown struck through and dimmed - present, so you see where this read sits
 * in the whole picture, but clearly not in play here.
 *
 * The phone only ever reaches 13.56 MHz. The greyed-out neighbours are the
 * standing reminder of what needs other hardware - a Proxmark for 125/134 kHz,
 * a UHF reader for the rest.
 */
Column {
    id: band

    property string active            // e.g. "13.56 MHz"

    width: parent ? parent.width : 0
    spacing: Theme.paddingSmall / 2

    Repeater {
        model: [
            { f: "125 kHz",    tag: "LF",  note: qsTr("EM4100, HID Prox, T5577") },
            { f: "134.2 kHz",  tag: "LF",  note: qsTr("animal ID, FDX-B") },
            { f: "13.56 MHz",  tag: "HF",  note: qsTr("ISO 14443, ISO 15693, NFC") },
            { f: "868–928 MHz", tag: "UHF", note: qsTr("EPC Gen2, long range") },
            { f: "2.45 GHz",   tag: "SHF", note: qsTr("active tags") }
        ]

        Row {
            width: band.width
            spacing: Theme.paddingMedium

            property bool on: modelData.f === band.active

            Label {
                width: Math.round(band.width * 0.32)
                horizontalAlignment: Text.AlignRight
                text: modelData.f
                color: parent.on ? Theme.primaryColor : Theme.secondaryColor
                opacity: parent.on ? 1.0 : 0.28
                font.bold: parent.on
                font.pixelSize: parent.on ? Theme.fontSizeMedium
                                          : Theme.fontSizeExtraSmall
            }

            Label {
                width: Theme.itemSizeExtraSmall / 2
                text: modelData.tag
                color: parent.on ? Theme.highlightColor : Theme.secondaryColor
                opacity: parent.on ? 1.0 : 0.28
                font.pixelSize: Theme.fontSizeExtraSmall
            }

            Label {
                width: band.width - Math.round(band.width * 0.32)
                       - Theme.itemSizeExtraSmall / 2 - 2 * Theme.paddingMedium
                text: modelData.note
                truncationMode: TruncationMode.Fade
                color: parent.on ? Theme.secondaryHighlightColor
                                 : Theme.secondaryColor
                opacity: parent.on ? 1.0 : 0.28
                font.pixelSize: Theme.fontSizeExtraSmall
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}
