#ifndef TOWER_H
#define TOWER_H

#include <QObject>
#include <QPoint>

class Board;
class Engine;
class Enemy;

class TowerConfig {
public:
    int cost;
    QString image;
    int imageBaseSize;
    int spriteCount;
    int startFrame;
    int animationDuration;
    QString shotImage;
    QString shotSound;
    QString shotType;
    int damage;
    int shotDuration;
    int shotLifetime;
    int shotRecovery;
    int shotCount;
    QPoint shotCenter;
    int shotStartDistance;
    qreal radius;
    int slowdown;
    bool locked;
    int unlockPoints;
    int destroyReward;
};

class Tower : public QObject
{
    Q_OBJECT
    Q_ENUMS(ShotType)

    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(int levels READ levels NOTIFY levelsChanged)
    Q_PROPERTY(int level READ level NOTIFY levelChanged)
    Q_PROPERTY(QString image READ image NOTIFY levelChanged)
    Q_PROPERTY(int imageBaseSize READ imageBaseSize NOTIFY levelChanged)
    Q_PROPERTY(int spriteCount READ spriteCount NOTIFY levelChanged)
    Q_PROPERTY(int startFrame READ startFrame NOTIFY levelChanged)
    Q_PROPERTY(int animationDuration READ animationDuration NOTIFY levelChanged)
    Q_PROPERTY(QString shotImage READ shotImage NOTIFY levelChanged)
    Q_PROPERTY(QString shotSound READ shotSound NOTIFY levelChanged)
    Q_PROPERTY(ShotType shotType READ shotType NOTIFY levelChanged)
    Q_PROPERTY(qreal radius READ radius NOTIFY levelChanged)
    Q_PROPERTY(int shotDuration READ shotDuration NOTIFY levelChanged)
    Q_PROPERTY(int shotLifetime READ shotLifetime NOTIFY levelChanged)
    Q_PROPERTY(bool canShoot READ canShoot NOTIFY levelChanged)
    Q_PROPERTY(int shotCount READ shotCount NOTIFY levelChanged)
    Q_PROPERTY(QPoint shotCenter READ shotCenter NOTIFY levelChanged)
    Q_PROPERTY(int shotStartDistance READ shotStartDistance NOTIFY levelChanged)
    Q_PROPERTY(int damage READ damage NOTIFY levelChanged)
    Q_PROPERTY(int slowdown READ slowdown NOTIFY levelChanged)
    Q_PROPERTY(bool charging READ charging NOTIFY chargingChanged)
    Q_PROPERTY(bool charged READ charged NOTIFY chargingChanged)

public:
    enum ShotType {
        ShotTypeSingle,
        ShotTypeBeam,
        ShotTypeRadial,
        ShotTypeCharge,
        ShotTypeAnimated
    };

    explicit Tower(int id, Engine *engine, QObject *parent = 0);
    void upgrade();

    int id() const;

    QString name() const;
    void setName(const QString &name);

    void setConfigs(const QList<TowerConfig> &configs);
    int levels() const;
    int level() const;

    QString image() const;
    int imageBaseSize() const;
    int spriteCount() const;
    int startFrame() const;
    int animationDuration() const;

    QString shotImage() const;
    QString shotSound() const;
    ShotType shotType() const;

    qreal radius() const;

    int shotDuration() const;
    int shotLifetime() const;
    bool canShoot() const;

    int shotCount() const;
    QPoint shotCenter() const;
    int shotStartDistance() const;

    int damage() const;

    int slowdown() const;

    QList<TowerConfig> configs();

    bool charging() const;
    bool charged() const;

public slots:
    void shoot(Enemy *enemy = nullptr);
    void charge();

signals:
    void nameChanged();
    void levelsChanged();
    void levelChanged();
    void chargingChanged();

private slots:
    void tick();
    void towerUnlocked(int id, int level);

private:
    int m_id;
    Engine *m_engine;
    QString m_name;
    int m_level;
    QList<TowerConfig> m_configs;
    int m_remainingRecoveryTicks;
    int m_remainingChargingTicks;
};

#endif // TOWER_H
