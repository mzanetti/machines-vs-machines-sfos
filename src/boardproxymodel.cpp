#include "boardproxymodel.h"
#include "board.h"
#include "field.h"

BoardProxyModel::BoardProxyModel(QObject *parent):
    QSortFilterProxyModel(parent)
{

}

Board* BoardProxyModel::board() const
{
    return m_board;
}

void BoardProxyModel::setBoard(Board *board)
{
    m_board = board;
    setSourceModel(board);
    emit boardChanged();
}

QVariant BoardProxyModel::data(const QModelIndex &index, int role) const
{
    return m_board->data(mapToSource(index), role);
}

Field* BoardProxyModel::field(int row) const
{
    QModelIndex idx = index(row, 0);
    return m_board->field(mapToSource(idx).row());
}

bool BoardProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    return m_board->field(source_row)->tower() != nullptr;
}
