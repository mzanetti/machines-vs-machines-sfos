#ifndef TOWERPROXYMODEL_H
#define TOWERROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class TowerFactory;

class TowerProxyModel: public QSortFilterProxyModel
{
    Q_OBJECT
    // Set this to the towerFactory you want to display
    Q_PROPERTY(TowerFactory *towerFactory READ towerFactory WRITE setTowerFactory NOTIFY towerFactoryChanged)
    // filter for unlocked towers only
    Q_PROPERTY(bool onlyUnlocked READ onlyUnlocked WRITE setOnlyUnlocked NOTIFY onlyUnlockedChanged)
    // filter for a certain tower only, -1 for no filter.
    Q_PROPERTY(int towerId READ towerId WRITE setTowerId NOTIFY towerIdChanged)
    // Set this to display tower information at a given level. defaults to 0.
    Q_PROPERTY(int level READ level WRITE setLevel NOTIFY levelChanged)

public:
    explicit TowerProxyModel(QObject *parent = 0);

    TowerFactory *towerFactory() const;
    void setTowerFactory(TowerFactory *towerFactory);

    bool onlyUnlocked() const;
    void setOnlyUnlocked(bool onlyUnlocked);

    int towerId() const;
    void setTowerId(int towerId);

    int level() const;
    void setLevel(int level);

    QVariant data(const QModelIndex &index, int role) const override;
    Q_INVOKABLE QVariantMap get(int row, int level) const;
    Q_INVOKABLE QVariantList getLineup(int row) const;

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

signals:
    void towerFactoryChanged();
    void onlyUnlockedChanged();
    void towerIdChanged();
    void levelChanged();

private:
    TowerFactory *m_sourceModel;
    bool m_onlyUnlocked;
    int m_towerId;
    int m_level;
};

#endif // TOWERPROXYMODEL_H
