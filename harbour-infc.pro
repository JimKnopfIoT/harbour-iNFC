# Two variants from one source tree:
#   default        -> harbour-infc      (base app, NFC only, publishable)
#   CONFIG+=um     -> harbour-infc-um   (adds Proxmark, sound, Ultimate Mode)
#
# The um build is selected by its own rpm spec, which runs qmake with
# CONFIG+=um. Everything below branches on that.

CONFIG(um) {
    TARGET = harbour-infc-um
    DEFINES += UM_BUILD
} else {
    TARGET = harbour-infc
}
DEFINES += "APP_TARGET=\\\"$${TARGET}\\\""

CONFIG += sailfishapp
CONFIG += sailfishapp_i18n
# We install the QML ourselves (per variant) instead of shipping the whole
# tree, so the base app never carries the um-only files.
CONFIG += sailfishapp_no_deploy_qml

QT += dbus network

# ---- Shared sources (both variants) --------------------------------------
HEADERS += \
    src/asn1util.h \
    src/nfcdbus.h \
    src/nfcbackend.h \
    src/ndefutil.h \
    src/emulator.h \
    src/cardstore.h

SOURCES += \
    src/harbour-infc.cpp \
    src/asn1util.cpp \
    src/nfcdbus.cpp \
    src/nfcbackend.cpp \
    src/ndefutil.cpp \
    src/emulator.cpp \
    src/cardstore.cpp

# ---- Shared QML ----------------------------------------------------------
qml_root.path = /usr/share/$${TARGET}/qml
qml_root.files = qml/main.qml

qml_cover.path = /usr/share/$${TARGET}/qml/cover
qml_cover.files = qml/cover/CoverPage.qml

qml_pages.path = /usr/share/$${TARGET}/qml/pages
qml_pages.files = \
    qml/pages/MainPage.qml \
    qml/pages/SaveDialog.qml \
    qml/pages/CommentDialog.qml \
    qml/pages/ExportDialog.qml \
    qml/pages/ImportDialog.qml

qml_comp.path = /usr/share/$${TARGET}/qml/components
qml_comp.files = \
    qml/components/Glossary.qml \
    qml/components/TermList.qml \
    qml/components/FrequencyBand.qml \
    qml/components/DataField.qml \
    qml/components/InfoBlock.qml \
    qml/components/PanelPulley.qml

qml_panels.path = /usr/share/$${TARGET}/qml/panels
qml_panels.files = \
    qml/panels/ReadPanel.qml \
    qml/panels/NdefPanel.qml \
    qml/panels/HealthCardPanel.qml \
    qml/panels/EidPanel.qml \
    qml/panels/PaymentPanel.qml \
    qml/panels/TransponderPanel.qml \
    qml/panels/OtherPanel.qml \
    qml/panels/EmulatePanel.qml \
    qml/panels/LogPanel.qml \
    qml/panels/ArchivePanel.qml

INSTALLS += qml_root qml_cover qml_pages qml_comp qml_panels

# ---- Shared assets -------------------------------------------------------
assets.path = /usr/share/$${TARGET}
assets.files = data/cover.png
INSTALLS += assets

# ---- um-only additions ---------------------------------------------------
CONFIG(um) {
    LIBS += -lpulse-simple -lpulse

    HEADERS += src/proxmark.h src/sfx.h
    SOURCES += src/proxmark.cpp src/sfx.cpp

    qml_um_root.path = /usr/share/$${TARGET}/qml
    qml_um_root.files = qml/UmHooks.qml

    qml_um_panels.path = /usr/share/$${TARGET}/qml/panels
    qml_um_panels.files = qml/panels/UltimatePanel.qml

    umassets.path = /usr/share/$${TARGET}
    umassets.files = data/pm3.png data/pm5.png data/glow-green.png data/explosion.wav

    INSTALLS += qml_um_root qml_um_panels umassets
}

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172 256x256

# Translation file follows the target name (SailfishApp loads <target>-<lang>).
TRANSLATIONS += translations/$${TARGET}-de.ts
