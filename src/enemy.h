#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QHash>

class QTimer;
class Board;
class Engine;

class Enemy : public QObject
{
    Q_OBJECT
    Q_ENUMS(TravelStatus)
    Q_ENUMS(WalkingDirection)
    Q_PROPERTY(QString image READ image NOTIFY imageChanged)
    Q_PROPERTY(int imageBaseSize READ imageBaseSize NOTIFY imageChanged)
    Q_PROPERTY(WalkingDirection walkingDirection READ walkingDirection NOTIFY walkingDirectionChanged)
    Q_PROPERTY(int spriteCount READ spriteCount NOTIFY imageChanged)
    Q_PROPERTY(int animationDuration READ animationDuration NOTIFY imageChanged)
    Q_PROPERTY(int x READ x NOTIFY currentFieldChanged)
    Q_PROPERTY(int y READ y NOTIFY currentFieldChanged)
    Q_PROPERTY(int currentField READ currentField NOTIFY currentFieldChanged)
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)

    Q_PROPERTY(int nextX READ nextX NOTIFY currentFieldChanged)
    Q_PROPERTY(int nextY READ nextY NOTIFY currentFieldChanged)
    Q_PROPERTY(int nextField READ nextField NOTIFY currentFieldChanged)

    Q_PROPERTY(bool running READ running NOTIFY runningChanged)
    Q_PROPERTY(int speed READ speed NOTIFY speedChanged)
    Q_PROPERTY(int energy READ energy NOTIFY energyChanged)
    Q_PROPERTY(int maxEnergy READ maxEnergy NOTIFY maxEnergyChanged)

public:
    enum TravelStatus {
        TravelStatusEntry = -1,
        TravelStatusExit = -2,
        TravelStatusNoWayFound = -3,
    };

    enum WalkingDirection {
        WalkingDirectionLeft,
        WalkingDirectionRight,
        WalkingDirectionUp,
        WalkingDirectionDown
    };

    explicit Enemy(Engine *engine);

    QString image() const;
    int imageBaseSize() const;
    int spriteCount() const;
    int animationDuration() const;
    void setImage(const QString &image, int baseSize, int spriteCount = 1, int animationDuration = 1);

    int x() const;
    int y() const;
    int currentField() const;

    int nextX() const;
    int nextY() const;
    int nextField() const;

    bool running() const;
    void setRunning(bool running);

    qreal progress() const;

    int speed() const;
    void setSpeed(int speed);

    void setEnergy(int energy);

    int reward() const;
    void setReward(int reward);

    int energy() const;
    int maxEnergy() const;

    Q_INVOKABLE qreal travelledDistance() const;

    WalkingDirection walkingDirection() const;

public slots:
    void advance();
    void hit(int damage, int slowDown, int delay);

signals:
    void imageChanged();
    void currentFieldChanged();
    void walkingDirectionChanged();
    void runningChanged();
    void progressChanged();
    void speedChanged();
    void energyChanged(int energy);
    void maxEnergyChanged();
    void exited();
    void died();

private:
    class Hit {
    public:
        int damage;
        int slowDown;
    };

private slots:
    void tick();
    void executeHit(const Hit &hit);

private:
    Engine *m_engine;
    Board *m_board;
    QString m_image;
    int m_imageBaseSize;
    WalkingDirection m_walkingDirection;
    int m_spriteCount;
    int m_animationDuration;
    bool m_running;
    qreal m_progress;
    qreal m_travelledDistance;
    int m_currentField;
    int m_nextField;
    int m_speed;
    int m_energy;
    int m_maxEnergy;
    int m_reward;

    // <ticks, slowdown>
    QList<QPair<int, int> > m_slowdowns;
    // <ticks, hit>
    QList<QPair<int, Hit> > m_hitList;
    QList<int> m_upcomingFields;
    QList<int> m_travelledFields;
};

#endif // ENEMY_H
