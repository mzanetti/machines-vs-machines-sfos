#ifndef ENEMIES_H
#define ENEMIES_H

#include <QAbstractListModel>

class Enemy;
class Board;
class Engine;

class Enemies : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum Roles {
        RoleImage,
        RoleImageBaseSize,
        RoleWalkingDirection,
        RoleTravelledDistance,
        RoleSpriteCount,
        RoleAnimationDuration,
        RoleRunning,
        RoleProgress,
        RoleX,
        RoleY,
        RoleNextX,
        RoleNextY,
        RoleCurrentField,
        RoleNextField,
        RoleSpeed,
        RoleEnergy,
        RoleMaxEnergy,
    };

    explicit Enemies(Engine *engine, QObject *parent = 0);

    int count() const { return rowCount(QModelIndex()); }
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    // The model takes ownership of the enemy
    void addEnemy(Enemy *enemy);

    Q_INVOKABLE Enemy *get(int index) const;

    void clear();

private slots:
    void itemRunningChanged();
    void itemProgressChanged();
    void itemCurrentFieldChanged();
    void itemSpeedChanged();
    void itemEnergyChanged();
    void itemWalkingDirectionChanged();

    void removeDeadEnemy();

signals:
    void countChanged();
    void enemyExited();

private:
    Engine *m_engine;
    QList<Enemy*> m_enemies;
    QList<Enemy*> m_deadEnemies;
};

class EnemyFactory
{
public:
    EnemyFactory(Engine *engine);
    void createTemplate(const QVariantMap &enemyMap);
    void clear();

    Enemy* createEnemy(int templateId);

    QList<int> templateIds();

private:
    class EnemyTemplate {
    public:
        QString image;
        int imageBaseSize;
        int spriteCount;
        int animationDuration;
        int speed;
        int energy;
        int reward;
    };

    Engine *m_engine;
    QHash<int, EnemyTemplate> m_enemyTemplates;
};

#endif // ENEMIES_H
