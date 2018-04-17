#include "tower.h"
#include "board.h"
#include "engine.h"
#include "enemy.h"
#include "towerfactory.h"

Tower::Tower(int id, Engine *engine, QObject *parent) :
    QObject(parent),
    m_id(id),
    m_engine(engine),
    m_level(0),
    m_remainingRecoveryTicks(0),
    m_remainingChargingTicks(-1)
{
    connect(engine, &Engine::tick, this, &Tower::tick);
    connect(engine->towerFactory(), &TowerFactory::towerUnlocked, this, &Tower::towerUnlocked);
}

int Tower::id() const
{
    return m_id;
}

QString Tower::name() const
{
    return m_name;
}

void Tower::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
    }
}

void Tower::setConfigs(const QList<TowerConfig> &configs)
{
    m_configs = configs;
    emit levelsChanged();
    m_level = 0;
    emit levelChanged();
}

int Tower::levels() const
{
    return m_configs.count();
}

int Tower::level() const
{
    return m_level;
}

QString Tower::image() const
{
    return m_configs.at(m_level).image;
}

int Tower::imageBaseSize() const
{
    return m_configs.at(m_level).imageBaseSize;
}

int Tower::spriteCount() const
{
    return m_configs.at(m_level).spriteCount;
}

int Tower::startFrame() const
{
    return m_configs.at(m_level).startFrame;
}

int Tower::animationDuration() const
{
    return m_configs.at(m_level).animationDuration;
}

QString Tower::shotImage() const
{
    return m_configs.at(m_level).shotImage;
}

QString Tower::shotSound() const
{
    return m_configs.at(m_level).shotSound;
}

Tower::ShotType Tower::shotType() const
{
    QString shotType = m_configs.at(m_level).shotType;
    ShotType t = ShotTypeSingle;
    if (shotType == "beam") {
        t = ShotTypeBeam;
    } else if (shotType == "radial") {
        t = ShotTypeRadial;
    } else if (shotType == "charge") {
        t = ShotTypeCharge;
    } else if (shotType == "animated") {
        t = ShotTypeAnimated;
    }

    return t;
}

qreal Tower::radius() const
{
    return m_configs.at(m_level).radius;
}

int Tower::shotDuration() const
{
    return m_configs.at(m_level).shotDuration;
}

int Tower::shotLifetime() const
{
    return m_configs.at(m_level).shotLifetime;
}

bool Tower::canShoot() const
{
    return m_remainingRecoveryTicks <= 0;
}

int Tower::shotCount() const
{
    return m_configs.at(m_level).shotCount;
}

QPoint Tower::shotCenter() const
{
    return m_configs.at(m_level).shotCenter;
}

int Tower::shotStartDistance() const
{
    return m_configs.at(m_level).shotStartDistance;
}

int Tower::damage() const
{
    return m_configs.at(m_level).damage;
}

int Tower::slowdown() const
{
    return m_configs.at(m_level).slowdown;
}

QList<TowerConfig> Tower::configs()
{
    return m_configs;
}

void Tower::shoot(Enemy *enemy)
{
    if (enemy) {
        enemy->hit(m_configs.at(m_level).damage, m_configs.at(m_level).slowdown, shotDuration());
    }

    if (m_remainingRecoveryTicks <= 0) {
        // ticks : recovery = tps : 1000
        m_remainingRecoveryTicks = m_configs.at(m_level).shotRecovery * m_engine->ticksPerSecond() / 1000;
    }
    m_remainingChargingTicks = -1;
    emit chargingChanged();
}

void Tower::tick()
{
    if (m_remainingRecoveryTicks > 0) {
        m_remainingRecoveryTicks -= m_engine->gameSpeed();
    }
    if (m_remainingChargingTicks > 0) {
        m_remainingChargingTicks -= m_engine->gameSpeed();
        if (m_remainingChargingTicks <= 0) {
            m_remainingChargingTicks = 0;
            emit chargingChanged();
        }
    }
}

void Tower::towerUnlocked(int id, int level)
{
    if (id == m_id) {
        m_configs[level].locked = false;
        emit levelChanged();
    }
}

void Tower::upgrade()
{
    if (m_configs.count() > m_level+1) {
        m_level++;
        emit levelChanged();
    }
}

bool Tower::charging() const
{
    return m_remainingChargingTicks > 0;
}

void Tower::charge()
{
    qDebug() << "charging for" << m_configs.at(m_level).animationDuration;
    m_remainingChargingTicks = m_configs.at(m_level).animationDuration * m_engine->ticksPerSecond() / 1000;
}

bool Tower::charged() const
{
    return m_remainingChargingTicks == 0;
}
