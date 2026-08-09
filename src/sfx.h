/*
 * iNFC - tiny sound effect player.
 *
 * QtMultimedia's QML module is not on this device, so we shell out to paplay
 * (PulseAudio is running in the user session). Fire and forget: a gag boom
 * should never block or fail loudly.
 */
#ifndef SFX_H
#define SFX_H

#include <QObject>
#include <QString>

class Sfx : public QObject
{
    Q_OBJECT
public:
    explicit Sfx(QObject* parent = Q_NULLPTR) : QObject(parent) {}

public slots:
    void play(const QString& file);
};

#endif // SFX_H
