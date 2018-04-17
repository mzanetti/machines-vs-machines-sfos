#include "levelpack.h"

#include "level.h"


LevelPack::LevelPack(Engine *engine, const QString &id, const QString &name, const QString &titleImage, const QString &copyright, const QString &titleSound, QObject *parent) :
    QAbstractListModel(parent),
    m_engine(engine),
    m_id(id),
    m_name(name),
    m_titleImage(titleImage),
    m_copyright(copyright),
    m_titleSound(titleSound)
{
    m_enemies = new Enemies(engine, this);
}

QString LevelPack::id() const
{
    return m_id;
}

QString LevelPack::name() const
{
    return m_name;
}

QString LevelPack::titleImage() const
{
    return m_titleImage;
}

QString LevelPack::copyright() const
{
    return m_copyright;
}

QString LevelPack::titleSound() const
{
    return m_titleSound;
}

int LevelPack::rowCount(const QModelIndex &parent) const
{
    return m_levels.count();
}

QVariant LevelPack::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case RoleHighscore:
        return m_levels.at(index.row())->highscore();
    }
    return QVariant();
}

QHash<int, QByteArray> LevelPack::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(RoleHighscore, "highscore");
    return roles;
}

Level *LevelPack::get(int index) const
{
    if (index >= m_levels.count()) {
        return nullptr;
    }
    return m_levels.at(index);
}

Enemies *LevelPack::enemies() const
{
    return m_enemies;
}

TowerFactory *LevelPack::towers() const
{
    return m_engine->towerFactory();
}

int LevelPack::totalPoints() const
{
    int count = 0;
    foreach (Level *level, m_levels) {
        count += level->rewardPoints();
    }
    return count;
}

Level* LevelPack::addLevel(const QVariantMap &levelMap)
{
    beginInsertRows(QModelIndex(), m_levels.count(), m_levels.count());
    Level *level = new Level(m_levels.count(), m_engine->difficulty(), m_id, levelMap, this);
    m_levels.append(level);
    endInsertRows();
    connect(level, &Level::highscoreChanged, this, &LevelPack::levelHighscoreChanged);
    return level;
}

void LevelPack::clear()
{
    beginResetModel();
    qDeleteAll(m_levels);
    m_levels.clear();
    endResetModel();
}

void LevelPack::init()
{
    m_enemies->clear();
    foreach (int id, m_engine->enemyFactory()->templateIds()) {
        m_enemies->addEnemy(m_engine->enemyFactory()->createEnemy(id));
    }
}

void LevelPack::levelHighscoreChanged()
{
    Level *level = static_cast<Level*>(sender());
    int idx = m_levels.indexOf(level);
    emit dataChanged(index(idx), index(idx));
}
