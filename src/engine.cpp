#include "engine.h"
#include "levelpacks.h"
#include "levelpack.h"
#include "board.h"
#include "enemies.h"
#include "enemy.h"
#include "field.h"
#include "wave.h"
#include "towerfactory.h"
#include "tower.h"
#include "level.h"
#include "settings.h"

#include <QTimer>
#include <QCoreApplication>

Engine::Engine(QObject *parent) :
    QObject(parent),
    m_levelPacks(nullptr),
    m_levelPack(nullptr),
    m_points(0),
    m_totalPoints(0),
    m_lives(10),
    m_levelCount(0),
    m_level(1),
    m_playStatus(PlayStatusNotStarted),
    m_earnedPoints(0),
    m_board(new Board(this)),
    m_enemyFactory(new EnemyFactory(this)),
    m_towerFactory(new TowerFactory(this)),
    m_currentWave(0),
    m_money(0),
    m_ticksPerSecond(25),
    m_timer(new QTimer(this)),
    m_gameSpeed(1),
    m_ticksToNextWave(-1),
    m_ticksToNextEnemy(-1)
{
    m_timer->setInterval(1000 / m_ticksPerSecond);
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, &QTimer::timeout, this, &Engine::slotTick);

    connect(m_board->enemies(), &Enemies::countChanged, this, &Engine::enemyCountChanged);

    loadDifficulty();
}

Engine::~Engine()
{
    delete m_enemyFactory;
    delete m_towerFactory;
}

Engine::Difficulty Engine::difficulty() const
{
    return m_difficulty;
}

void Engine::setDifficulty(Engine::Difficulty difficulty)
{
    qDebug() << "setting difficulty" << difficulty;
    if (m_difficulty != difficulty) {
        m_difficulty = difficulty;
        saveDifficulty();

        emit pointsChanged();
        emit highestUnlockedLevelChanged();
    }
}

QUrl Engine::dataDir() const
{
    return m_dataDir;
}

void Engine::setDataDir(const QUrl &dataDir)
{
    m_dataDir = dataDir;
    emit dataDirChanged();
    if (m_levelPacks) {
        m_levelPacks->deleteLater();
    }
    m_levelPacks = new LevelPacks(this, dataDir);
    emit levelPacksChanged();
}

LevelPacks *Engine::levelPacks()
{
    return m_levelPacks;
}

int Engine::lives() const
{
    return m_lives;
}

int Engine::level() const
{
    return m_level;
}

Engine::PlayStatus Engine::playStatus() const
{
    return m_playStatus;
}

int Engine::earnedPoints() const
{
    return m_earnedPoints;
}

int Engine::money() const
{
    return m_money;
}

Board *Engine::board() const
{
    return m_board;
}

int Engine::waves() const
{
    return m_waves.count();
}

int Engine::currentWave() const
{
    return m_currentWave;
}

int Engine::secsToNextWave() const
{
    return m_ticksToNextWave / m_ticksPerSecond;
}

int Engine::gameSpeed() const
{
    return m_gameSpeed;
}

void Engine::setGameSpeed(int gameSpeed)
{
    if (gameSpeed > 0 && m_gameSpeed != gameSpeed) {
        m_gameSpeed = gameSpeed;
        emit gameSpeedChanged();
    }
}

qreal Engine::stepSize() const
{
    return 1.0 / (50 * m_ticksPerSecond / 33);
}

TowerFactory* Engine::towerFactory() const
{
    return m_towerFactory;
}

EnemyFactory *Engine::enemyFactory() const
{
    return m_enemyFactory;
}

int Engine::ticksPerSecond()
{
    return m_ticksPerSecond;
}

void Engine::unlockTower(int availableTowersIndex, int level)
{
    int unlockPoints = m_towerFactory->data(m_towerFactory->index(availableTowersIndex), TowerFactory::RoleUnlockPoints, level).toInt();
    qDebug() << "unlocking tower points" << unlockPoints << m_points << availableTowersIndex << level;
    if (m_points >= unlockPoints) {
        m_points -= unlockPoints;
        m_towerFactory->unlockTower(availableTowersIndex, level);
        emit pointsChanged();
        savePoints();
    }
}

int Engine::levelCount() const
{
    return m_levelCount;
}

int Engine::points() const
{
    return m_points;
}

int Engine::totalPoints() const
{
    return m_totalPoints;
}

LevelPack *Engine::levelPack() const
{
    return m_levelPack;
}

void Engine::loadLevelPack(const QString &levelPackId)
{
    if (m_playStatus != PlayStatusNotStarted) {
        stopGame();
    }

    m_levelPack = m_levelPacks->levelPack(levelPackId);
    qDebug() << "setting levelpack" << levelPackId;

    if (!m_levelPack) {
        emit levelPackChanged();
        return;
    }

    m_levelCount = 0;
    while (QFileInfo(levelFile(m_levelCount+1)).exists()) {
        m_levelCount++;
    }
    emit levelCountChanged();


    if (m_levelPack->rowCount(QModelIndex()) > 0) {
        m_levelPack->clear();
    }
    loadLevels();

    loadPoints();
    loadEnemies();
    loadTowers();
    emit highestUnlockedLevelChanged();

    m_levelPack->init();

    emit levelPackChanged();
}

void Engine::unloadLevelPack()
{
    m_levelPack = nullptr;
    emit levelPackChanged();
}

bool Engine::addTower(int fieldIndex, int towerTemplateId)
{
    if (m_money < m_towerFactory->cost(towerTemplateId)) {
        qWarning() << "not enough money. sorry";
        return false;
    }

    Field *field = m_board->field(fieldIndex);
    if (!field) {
        qWarning() << "No such field" << fieldIndex;
        return false;
    }

    if (field->isOnPath()) {
        qWarning() << "Cannot create tower on path";
        return false;
    }
    if (field->tower()) {
        qWarning() << "Field already has a tower";
        return false;
    }

    m_money -= m_towerFactory->cost(towerTemplateId);
    emit moneyChanged();

    field->addTower(m_towerFactory->createTower(towerTemplateId));
    return true;
}

void Engine::upgradeTower(Tower *tower)
{
    qDebug() << "should upgrade tower" << tower->level() << tower->levels() << tower->configs().at(1).locked;
    if (tower->level() >= tower->levels() - 1) {
        qWarning() << "Can't upgrade this tower further";
        return;
    }
    SETTINGS("global");
    if (tower->configs().at(tower->level()+1).locked) {
        qWarning() << "Can't upgrade this tower. Next level is locked.";
        return;
    }
    if (m_money < tower->configs().at(tower->level()+1).cost) {
        qWarning() << "Not enough money...";
        return;
    }
    m_money -= tower->configs().at(tower->level() + 1).cost;
    emit moneyChanged();
    tower->upgrade();
}

void Engine::destroyTower(int fieldIndex)
{
    Field *field = m_board->field(fieldIndex);
    if (field->tower()) {
        m_money += field->tower()->configs().at(field->tower()->level()).destroyReward;
        emit moneyChanged();
        field->destroyTower();
    }
}

void Engine::resetScores()
{
    if (m_levelPack) {
        {
            SETTINGS("scores");
            settings.beginGroup(m_levelPack->id());
            settings.beginGroup(QString::number(m_difficulty));
            settings.remove("");
            m_points = 0;
            m_totalPoints = 0;
            emit pointsChanged();
            emit highestUnlockedLevelChanged();
        }
        {
            SETTINGS("towers");
            settings.beginGroup(m_levelPack->id());
            settings.beginGroup(QString::number(m_difficulty));
            settings.remove("");
            loadTowers();
        }
    } else {
        Settings s;
        s.resetScores();
    }
}

void Engine::slotTick()
{
    if (m_ticksToNextWave > 0) {
        int oldSecsToNextWave = secsToNextWave();
        m_ticksToNextWave = qMax(0, m_ticksToNextWave - m_gameSpeed);
        if (secsToNextWave() < oldSecsToNextWave) {
            emit secsToNextWaveChanged();
        }
    }
    if (m_ticksToNextEnemy > 0) {
        m_ticksToNextEnemy = qMax(0, m_ticksToNextEnemy - m_gameSpeed);
    }

    if (m_ticksToNextWave == 0) {
        m_ticksToNextWave = -1;
        nextWave();
    }
    if (m_ticksToNextEnemy == 0) {
        m_ticksToNextEnemy = -1;
        nextEnemy();
    }
    emit tick();
}

void Engine::startNewGame(int level)
{
    if (highestUnlockedLevel() < level) {
        qWarning() << "Can't start level" << level << ". Not unlocked yet.";
        return;
    }

    m_gameSpeed = 1;
    emit gameSpeedChanged();

    m_money = 0;
    emit moneyChanged();

    m_lives = 10;
    emit livesChanged();

    m_currentWave = -1;
    emit currentWaveChanged();

    m_level = level;
    if (!loadLevel(m_level)) {
        return;
    }
    emit levelChanged();

    m_timer->start();
    m_playStatus = PlayStatusRunning;
    emit playStatusChanged();

    emit wavesChanged();
    m_ticksToNextEnemy = -1;
    m_ticksToNextWave = 6 * m_ticksPerSecond;
    emit secsToNextWaveChanged();
}

void Engine::pauseGame()
{
    m_playStatus = PlayStatusPaused;
    m_timer->stop();
    emit playStatusChanged();
}

void Engine::resumeGame()
{
    m_playStatus = PlayStatusRunning;
    m_timer->start();
    emit playStatusChanged();
}

void Engine::stopGame()
{
    m_timer->stop();
    m_board->enemies()->clear();
    m_waitingEnemies.clear();
    m_earnedPoints = 0;
    m_playStatus = PlayStatusNotStarted;
    emit playStatusChanged();
}

void Engine::restartLevel()
{
    stopGame();
    startNewGame(m_level);
}

void Engine::nextWave()
{
    m_currentWave++;
    emit currentWaveChanged();
    Wave wave = m_waves.at(m_currentWave);
    qDebug() << "starting wave" << wave.enemies();
    foreach (int enemyIndex, wave.enemies()) {
        Enemy *enemy = m_enemyFactory->createEnemy(enemyIndex);
        connect(enemy, &Enemy::exited, this, &Engine::liveLost);
        connect(enemy, &Enemy::died, this, &Engine::enemyDied);
        m_board->addEnemy(enemy);
        m_waitingEnemies.append(enemy);
    }

    m_ticksToNextEnemy = 1;
}

void Engine::nextEnemy()
{
    m_waitingEnemies.takeFirst()->setRunning(true);
    if (m_waitingEnemies.count() > 0) {
        m_ticksToNextEnemy = 1.0 * m_waves.at(m_currentWave).interval() / 1000 * m_ticksPerSecond;

        switch (m_difficulty) {
        case DifficultyEasy:
            m_ticksToNextEnemy += 300.0 / 1000 * m_ticksPerSecond;
        case DifficultyMedium:
            m_ticksToNextEnemy += 300.0 / 1000 * m_ticksPerSecond;
        }
    }
}

void Engine::liveLost()
{
    m_lives--;
    emit livesChanged();

    if (m_lives == 0) {
        m_timer->stop();
        m_waitingEnemies.clear();
        m_playStatus = PlayStatusLost;
        emit playStatusChanged();
    }
}

void Engine::enemyCountChanged()
{
    if (m_board->enemies()->count() == 0) {
        if (m_currentWave == m_waves.count() - 1) {
            qDebug() << "level completed!";
            // lives : 10 = rewardPoints : maxRewardPoints
            int rewardPoints = m_lives * m_levelPack->get(m_level)->rewardPoints() / 10;

            int highscore = m_levelPack->get(m_level)->highscore();
            if (rewardPoints > highscore) {
                m_points += (rewardPoints - highscore);
                m_totalPoints += (rewardPoints - highscore);
                m_levelPack->get(m_level)->setHighscore(rewardPoints);
            }

            emit pointsChanged();
            savePoints();
            m_earnedPoints = rewardPoints;

            if (m_level == highestUnlockedLevel()) {
                setHighestUnlockedLevel(highestUnlockedLevel() + 1);
            }
            m_playStatus = PlayStatusWon;
            emit playStatusChanged();

        } else {
            m_ticksToNextWave = 3 * m_ticksPerSecond;
            emit secsToNextWaveChanged();
        }
    }
}

void Engine::enemyDied()
{
    Enemy *enemy = static_cast<Enemy*>(sender());

    m_money += enemy->reward();
    emit moneyChanged();
}

QString Engine::levelFile(int level)
{
    return m_dataDir.path() + QString("/%1/level%2/level.json").arg(m_levelPack->id()).arg(level);
}

bool Engine::loadLevel(int index)
{
    qDebug() << "loading level" << index;
    Level *level = m_levelPack->get(index);
    qDebug() << "have levels:" << m_levelPack->rowCount(QModelIndex());
    qDebug() << "level:" << level;

    qDebug() << "init board:" << m_board->enemies()->count();
    if (!m_board->init(level->rows(), level->columns(), level->fieldsOnPath(), level->forbiddenFields())) {
        qWarning() << "Error loading board. Entrance/Exit not found.";
        return false;
    }

    m_waves = level->waves();
    m_currentWave = -1;
    qDebug() << "loaded level with" << m_waves.count() << "waves";

    m_money = level->startMoney();
    emit moneyChanged();

    return true;
}

void Engine::loadEnemies()
{
    m_enemyFactory->clear();
    QFile enemyfile(m_dataDir.path() + "/" + m_levelPack->id() + "/enemies/enemies.json");
    if (!enemyfile.open(QFile::ReadOnly)) {
        qWarning() << "couldn't open" << enemyfile.fileName();
        return;
    }

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(enemyfile.readAll(), &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse" << enemyfile.fileName();
        return;
    }

    foreach (const QVariant &enemyVariant, jsonDoc.toVariant().toList()) {
        m_enemyFactory->createTemplate(enemyVariant.toMap());
    }
}

void Engine::loadTowers()
{
    m_towerFactory->clear();
    QFile towerfile(m_dataDir.path() + "/" + m_levelPack->id() + "/towers/towers.json");
    if (!towerfile.open(QFile::ReadOnly)) {
        qWarning() << "couldn't open" << towerfile.fileName();
        return;
    }

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(towerfile.readAll(), &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse" << towerfile.fileName();
        return;
    }

    foreach (const QVariant &towerVariant, jsonDoc.toVariant().toList()) {
        m_towerFactory->createTemplate(towerVariant.toMap());
    }
}

void Engine::loadLevels()
{
    int levelIndex = -1;
    while (QFileInfo(levelFile(++levelIndex + 1)).exists()) {
        QFile file(levelFile(levelIndex + 1));
        if (!file.open(QFile::ReadOnly)) {
            qWarning() << "Can't open level file" << levelFile(levelIndex + 1);
            continue;
        }

        QByteArray levelJson = file.readAll();
        file.close();

        QJsonParseError error;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(levelJson, &error);
        if (error.error != QJsonParseError::NoError) {
            qWarning() << "failed to parse level file" << error.errorString();
            continue;
        }

        QVariantMap levelData = jsonDoc.toVariant().toMap();
        if (!levelData.contains("board")
                || !levelData.value("board").toMap().contains("rows")
                || !levelData.value("board").toMap().contains("columns")
                || !levelData.value("board").toMap().contains("fieldsOnPath")) {
            qWarning() << "level file doesn't contain a valid board description.";
            continue;
        }

        if (!levelData.contains("waves")) {
            qWarning() << "Waves not found in level file";
            continue;
        }

        if (!levelData.contains("rewardPoints")) {
            qWarning() << "rewardPoints not fount in level file.";
            continue;
        }

        Level *level = m_levelPack->addLevel(levelData);
    }
}

void Engine::saveDifficulty()
{
    SETTINGS("scores");
    settings.setValue("lastDifficulty", m_difficulty);
    emit difficultyChanged();
}

void Engine::loadDifficulty()
{
    SETTINGS("scores");
    m_difficulty = (Difficulty)settings.value("lastDifficulty", 1).toInt();
}

void Engine::savePoints()
{
    SETTINGS("scores");
    settings.beginGroup(m_levelPack->id());
    settings.beginGroup(QString::number(m_difficulty));
    settings.setValue("points", m_points);
    settings.setValue("totalPoints", m_totalPoints);
}

void Engine::loadPoints()
{
    SETTINGS("scores");
    settings.beginGroup(m_levelPack->id());
    settings.beginGroup(QString::number(m_difficulty));
    m_points = settings.value("points").toInt();
    m_totalPoints = settings.value("totalPoints").toInt();
    emit pointsChanged();
}

int Engine::highestUnlockedLevel() const
{
    int level = 0;
    if (m_levelPack){
        SETTINGS("scores");
        settings.beginGroup(m_levelPack->id());
        settings.beginGroup(QString::number(m_difficulty));
        level = settings.value("highestUnlockedLevel").toInt();
    }
    {
        SETTINGS("global");
        if (settings.value("allUnlocked", false).toBool()) {
            level = 999999;
        }
    }
    return level;
}

void Engine::setHighestUnlockedLevel(int highestLevel)
{
    SETTINGS("scores");
    settings.beginGroup(m_levelPack->id());
    settings.beginGroup(QString::number(m_difficulty));
    settings.setValue("highestUnlockedLevel", highestLevel);
    emit highestUnlockedLevelChanged();
}

