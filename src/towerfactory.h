#ifndef TOWERFACTORY_H
#define TOWERFACTORY_H

#include "tower.h"

#include <QAbstractListModel>
#include <QVariantMap>
#include <QColor>

class Engine;

class TowerFactory: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum Roles {
        RoleId,
        RoleName,
        RoleCost,
        RoleLevels,
        RoleImage,
        RoleImageBaseSize,
        RoleSpriteCount,
        RoleStartFrame,
        RoleAnimationDuration,
        RoleShotImage,
        RoleShotSound,
        RoleRadius,
        RoleShotDuration,
        RoleShotLifetime,
        RoleDamage,
        RoleSlowdown,
        RoleLocked,
        RoleUnlockPoints,
        RoleDestroyReward
    };

    TowerFactory(Engine *engine);

    void createTemplate(const QVariantMap &towerData);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant data(const QModelIndex &index, int role, int level) const;
    QHash<int, QByteArray> roleNames() const;
    void clear();
    Q_INVOKABLE QVariantMap get(int index, int level) const;
    Q_INVOKABLE QVariantMap getByTowerId(int towerId, int level) const;
    Q_INVOKABLE QVariantList getLineup(int index) const;
    Q_INVOKABLE QVariantList getLineupByTowerId(int towerId) const;
    int count() const { return rowCount(QModelIndex()); }

    int cost(int index) const;
    Tower *createTower(int index);

    bool unlockTower(int index, int level);

signals:
    void countChanged();
    void towerUnlocked(int towerId, int level);

private:
    class TowerTemplate {
    public:
        int id;
        QString name;
        QList<TowerConfig> configs;
    };

    Engine *m_engine;
    QList<TowerTemplate> m_towerTemplates;
};

#endif // TOWERFACTORY_H
