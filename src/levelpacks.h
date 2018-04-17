#ifndef LEVELPACKS_H
#define LEVELPACKS_H

#include "engine.h"

#include <QAbstractListModel>

class LevelPack;

class LevelPacks : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        RoleId,
        RoleName,
        RoleTitleImage,
        RoleCopyright
    };

    explicit LevelPacks(Engine *engine, const QUrl &dataDir, QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    Q_INVOKABLE LevelPack *levelPack(const QString &id) const;
    QHash<int, QByteArray> roleNames() const {
        QHash<int, QByteArray> roles;
        roles.insert(RoleId, "id");
        roles.insert(RoleName, "name");
        roles.insert(RoleTitleImage, "titleImage");
        roles.insert(RoleCopyright, "copyright");
        return roles;
    }
private:
    Engine* m_engine;
    QList<LevelPack*> m_list;

};

#endif // LEVELPACKS_H
