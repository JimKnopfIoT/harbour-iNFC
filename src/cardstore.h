/*
 * iNFC - persistence.
 *
 * Two separate directories on purpose:
 *
 *   cards/  what was actually on the tag, and nothing else
 *   notes/  your own comments about it
 *
 * They are never merged in memory either. Writing to a tag only ever draws
 * from cards/, so a comment cannot end up on a card by accident.
 */
#ifndef CARDSTORE_H
#define CARDSTORE_H

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>

class CardStore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList entries READ entries NOTIFY entriesChanged)
    Q_PROPERTY(int count READ count NOTIFY entriesChanged)

public:
    explicit CardStore(QObject* parent = Q_NULLPTR);

    QVariantList entries() const { return iEntries; }
    int count() const { return iEntries.count(); }

public slots:
    // Returns the new entry id, or an empty string on failure.
    QString save(const QVariantMap& card);
    QVariantMap load(const QString& id) const;
    void remove(const QString& id);

    // Both live in notes/, because both are written by you and must never
    // travel onto a card.
    QString comment(const QString& id) const;
    void setComment(const QString& id, const QString& text);
    QString label(const QString& id) const;
    void setLabel(const QString& id, const QString& text);

    // "Payment card 09.08.2026 08:20" - never nameless, always dated.
    QString suggestedLabel(const QString& kindLabel) const;

    void refresh();

    // Both return a status string for the UI.
    QString exportAll(const QString& fileName, bool includeNotes);
    QString exportDir() const;
    QString suggestedExportName() const;
    QString importFrom(const QString& path);

    QString storagePath() const;

signals:
    void entriesChanged();

private:
    QString dir(const QString& sub) const;
    bool writeJson(const QString& path, const QVariantMap& data) const;
    QVariantMap readJson(const QString& path) const;
    QString noteField(const QString& id, const QString& key) const;
    void setNoteField(const QString& id, const QString& key, const QString& text);

private:
    QVariantList iEntries;
};

#endif // CARDSTORE_H
