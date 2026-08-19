#include "cardstore.h"
#include "emulator.h"
#include "nfcbackend.h"
#ifdef UM_BUILD
#  include "proxmark.h"
#endif

#include <sailfishapp.h>

#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickView>
#include <QScopedPointer>

// APP_TARGET is passed in from the .pro (harbour-infc or harbour-infc-um) so
// storage, share paths and window all follow the built variant.
#ifndef APP_TARGET
#  define APP_TARGET "harbour-infc"
#endif

int main(int argc, char* argv[])
{
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    app->setApplicationName(QStringLiteral(APP_TARGET));
    app->setOrganizationName(QStringLiteral(APP_TARGET));

    NfcBackend nfc;
    Emulator emulator;
    CardStore store;

    QScopedPointer<QQuickView> view(SailfishApp::createView());
    QQmlContext* ctx = view->rootContext();
    ctx->setContextProperty(QStringLiteral("Nfc"), &nfc);
    ctx->setContextProperty(QStringLiteral("Emulator"), &emulator);
    ctx->setContextProperty(QStringLiteral("Store"), &store);

    // Where installed assets live for this variant.
    ctx->setContextProperty(QStringLiteral("appShareDir"),
        QStringLiteral("/usr/share/") + app->applicationName());

#ifdef UM_BUILD
    Proxmark proxmark;
    ctx->setContextProperty(QStringLiteral("Proxmark"), &proxmark);
    ctx->setContextProperty(QStringLiteral("umBuild"), true);
#else
    ctx->setContextProperty(QStringLiteral("umBuild"), false);
#endif

    // One shared entry QML for both variants (decoupled from TARGET name).
    view->setSource(SailfishApp::pathTo(QStringLiteral("qml/main.qml")));
    view->show();

    return app->exec();
}
