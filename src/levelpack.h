#ifndef LEVELPACK_H
#define LEVELPACK_H

#include <QObject>

#include "engine.h"
#include "enemies.h"
#

class Level;

class LevelPack : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString titleImage READ titleImage CONSTANT)
    Q_PROPERTY(QString copyright READ copyright CONSTANT)
    Q_PROPERTY(QString titleSound READ titleSound CONSTANT)
    Q_PROPERTY(int totalPoints READ totalPoints CONSTANT)

    Q_PROPERTY(int count READ rowCount CONSTANT)
    Q_PROPERTY(Enemies* enemies READ enemies CONSTANT)
    Q_PROPERTY(TowerFactory* towers READ towers CONSTANT)

public:
    enum Roles {
        RoleHighscore
    };

    explicit LevelPack(Engine *engine, const QString &id, const QString &name, const QString &titleImage, const QString &copyright, const QString &titleSound, QObject *parent = 0);

    QString id() const;
    QString name() const;
    QString titleImage() const;
    QString copyright() const;
    QString titleSound() const;
    int totalPoints() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE Level *get(int index) const;

    Enemies *enemies() const;
    TowerFactory *towers() const;

    Level *addLevel(const QVariantMap &levelMap);

    void clear();

    void init();

private slots:
    void levelHighscoreChanged();

private:
    Engine *m_engine;
    QString m_id;
    QString m_name;
    QString m_titleImage;
    QString m_copyright;
    QString m_titleSound;

    Enemies *m_enemies;

    QList<Level*> m_levels;

};

#endif // LEVELPACK_H
