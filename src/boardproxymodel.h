#ifndef BOARDPROXYMODEL_H
#define BOARDPROXYMODEL_H

#include <QSortFilterProxyModel>

class Board;
class Field;

class BoardProxyModel: public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(Board *sourceModel READ board WRITE setBoard NOTIFY boardChanged)

public:
    BoardProxyModel(QObject *parent = nullptr);

    Board *board() const;
    void setBoard(Board *board);

    QVariant data(const QModelIndex &index, int role) const override;
    Q_INVOKABLE Field *field(int row) const;

signals:
    void boardChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    Board *m_board;
};

#endif
