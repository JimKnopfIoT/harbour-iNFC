#include "sfx.h"

#include <QDebug>
#include <QFile>

#include <pulse/error.h>
#include <pulse/simple.h>

#include <thread>

// Play PCM straight to PulseAudio, in-process. Sailjail blocks spawning
// paplay/gst, so a subprocess is out; libpulse-simple is present and works
// inside the sandbox once the Audio permission is granted.
static void playPcm(QByteArray pcm)
{
    // On this device the default role (x-maemo) routes to a silent path;
    // playing explicitly to the primary output is the one that is audible.
    // Confirmed by probing every role/sink on the device.
    qputenv("PULSE_PROP", "media.role=event");

    pa_sample_spec ss;
    ss.format = PA_SAMPLE_S16LE;   // WAV we ship: 44100 Hz mono S16 (pa resamples)
    ss.rate = 44100;
    ss.channels = 1;

    int err = 0;
    // Target the primary output by name; fall back to the default sink if that
    // name is ever absent (e.g. a different audio route).
    pa_simple* s = pa_simple_new(Q_NULLPTR, "harbour-infc", PA_STREAM_PLAYBACK,
        "sink.primary_output", "ultimate", &ss, Q_NULLPTR, Q_NULLPTR, &err);
    if (!s) {
        s = pa_simple_new(Q_NULLPTR, "harbour-infc", PA_STREAM_PLAYBACK,
            Q_NULLPTR, "ultimate", &ss, Q_NULLPTR, Q_NULLPTR, &err);
    }
    if (!s) {
        qWarning() << "[sfx] pa_simple_new failed:" << pa_strerror(err);
        return;
    }
    if (pa_simple_write(s, pcm.constData(), (size_t)pcm.size(), &err) < 0) {
        qWarning() << "[sfx] pa_simple_write failed:" << pa_strerror(err);
    }
    pa_simple_drain(s, &err);
    pa_simple_free(s);
    qWarning() << "[sfx] played" << pcm.size() << "bytes";
}

void Sfx::play(const QString& file)
{
    QFile f(file);
    if (!f.open(QIODevice::ReadOnly)) {
        qWarning() << "[sfx] cannot open" << file;
        return;
    }
    const QByteArray wav = f.readAll();
    f.close();

    // Skip the 44-byte canonical WAV header - this is our own file, so the
    // layout is fixed.
    if (wav.size() <= 44) return;
    const QByteArray pcm = wav.mid(44);

    // Detached so the 0.75 s playback never blocks the UI thread.
    std::thread(playPcm, pcm).detach();
}
