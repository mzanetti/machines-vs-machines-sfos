#ifndef LEVEL_H
#define LEVEL_H

#include "wave.h"
#include "engine.h"

#include <QObject>

class Level : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int startMoney READ startMoney CONSTANT)
    Q_PROPERTY(int rows READ rows CONSTANT)
    Q_PROPERTY(int columns READ columns CONSTANT)
    Q_PROPERTY(int rewardPoints READ rewardPoints CONSTANT)
    Q_PROPERTY(QList<int> fieldsOnPath READ fieldsOnPath CONSTANT)
    Q_PROPERTY(QList<int> forbiddenFields READ forbiddenFields CONSTANT)
    Q_PROPERTY(QString soundFile READ soundFile CONSTANT)

    Q_PROPERTY(int highscore READ highscore NOTIFY highscoreChanged)

public:
    explicit Level(int number, Engine::Difficulty difficulty, const QString &levelpack, const QVariantMap &levelMap, QObject *parent = 0);

    int number() const;
    int startMoney() const;
    int rows() const;
    int columns() const;
    QList<int> fieldsOnPath() const;
    QList<int> forbiddenFields() const;
    QString soundFile() const;
    int rewardPoints() const;
    QList<Wave> waves() const;

    int highscore() const;
    void setHighscore(int highscore);

signals:
    void highscoreChanged();

private:
    int m_number;
    Engine::Difficulty m_difficulty;
    QString m_levelpack;
    int m_startMoney;
    int m_rows;
    int m_columns;
    QList<int> m_fieldsOnPath;
    QList<int> m_forbiddenFields;
    QString m_soundFile;
    int m_rewardPoints;
    QList<Wave> m_waves;

    int m_highscore;
};

#endif // LEVEL_H
