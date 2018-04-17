#include "level.h"
#include "settings.h"

#include <QVariantMap>

Level::Level(int number, Engine::Difficulty difficulty, const QString &levelpack, const QVariantMap &levelMap, QObject *parent) :
    QObject(parent),
    m_number(number),
    m_difficulty(difficulty),
    m_levelpack(levelpack)
{
    m_startMoney = levelMap.value("startMoney").toInt();
    m_rows = levelMap.value("board").toMap().value("rows").toInt();
    m_columns = levelMap.value("board").toMap().value("columns").toInt();
    foreach (const QVariant &fieldVariant, levelMap.value("board").toMap().value("fieldsOnPath").toList()) {
        m_fieldsOnPath.append(fieldVariant.toInt());
    }
    foreach (const QVariant &fieldVariant, levelMap.value("board").toMap().value("forbiddenFields").toList()) {
        m_forbiddenFields.append(fieldVariant.toInt());
    }

    m_soundFile = levelMap.value("soundFile").toString();
    m_rewardPoints = levelMap.value("rewardPoints").toInt();

    foreach (const QVariant &waveData, levelMap.value("waves").toList()) {
        QList<int> enemies;
        foreach (const QVariant &enemyId, waveData.toMap().value("enemies").toList()) {
            enemies.append(enemyId.toInt());
        }
        Wave wave(waveData.toMap().value("interval").toInt(), enemies);
        m_waves.append(wave);
    }
}

int Level::startMoney() const
{
    return m_startMoney;
}

int Level::rows() const
{
    return m_rows;
}

int Level::columns() const
{
    return m_columns;
}

QList<int> Level::fieldsOnPath() const
{
    return m_fieldsOnPath;
}

QList<int> Level::forbiddenFields() const
{
    return m_forbiddenFields;
}

QString Level::soundFile() const
{
    return m_soundFile;
}

int Level::rewardPoints() const
{
    return m_rewardPoints;
}

QList<Wave> Level::waves() const
{
    return m_waves;
}

int Level::highscore() const
{
    SETTINGS("scores");
    settings.beginGroup(m_levelpack);
    settings.beginGroup(QString::number(m_difficulty));
    return settings.value("highscore-level" + QString::number(m_number)).toInt();
}

void Level::setHighscore(int highscore)
{
    SETTINGS("scores");
    settings.beginGroup(m_levelpack);
    settings.beginGroup(QString::number(m_difficulty));
    if (this->highscore() != highscore) {
        settings.setValue("highscore-level" + QString::number(m_number), highscore);
        emit highscoreChanged();
    }
}
