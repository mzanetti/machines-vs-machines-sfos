#include "board.h"

#include "field.h"
#include "enemy.h"
#include "enemies.h"
#include "tower.h"

#include <QVector2D>

Board::Board(Engine *engine, QObject *parent) :
    QAbstractListModel(parent),
    m_engine(engine),
    m_rows(-1),
    m_columns(-1),
    m_enemies(new Enemies(engine, this))
{

}

int Board::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_fields.count();
}

QVariant Board::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case RoleX:
        return m_fields.at(index.row())->x();
    case RoleY:
        return m_fields.at(index.row())->y();
    case RoleIsOnPath:
        return m_fields.at(index.row())->isOnPath();
    case RoleAllowedForTower:
        return m_fields.at(index.row())->towerAllowed();
    case RoleAllowedForEnemy:
        return m_fields.at(index.row())->allowedForEnemy();
    case RoleColor:
        return m_fields.at(index.row())->color();
    case RoleTower:
        return QVariant::fromValue(m_fields.at(index.row())->tower());
    }

    return QVariant();
}

QHash<int, QByteArray> Board::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(RoleX, "x");
    roles.insert(RoleY, "y");
    roles.insert(RoleIsOnPath, "isOnPath");
    roles.insert(RoleAllowedForTower, "allowedForTower");
    roles.insert(RoleAllowedForEnemy, "allowedForEnemy");
    roles.insert(RoleColor, "color");
    roles.insert(RoleTower, "tower");
    return roles;
}

Enemies *Board::enemies() const
{
    return m_enemies;
}

Field *Board::field(int index) const
{
    if (index  < 0 || index > m_fields.count() - 1) {
        return nullptr;
    }
    return m_fields.at(index);
}

int Board::columns() const
{
    return m_columns;
}

int Board::rows() const
{
    return m_rows;
}

QList<int> Board::path() const
{
    return m_path;
}

bool Board::intersects(const QPoint &tower, int radius, const QPoint &enemy)
{
    QVector2D towerVector(tower);
    QVector2D enemyVector(enemy);

    return towerVector.distanceToPoint(enemyVector) < radius;
}

bool Board::init(int rows, int columns, const QList<int> &fieldsOnPath, const QList<int> &forbiddenFields)
{
    m_enemies->clear();

    beginResetModel();

    // reset values
    qDeleteAll(m_fields);
    m_fields.clear();
    m_rows = 0;
    m_columns = 0;

    // refill
    m_rows = rows;
    m_columns = columns;
    m_path = fieldsOnPath;

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            Field * newField = new Field(m_engine, x, y, this);
            connect(newField, &Field::towerChanged, this, &Board::fieldTowerChanged);

            if (fieldsOnPath.contains(y * columns + x)) {
                newField->setIsOnPath(true);
            }

            if (forbiddenFields.contains(y * columns + x)) {
                newField->setTowerAllowed(false);
            }

            m_fields.append(newField);
        }
    }
    qDebug() << "Loaded board:";
    qDebug() << "rows:" << m_rows << "columns:" << m_columns;
    endResetModel();
    emit boardChanged();

    return true;
}

void Board::addEnemy(Enemy *enemy)
{
    m_enemies->addEnemy(enemy);
}

void Board::fieldTowerChanged()
{
    qDebug() << "fieldTowerChanged";
    Field *field = static_cast<Field*>(sender());
    int idx = m_fields.indexOf(field);
    emit dataChanged(index(idx), index(idx), QVector<int>() << RoleTower);
}
