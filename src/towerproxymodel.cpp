#include "towerproxymodel.h"
#include "towerfactory.h"

#include <QDebug>

TowerProxyModel::TowerProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    m_towerId(-1),
    m_level(0),
    m_onlyUnlocked(false)
{
}


TowerFactory *TowerProxyModel::towerFactory() const
{
    return m_sourceModel;
}

void TowerProxyModel::setTowerFactory(TowerFactory *towerFactory)
{
    m_sourceModel = towerFactory;
    setSourceModel(m_sourceModel);
    invalidateFilter();
    emit towerFactoryChanged();
}

bool TowerProxyModel::onlyUnlocked() const
{
    return m_onlyUnlocked;
}

void TowerProxyModel::setOnlyUnlocked(bool onlyUnlocked)
{
    m_onlyUnlocked = onlyUnlocked;
    emit onlyUnlockedChanged();
    invalidateFilter();
}

int TowerProxyModel::towerId() const
{
    return m_towerId;
}

void TowerProxyModel::setTowerId(int towerId)
{
    if (m_towerId != towerId) {
        m_towerId = towerId;
        emit towerIdChanged();
        invalidateFilter();
        qDebug() << "towerid set" << towerId;
    }
}

int TowerProxyModel::level() const
{
    return m_level;
}

void TowerProxyModel::setLevel(int level)
{
    if (m_level != level) {
        m_level = level;
        invalidateFilter();
        emit levelChanged();
        emit layoutChanged();
    }
}

QVariant TowerProxyModel::data(const QModelIndex &index, int role) const
{
    return m_sourceModel->data(mapToSource(index), role, m_level);
}

QVariantMap TowerProxyModel::get(int row, int level) const
{
    return m_sourceModel->get(mapToSource(index(row, 0)).row(), level);
}

QVariantList TowerProxyModel::getLineup(int row) const
{
    return m_sourceModel->getLineup(mapToSource(index(row, 0)).row());
}

bool TowerProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (m_level >= m_sourceModel->data(m_sourceModel->index(source_row), TowerFactory::RoleLevels).toInt()) {
        return false;
    }
    if (m_onlyUnlocked && m_sourceModel->data(m_sourceModel->index(source_row), TowerFactory::RoleLocked, m_level).toBool()) {
        return false;
    }
    if (m_towerId > -1) {
        return m_sourceModel->data(m_sourceModel->index(source_row), TowerFactory::RoleId).toInt() == m_towerId;
    }
    return true;
}
