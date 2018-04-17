#include "towerfactory.h"
#include "tower.h"
#include "engine.h"
#include "levelpack.h"
#include "settings.h"
#include <QDebug>
#include <QStandardPaths>

TowerFactory::TowerFactory(Engine *engine):
    m_engine(engine)
{
}

int TowerFactory::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_towerTemplates.count();
}

QVariant TowerFactory::data(const QModelIndex &index, int role) const
{
    return data(index, role, 0);
}

QVariant TowerFactory::data(const QModelIndex &index, int role, int level) const
{
    if (level >= m_towerTemplates.at(index.row()).configs.count()) {
        return QVariant();
    }
    switch (role) {
    case RoleId:
        return m_towerTemplates.at(index.row()).id;
    case RoleName:
        return m_towerTemplates.at(index.row()).name;
    case RoleLevels:
        return m_towerTemplates.at(index.row()).configs.count();
    case RoleCost:
        return m_towerTemplates.at(index.row()).configs.at(level).cost;
    case RoleImage:
        return m_towerTemplates.at(index.row()).configs.at(level).image;
    case RoleImageBaseSize:
        return m_towerTemplates.at(index.row()).configs.at(level).imageBaseSize;
    case RoleSpriteCount:
        return m_towerTemplates.at(index.row()).configs.at(level).spriteCount;
    case RoleStartFrame:
        return m_towerTemplates.at(index.row()).configs.at(level).startFrame;
    case RoleAnimationDuration:
        return m_towerTemplates.at(index.row()).configs.at(level).animationDuration;
    case RoleShotImage:
        return m_towerTemplates.at(index.row()).configs.at(level).shotImage;
    case RoleShotSound:
        return m_towerTemplates.at(index.row()).configs.at(level).shotSound;
    case RoleRadius:
        return m_towerTemplates.at(index.row()).configs.at(level).radius;
    case RoleShotDuration:
        return m_towerTemplates.at(index.row()).configs.at(level).shotDuration;
    case RoleShotLifetime:
        return m_towerTemplates.at(index.row()).configs.at(level).shotDuration;
    case RoleDamage:
        return m_towerTemplates.at(index.row()).configs.at(level).damage;
    case RoleSlowdown:
        return m_towerTemplates.at(index.row()).configs.at(level).slowdown;
    case RoleLocked:
        return m_towerTemplates.at(index.row()).configs.at(level).locked;
    case RoleUnlockPoints:
        return m_towerTemplates.at(index.row()).configs.at(level).unlockPoints;
    case RoleDestroyReward:
        return m_towerTemplates.at(index.row()).configs.at(level).destroyReward;
    }
    return QVariant();
}

QHash<int, QByteArray> TowerFactory::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(RoleId, "id");
    roles.insert(RoleName, "name");
    roles.insert(RoleLevels, "levels");
    roles.insert(RoleCost, "cost");
    roles.insert(RoleImage, "image");
    roles.insert(RoleImageBaseSize, "imageBaseSize");
    roles.insert(RoleSpriteCount, "spriteCount");
    roles.insert(RoleStartFrame, "startFrame");
    roles.insert(RoleAnimationDuration, "animationDuration");
    roles.insert(RoleShotImage, "shotImage");
    roles.insert(RoleShotSound, "shotSound");
    roles.insert(RoleRadius, "radius");
    roles.insert(RoleShotDuration, "shotDuration");
    roles.insert(RoleShotLifetime, "shotLifetime");
    roles.insert(RoleDamage, "damage");
    roles.insert(RoleSlowdown, "slowdown");
    roles.insert(RoleLocked, "locked");
    roles.insert(RoleUnlockPoints, "unlockPoints");
    roles.insert(RoleDestroyReward, "destroyReward");
    return roles;
}

void TowerFactory::clear()
{
    beginResetModel();
    m_towerTemplates.clear();
    endResetModel();
    emit countChanged();
}

QVariantMap TowerFactory::get(int index, int level) const
{
    QVariantMap map;
    if (index > m_towerTemplates.count()-1 || level > m_towerTemplates.at(index).configs.count() - 1) {
        return map;
    }
    map.insert("name", m_towerTemplates.at(index).name);
    TowerConfig config = m_towerTemplates.at(index).configs.at(level);
    map.insert("image", config.image);
    map.insert("imageBaseSize", config.imageBaseSize);
    map.insert("animationDuration", config.animationDuration);
    map.insert("spriteCount", config.spriteCount);
    map.insert("shotDuration", config.shotDuration);
    map.insert("shotRecovery", config.shotRecovery);
    map.insert("damage", config.damage);
    map.insert("slowdown", config.slowdown);
    map.insert("radius", config.radius);
    map.insert("cost", config.cost);
    map.insert("locked", config.locked);
    map.insert("unlockPoints", config.unlockPoints);
    map.insert("destroyReward", config.destroyReward);
    map.insert("index", index);
    map.insert("level", level);
    return map;
}

QVariantMap TowerFactory::getByTowerId(int towerId, int level) const
{
    for (int i = 0; i < m_towerTemplates.count(); i++) {
        if (m_towerTemplates.at(i).id == towerId) {
            return get(i, level);
        }
    }
    return QVariantMap();
}

QVariantList TowerFactory::getLineup(int index) const
{
    QVariantList list;
    if (index > m_towerTemplates.count()-1) {
        return list;
    }
    for (int i = 0; i < m_towerTemplates.at(index).configs.count(); i++) {
        list.append(get(index, i));
    }
    return list;
}

QVariantList TowerFactory::getLineupByTowerId(int towerId) const
{
    for (int i = 0; i < m_towerTemplates.count(); i++) {
        if (m_towerTemplates.at(i).id == towerId) {
            return getLineup(i);
        }
    }
    return QVariantList();
}

void TowerFactory::createTemplate(const QVariantMap &towerData)
{
    beginInsertRows(QModelIndex(), m_towerTemplates.count(), m_towerTemplates.count());
    TowerTemplate temp;
    temp.id = towerData.value("id").toInt();
    temp.name = towerData.value("name").toString();
    int index = 0;
    foreach (const QVariant &configVariant, towerData.value("configs").toList()) {
        TowerConfig config;
        QVariantMap configMap = configVariant.toMap();
        config.cost = configMap.value("cost", 0).toInt();
        config.image = m_engine->levelPack()->id() + "/towers/" + configMap.value("image").toString();
        config.imageBaseSize = configMap.value("imageBaseSize", 256).toInt();
        config.spriteCount = configMap.value("spriteCount", 1).toInt();
        config.startFrame = configMap.value("startFrame", 0).toInt();
        config.animationDuration = configMap.value("animationDuration", 1000).toInt();
        if (configMap.contains("shotImage")) {
            config.shotImage = m_engine->levelPack()->id() + "/towers/" + configMap.value("shotImage").toString();
        } else {
            config.shotImage = m_engine->levelPack()->id() + "/towers/shot-" + configMap.value("image").toString();
        }
        config.shotSound = m_engine->levelPack()->id() + "/towers/" + configMap.value("shotSound").toString();
        config.shotType = configMap.value("shotType", "single").toString();
        config.damage = configMap.value("damage", 0).toInt();
        config.radius = configMap.value("radius", 1).toReal();
        config.shotDuration = configMap.value("shotDuration", 200).toInt();
        config.shotRecovery = configMap.value("shotRecovery", 600).toInt();
        config.shotLifetime = configMap.value("shotLifetime", config.shotDuration).toInt();
        config.shotCount = configMap.value("shotCount", 1).toInt();
        if (configMap.contains("shotCenter")) {
            config.shotCenter = QPoint(configMap.value("shotCenter").toMap().value("x", 128).toInt(), configMap.value("shotCenter").toMap().value("y", 32).toInt());
        } else {
            config.shotCenter = QPoint(128, 32);
        }
        config.shotStartDistance = configMap.value("shotStartDistance", 0).toInt();
        config.slowdown = configMap.value("slowdown", 0).toInt();
        config.locked = configMap.value("locked", false).toBool();
        if (config.locked) {
            {
                SETTINGS("towers");
                settings.beginGroup(m_engine->levelPack()->id());
                settings.beginGroup(QString::number(m_engine->difficulty()));
                settings.beginGroup(QString::number(temp.id));
                settings.beginGroup(QString::number(index));
                config.locked = settings.value("locked", config.locked).toBool();
            }
            {
                SETTINGS("global");
                if (settings.value("allUnlocked", false).toBool()) {
                    config.locked = false;
                }
            }
        }
        config.unlockPoints = configMap.value("unlockPoints", 0).toInt();
        config.destroyReward = configMap.value("destroyReward", 0).toInt();
        temp.configs.append(config);
        index++;
    }

    m_towerTemplates.append(temp);
    endInsertRows();
    emit countChanged();
}

int TowerFactory::cost(int index) const
{
    return m_towerTemplates.at(index).configs.at(0).cost;
}

Tower *TowerFactory::createTower(int index)
{
    TowerTemplate temp = m_towerTemplates.value(index);
    Tower *tower = new Tower(temp.id, m_engine);
    tower->setConfigs(m_towerTemplates.value(index).configs);
    qDebug() << "created tower" << tower->configs().at(1).unlockPoints;
    return tower;
}

bool TowerFactory::unlockTower(int index, int level)
{
    if (index < 0 || index >= m_towerTemplates.count()) {
        return false;
    }
    if (level < 0 || level >= m_towerTemplates[index].configs.count()) {
        return false;
    }
    m_towerTemplates[index].configs[level].locked = false;
    emit dataChanged(this->index(index), this->index(index), QVector<int>() << RoleLocked);

    SETTINGS("towers");
    settings.beginGroup(m_engine->levelPack()->id());
    settings.beginGroup(QString::number(m_engine->difficulty()));
    settings.beginGroup(QString::number(m_towerTemplates.at(index).id));
    settings.beginGroup(QString::number(level));
    settings.setValue("locked", false);
    emit towerUnlocked(m_towerTemplates[index].id, level);
    return true;
}


