#include "wave.h"

Wave::Wave(int interval, const QList<int> enemies):
    m_interval(interval),
    m_enemies(enemies)
{
}

int Wave::interval() const
{
    return m_interval;
}

QList<int> Wave::enemies() const
{
    return m_enemies;
}

void Wave::setEnemies(const QList<int> enemies)
{
    m_enemies = enemies;
}
