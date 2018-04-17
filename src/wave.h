#ifndef WAVE_H
#define WAVE_H

#include <QList>

class Wave
{
public:
    Wave(int interval, const QList<int> enemies);

    int interval() const;
    QList<int> enemies() const;
    void setEnemies(const QList<int> enemies);

private:
    int m_interval;
    QList<int> m_enemies;
};

#endif // WAVE_H
