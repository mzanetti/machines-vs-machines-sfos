#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QtQml>

class LevelPacks;
class LevelPack;
class Board;
class Enemies;
class Enemy;
class EnemyFactory;
class TowerFactory;
class Wave;
class Tower;

class QTimer;

class Engine : public QObject
{
    Q_OBJECT
    Q_ENUMS(Difficulty)
    Q_ENUMS(PlayStatus)

    Q_PROPERTY(Difficulty difficulty READ difficulty WRITE setDifficulty NOTIFY difficultyChanged)
    Q_PROPERTY(QUrl dataDir READ dataDir WRITE setDataDir NOTIFY dataDirChanged)
    Q_PROPERTY(LevelPacks* levelPacks READ levelPacks NOTIFY levelPacksChanged)
    Q_PROPERTY(LevelPack* levelPack READ levelPack NOTIFY levelPackChanged)
    Q_PROPERTY(int levelCount READ levelCount NOTIFY levelCountChanged)
    Q_PROPERTY(int highestUnlockedLevel READ highestUnlockedLevel NOTIFY highestUnlockedLevelChanged)
    Q_PROPERTY(int points READ points NOTIFY pointsChanged)
    Q_PROPERTY(int totalPoints READ totalPoints NOTIFY pointsChanged)
    Q_PROPERTY(int lives READ lives NOTIFY livesChanged)
    Q_PROPERTY(int level READ level NOTIFY levelChanged)
    Q_PROPERTY(PlayStatus playStatus READ playStatus NOTIFY playStatusChanged)
    Q_PROPERTY(Board* board READ board CONSTANT)
    Q_PROPERTY(int earnedPoints READ earnedPoints NOTIFY playStatusChanged)
    Q_PROPERTY(int waves READ waves NOTIFY wavesChanged)
    Q_PROPERTY(int currentWave READ currentWave NOTIFY currentWaveChanged)
    Q_PROPERTY(int money READ money NOTIFY moneyChanged)
    Q_PROPERTY(int gameSpeed READ gameSpeed WRITE setGameSpeed NOTIFY gameSpeedChanged)
    Q_PROPERTY(TowerFactory* availableTowers READ towerFactory NOTIFY availableTowersChanged)
    Q_PROPERTY(int secsToNextWave READ secsToNextWave NOTIFY secsToNextWaveChanged)

    Q_PROPERTY(int ticksPerSecond READ ticksPerSecond CONSTANT)

public:
    enum Difficulty {
        DifficultyEasy,
        DifficultyMedium,
        DifficultyHard,
    };

    enum PlayStatus {
        PlayStatusNotStarted,
        PlayStatusRunning,
        PlayStatusLost,
        PlayStatusWon,
        PlayStatusPaused
    };
    explicit Engine(QObject *parent = 0);
    ~Engine();

public:
    Difficulty difficulty() const;
    void setDifficulty(Difficulty difficulty);

    QUrl dataDir() const;
    void setDataDir(const QUrl &dataDir);

    LevelPacks* levelPacks();

    LevelPack* levelPack() const;

    int levelCount() const;

    int highestUnlockedLevel() const;

    int points() const;
    int totalPoints() const;

    int lives() const;
    int level() const;
    PlayStatus playStatus() const;
    int earnedPoints() const;

    int money() const;

    Board* board() const;

    int waves() const;
    int currentWave() const;
    int secsToNextWave() const;

    int gameSpeed() const;
    void setGameSpeed(int gameSpeed);
    qreal stepSize() const;

    TowerFactory* towerFactory() const;
    EnemyFactory* enemyFactory() const;

    int ticksPerSecond();

public slots:
    void loadLevelPack(const QString &levelPackId);
    void unloadLevelPack();
    void unlockTower(int availableTowersIndex, int level);
    void startNewGame(int level);
    void pauseGame();
    void resumeGame();
    void stopGame();
    void restartLevel();
    bool addTower(int fieldIndex, int towerTemplateId);
    void upgradeTower(Tower *tower);
    void destroyTower(int fieldIndex);
    void resetScores();

signals:
    void difficultyChanged();
    void dataDirChanged();
    void levelPacksChanged();
    void levelPackChanged();
    void levelCountChanged();
    void pointsChanged();
    void livesChanged();
    void levelChanged();
    void highestUnlockedLevelChanged();
    void playStatusChanged();
    void wavesChanged();
    void currentWaveChanged();
    void secsToNextWaveChanged();

    void moneyChanged();
    void tick();
    void gameSpeedChanged();
    void availableTowersChanged();

private slots:
    void slotTick();

    void nextWave();
    void nextEnemy();
    void liveLost();

    void enemyCountChanged();
    void enemyDied();

private:
    QString levelFile(int level);
    bool loadLevel(int index);
    void loadEnemies();
    void loadTowers();
    void loadLevels();

    void loadDifficulty();
    void saveDifficulty();

    void savePoints();
    void loadPoints();

    void setHighestUnlockedLevel(int hightestLevel);

private:
    Difficulty m_difficulty;
    QUrl m_dataDir;
    LevelPacks *m_levelPacks;
    LevelPack *m_levelPack;
    int m_points;
    int m_totalPoints;
    int m_lives;
    int m_levelCount;
    int m_level;
    PlayStatus m_playStatus;
    int m_earnedPoints;

    Board *m_board;
    EnemyFactory *m_enemyFactory;
    TowerFactory *m_towerFactory;
    QList<Wave> m_waves;
    int m_currentWave;
    QList<Enemy*> m_waitingEnemies;

    int m_money;

    int m_ticksPerSecond;
    QTimer *m_timer;
    int m_gameSpeed;
    int m_ticksToNextWave;
    int m_ticksToNextEnemy;
};

#endif // ENGINE_H
