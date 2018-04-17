#ifndef BOARD_H
#define BOARD_H

#include <QAbstractListModel>

#include <QtQml>

class Field;
class Enemy;
class Enemies;
class Engine;

class Board : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Enemies* enemies READ enemies CONSTANT)
    Q_PROPERTY(int count READ count NOTIFY boardChanged)
    Q_PROPERTY(int columns READ columns NOTIFY boardChanged)
    Q_PROPERTY(int rows READ rows NOTIFY boardChanged)
    Q_PROPERTY(QList<int> path READ path NOTIFY boardChanged)

public:
    enum Role {
        RoleX,
        RoleY,
        RoleIsOnPath,
        RoleAllowedForEnemy,
        RoleAllowedForTower,
        RoleColor,
        RoleTower
    };

    explicit Board(Engine *engine, QObject *parent = 0);

    int count() { return rowCount(QModelIndex()); }
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE Field* field(int index) const;

    Enemies* enemies() const;

    int columns() const;
    int rows() const;
    QList<int> path() const;

    Q_INVOKABLE bool intersects(const QPoint &tower, int radius, const QPoint &enemy);

public slots:
    bool init(int rows, int columns, const QList<int> &fieldsOnPath, const QList<int> &forbiddenFields);

    void addEnemy(Enemy *enemy);

signals:
    void boardChanged();

private slots:
    void fieldTowerChanged();

private:
    Engine *m_engine;
    int m_rows;
    int m_columns;
    QList<Field*> m_fields;
    QList<int> m_path;
    Enemies *m_enemies;

};

#endif // BOARD_H
