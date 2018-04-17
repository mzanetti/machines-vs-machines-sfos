#include "field.h"
#include "tower.h"

Field::Field(Engine *engine, int x, int y, QObject *parent) :
    QObject(parent),
    m_engine(engine),
    m_x(x),
    m_y(y),
    m_color("darkgreen"),
    m_isOnPath(false),
    m_towerAllowed(true),
    m_tower(nullptr)
{
}

int Field::x() const
{
    return m_x;
}

int Field::y() const
{
    return m_y;
}

bool Field::isOnPath() const
{
    return m_isOnPath;
}

void Field::setIsOnPath(bool isOnPath)
{
    m_isOnPath = isOnPath;
}

bool Field::allowedForEnemy() const
{
    return m_isOnPath;
}

bool Field::towerAllowed() const
{
    return m_towerAllowed;
}

void Field::setTowerAllowed(bool towerAllowed)
{
    m_towerAllowed = towerAllowed;
}

QString Field::color() const
{
    return m_isOnPath ? "yellow" : "darkgreen";
}

void Field::setColor(const QString &color)
{
    m_color = color;
}

Tower *Field::tower() const
{
    return m_tower;
}

void Field::addTower(Tower *tower)
{
    Q_ASSERT(!m_tower);
    m_tower = tower;
    emit towerChanged();
}

void Field::destroyTower()
{
    if (m_tower) {
        m_tower->deleteLater();
        m_tower = 0;
        emit towerChanged();
    }
}
