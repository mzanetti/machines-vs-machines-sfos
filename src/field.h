#ifndef FIELD_H
#define FIELD_H

#include <QObject>

class Tower;
class Engine;

class Field : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x CONSTANT)
    Q_PROPERTY(int y READ y CONSTANT)

    Q_PROPERTY(bool isOnPath READ isOnPath CONSTANT)
    Q_PROPERTY(bool allowedForEnenmy READ allowedForEnemy CONSTANT)
    Q_PROPERTY(bool towerAllowed READ towerAllowed CONSTANT)
    Q_PROPERTY(QString color READ color CONSTANT)
    Q_PROPERTY(Tower* tower READ tower NOTIFY towerChanged)

public:
    explicit Field(Engine *engine, int x, int y, QObject *parent = 0);

    int x() const;
    int y() const;

    bool isOnPath() const;
    void setIsOnPath(bool isOnPath);

    bool allowedForEnemy() const;

    bool towerAllowed() const;
    void setTowerAllowed(bool towerAllowed);

    QString color() const;
    void setColor(const QString &color);

    Tower* tower() const;

    void addTower(Tower *tower);
    void destroyTower();

signals:
    void towerChanged();

private:
    Engine *m_engine;
    int m_x;
    int m_y;
    QString m_color;
    bool m_isOnPath;
    bool m_towerAllowed;
    Tower *m_tower;
};

#endif // FIELD_H
