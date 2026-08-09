#include "cardstore.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

CardStore::CardStore(QObject* parent) :
    QObject(parent)
{
    QDir().mkpath(dir(QStringLiteral("cards")));
    QDir().mkpath(dir(QStringLiteral("notes")));
    refresh();
}

QString CardStore::storagePath() const
{
    // Keyed on the application name so the base app and the uv build keep
    // separate archives (harbour-infc vs harbour-infc-uv).
    return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
        + QLatin1Char('/') + QCoreApplication::applicationName();
}

QString CardStore::dir(const QString& sub) const
{
    return storagePath() + QLatin1Char('/') + sub;
}

bool CardStore::writeJson(const QString& path, const QVariantMap& data) const
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;
    f.write(QJsonDocument(QJsonObject::fromVariantMap(data)).toJson());
    f.close();
    return true;
}

QVariantMap CardStore::readJson(const QString& path) const
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) return QVariantMap();
    const QByteArray raw = f.readAll();
    f.close();
    return QJsonDocument::fromJson(raw).object().toVariantMap();
}

QString CardStore::save(const QVariantMap& card)
{
    const QString stamp = QDateTime::currentDateTime()
        .toString(QStringLiteral("yyyyMMdd-hhmmss"));
    QString uid = card.value(QStringLiteral("uid")).toString();
    uid.remove(QLatin1Char(':'));
    if (uid.isEmpty()) uid = QStringLiteral("nouid");
    const QString id = uid + QLatin1Char('-') + stamp;

    QVariantMap data = card;
    data.insert(QStringLiteral("id"), id);
    data.insert(QStringLiteral("saved"), QDateTime::currentDateTime().toString(Qt::ISODate));

    if (!writeJson(dir(QStringLiteral("cards")) + QLatin1Char('/') + id + QStringLiteral(".json"), data)) {
        return QString();
    }
    refresh();
    return id;
}

QVariantMap CardStore::load(const QString& id) const
{
    return readJson(dir(QStringLiteral("cards")) + QLatin1Char('/') + id + QStringLiteral(".json"));
}

void CardStore::remove(const QString& id)
{
    QFile::remove(dir(QStringLiteral("cards")) + QLatin1Char('/') + id + QStringLiteral(".json"));
    QFile::remove(dir(QStringLiteral("notes")) + QLatin1Char('/') + id + QStringLiteral(".json"));
    refresh();
}

// Label and comment share one note file, so writing either must preserve
// the other.
void CardStore::setNoteField(const QString& id, const QString& key,
    const QString& text)
{
    const QString path = dir(QStringLiteral("notes")) + QLatin1Char('/') + id + QStringLiteral(".json");
    QVariantMap m = readJson(path);
    m.insert(QStringLiteral("id"), id);
    m.insert(key, text);
    m.insert(QStringLiteral("edited"), QDateTime::currentDateTime().toString(Qt::ISODate));

    const bool empty = m.value(QStringLiteral("comment")).toString().isEmpty() &&
                       m.value(QStringLiteral("label")).toString().isEmpty();
    if (empty) {
        QFile::remove(path);
    } else {
        writeJson(path, m);
    }
    refresh();
}

QString CardStore::noteField(const QString& id, const QString& key) const
{
    return readJson(dir(QStringLiteral("notes")) + QLatin1Char('/') + id + QStringLiteral(".json"))
        .value(key).toString();
}

QString CardStore::comment(const QString& id) const
{
    return noteField(id, QStringLiteral("comment"));
}

void CardStore::setComment(const QString& id, const QString& text)
{
    setNoteField(id, QStringLiteral("comment"), text);
}

QString CardStore::label(const QString& id) const
{
    return noteField(id, QStringLiteral("label"));
}

void CardStore::setLabel(const QString& id, const QString& text)
{
    setNoteField(id, QStringLiteral("label"), text);
}

QString CardStore::suggestedLabel(const QString& kindLabel) const
{
    const QString when = QDateTime::currentDateTime()
        .toString(QStringLiteral("dd.MM.yyyy HH:mm"));
    return kindLabel.isEmpty() ? when
        : (kindLabel + QLatin1Char(' ') + when);
}

void CardStore::refresh()
{
    iEntries.clear();
    QDir d(dir(QStringLiteral("cards")));
    const QStringList files = d.entryList(QStringList() << QStringLiteral("*.json"),
        QDir::Files, QDir::Name | QDir::Reversed);
    for (int i = 0; i < files.count(); i++) {
        const QVariantMap card = readJson(d.filePath(files.at(i)));
        if (card.isEmpty()) continue;
        QVariantMap e;
        const QString id = card.value(QStringLiteral("id")).toString();
        e.insert(QStringLiteral("id"), id);
        e.insert(QStringLiteral("uid"), card.value(QStringLiteral("uid")));
        e.insert(QStringLiteral("kind"), card.value(QStringLiteral("kind")));
        e.insert(QStringLiteral("kindLabel"), card.value(QStringLiteral("kindLabel")));
        e.insert(QStringLiteral("saved"), card.value(QStringLiteral("saved")));
        e.insert(QStringLiteral("comment"), comment(id));
        e.insert(QStringLiteral("label"), label(id));
        iEntries.append(e);
    }
    emit entriesChanged();
}

QString CardStore::exportDir() const
{
    return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
}

QString CardStore::suggestedExportName() const
{
    return QStringLiteral("infc-export-")
        + QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd-hhmmss"))
        + QStringLiteral(".json");
}

QString CardStore::exportAll(const QString& fileName, bool includeNotes)
{
    QVariantList cards;
    QDir d(dir(QStringLiteral("cards")));
    const QStringList files = d.entryList(QStringList() << QStringLiteral("*.json"), QDir::Files, QDir::Name);
    for (int i = 0; i < files.count(); i++) {
        QVariantMap card = readJson(d.filePath(files.at(i)));
        if (includeNotes) {
            const QString c = comment(card.value(QStringLiteral("id")).toString());
            if (!c.isEmpty()) card.insert(QStringLiteral("comment"), c);
        }
        cards.append(card);
    }

    QJsonObject root;
    root.insert(QStringLiteral("application"), QStringLiteral("harbour-infc"));
    root.insert(QStringLiteral("exported"), QDateTime::currentDateTime().toString(Qt::ISODate));
    root.insert(QStringLiteral("notesIncluded"), includeNotes);
    root.insert(QStringLiteral("cards"), QJsonArray::fromVariantList(cards));

    QString name = fileName.trimmed();
    if (name.isEmpty()) name = suggestedExportName();
    // Keep it a file name, not a path: the target directory is fixed so a
    // typo cannot scatter exports across the filesystem.
    name = name.section(QLatin1Char('/'), -1);
    if (!name.endsWith(QStringLiteral(".json"))) {
        name += QStringLiteral(".json");
    }

    const QString out = exportDir() + QLatin1Char('/') + name;
    QDir().mkpath(exportDir());

    QFile f(out);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return tr("Export failed: cannot write %1").arg(out);
    }
    f.write(QJsonDocument(root).toJson());
    f.close();
    return tr("Exported %n card(s) to %1", "", cards.count()).arg(out);
}

QString CardStore::importFrom(const QString& path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        return tr("Import failed: cannot read %1").arg(path);
    }
    const QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    f.close();

    const QJsonArray cards = doc.object().value(QStringLiteral("cards")).toArray();
    int n = 0;
    for (int i = 0; i < cards.count(); i++) {
        QVariantMap card = cards.at(i).toObject().toVariantMap();
        const QString comment = card.take(QStringLiteral("comment")).toString();
        const QString id = save(card);
        if (!id.isEmpty()) {
            n++;
            if (!comment.isEmpty()) setComment(id, comment);
        }
    }
    refresh();
    return tr("Imported %n card(s)", "", n);
}
