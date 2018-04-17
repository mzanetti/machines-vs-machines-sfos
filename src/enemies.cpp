#include "enemies.h"
#include "enemy.h"
#include "engine.h"
#include "levelpack.h"

#include <QTimer>

Enemies::Enemies(Engine *engine, QObject *parent) :
    QAbstractListModel(parent),
    m_engine(engine)
{
}

int Enemies::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_enemies.count();
}

QVariant Enemies::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case RoleImage:
        return m_enemies.at(index.row())->image();
    case RoleImageBaseSize:
        return m_enemies.at(index.row())->imageBaseSize();
    case RoleWalkingDirection:
        return m_enemies.at(index.row())->walkingDirection();
    case RoleTravelledDistance:
        return m_enemies.at(index.row())->travelledDistance();
    case RoleSpriteCount:
        return m_enemies.at(index.row())->spriteCount();
    case RoleAnimationDuration:
        return m_enemies.at(index.row())->animationDuration();
    case RoleRunning:
        return m_enemies.at(index.row())->running();
    case RoleProgress:
        return m_enemies.at(index.row())->progress();
    case RoleX:
        return m_enemies.at(index.row())->x();
    case RoleY:
        return m_enemies.at(index.row())->y();
    case RoleNextX:
        return m_enemies.at(index.row())->nextX();
    case RoleNextY:
        return m_enemies.at(index.row())->nextY();
    case RoleCurrentField:
        return m_enemies.at(index.row())->currentField();
    case RoleNextField:
        return m_enemies.at(index.row())->nextField();
    case RoleSpeed:
        return m_enemies.at(index.row())->speed();
    case RoleEnergy:
        return m_enemies.at(index.row())->energy();
    case RoleMaxEnergy:
        return m_enemies.at(index.row())->maxEnergy();
    }
    return QVariant();
}

QHash<int, QByteArray> Enemies::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(RoleImage, "image");
    roles.insert(RoleImageBaseSize, "imageBaseSize");
    roles.insert(RoleWalkingDirection, "walkingDirection");
    roles.insert(RoleTravelledDistance, "travelledDistance");
    roles.insert(RoleSpriteCount, "spriteCount");
    roles.insert(RoleAnimationDuration, "animationDuration");
    roles.insert(RoleRunning, "running");
    roles.insert(RoleProgress, "progress");
    roles.insert(RoleX, "x");
    roles.insert(RoleY, "y");
    roles.insert(RoleNextX, "nextX");
    roles.insert(RoleNextY, "nextY");
    roles.insert(RoleCurrentField, "currentField");
    roles.insert(RoleNextField, "nextField");
    roles.insert(RoleSpeed, "speed");
    roles.insert(RoleEnergy, "energy");
    roles.insert(RoleMaxEnergy, "maxEnergy");
    return roles;
}

void Enemies::addEnemy(Enemy *enemy)
{
    enemy->setParent(this);
    connect(enemy, &Enemy::runningChanged, this, &Enemies::itemRunningChanged);
    connect(enemy, &Enemy::progressChanged, this, &Enemies::itemProgressChanged);
    connect(enemy, &Enemy::currentFieldChanged, this, &Enemies::itemCurrentFieldChanged);
    connect(enemy, &Enemy::speedChanged, this, &Enemies::itemSpeedChanged);
    connect(enemy, &Enemy::energyChanged, this, &Enemies::itemEnergyChanged);
    connect(enemy, &Enemy::walkingDirectionChanged, this, &Enemies::itemWalkingDirectionChanged);
    beginInsertRows(QModelIndex(), m_enemies.count(), m_enemies.count());
    m_enemies.append(enemy);
    endInsertRows();
    emit countChanged();
}

Enemy *Enemies::get(int index) const
{
    if (index < 0 || index >= m_enemies.count()) {
        return nullptr;
    }
    return m_enemies.at(index);
}

void Enemies::clear()
{
    beginResetModel();
    qDeleteAll(m_enemies);
    m_enemies.clear();
    endResetModel();
}

void Enemies::itemRunningChanged()
{
    Enemy *enemy = static_cast<Enemy*>(sender());
    int idx = m_enemies.indexOf(enemy);
    emit dataChanged(index(idx), index(idx), QVector<int>() << RoleRunning);
}

void Enemies::itemProgressChanged()
{
    Enemy *enemy = static_cast<Enemy*>(sender());
    int idx = m_enemies.indexOf(enemy);
    emit dataChanged(index(idx), index(idx), QVector<int>() << RoleProgress << RoleTravelledDistance);
}

void Enemies::itemCurrentFieldChanged()
{
    Enemy *enemy = static_cast<Enemy*>(sender());
    int idx = m_enemies.indexOf(enemy);
    emit dataChanged(index(idx), index(idx), QVector<int>() << RoleX << RoleY << RoleNextX << RoleNextY << RoleCurrentField);

    if (enemy->nextField() == Enemy::TravelStatusExit) {
        m_deadEnemies.append(enemy);
        QTimer::singleShot(1000 / m_engine->gameSpeed(), this, SLOT(removeDeadEnemy()));

        emit enemyExited();
    }
}

void Enemies::itemSpeedChanged()
{
    Enemy *enemy = static_cast<Enemy*>(sender());
    int idx = m_enemies.indexOf(enemy);
    emit dataChanged(index(idx), index(idx), QVector<int>() << RoleSpeed);
}

void Enemies::itemEnergyChanged()
{
    Enemy *enemy = static_cast<Enemy*>(sender());
    int idx = m_enemies.indexOf(enemy);
    emit dataChanged(index(idx), index(idx), QVector<int>() << RoleEnergy);

    if (enemy->energy() <= 0) {
        m_deadEnemies.append(enemy);
        QTimer::singleShot(1000 / m_engine->gameSpeed(), this, SLOT(removeDeadEnemy()));
    }
}

void Enemies::itemWalkingDirectionChanged()
{
    Enemy *enemy = static_cast<Enemy*>(sender());
    int idx = m_enemies.indexOf(enemy);
    emit dataChanged(index(idx), index(idx), QVector<int>() << RoleWalkingDirection);
}

void Enemies::removeDeadEnemy()
{
    Enemy *enemy = m_deadEnemies.takeAt(0);
    if (!enemy || !m_enemies.contains(enemy)) {
        return;
    }
    int idx = m_enemies.indexOf(enemy);
    beginRemoveRows(QModelIndex(), idx, idx);
    m_enemies.takeAt(idx)->deleteLater();
    endRemoveRows();
    emit countChanged();
}


EnemyFactory::EnemyFactory(Engine *engine):
    m_engine(engine)
{

}

void EnemyFactory::createTemplate(const QVariantMap &enemyMap)
{
    if (!enemyMap.contains("id")) {
        qWarning() << "Cannot parse enemy. Field \"id\" is required.";
        return;
    }
    if (!enemyMap.contains("image")) {
        qWarning() << "Cannot parse enemy. Field \"id\" image is required.";
        return;
    }
    int id = enemyMap.value("id").toInt();
    EnemyTemplate et;
    et.image = enemyMap.value("image").toString();
    et.imageBaseSize = enemyMap.value("imageBaseSize", 256).toInt();
    et.spriteCount = enemyMap.value("spriteCount", 1).toInt();
    et.animationDuration = enemyMap.value("animationDuration", 1000).toInt();
    et.speed = enemyMap.value("speed", 1).toInt();
    et.energy = enemyMap.value("energy", 1).toInt();
    et.reward = enemyMap.value("reward").toInt();
    m_enemyTemplates.insert(id, et);
}

void EnemyFactory::clear()
{
    m_enemyTemplates.clear();
}

Enemy *EnemyFactory::createEnemy(int templateId)
{
    Enemy *enemy = new Enemy(m_engine);
    enemy->setSpeed(m_enemyTemplates.value(templateId).speed);
    enemy->setEnergy(m_enemyTemplates.value(templateId).energy);
    enemy->setReward(m_enemyTemplates.value(templateId).reward);
    QString basePath = m_engine->levelPack()->id() + "/enemies/";
    QString image = basePath + m_enemyTemplates.value(templateId).image;
    enemy->setImage(image, m_enemyTemplates.value(templateId).imageBaseSize, m_enemyTemplates.value(templateId).spriteCount, m_enemyTemplates.value(templateId).animationDuration);

    return enemy;
}

QList<int> EnemyFactory::templateIds()
{
    return m_enemyTemplates.keys();
}
