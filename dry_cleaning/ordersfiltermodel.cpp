#include "ordersfiltermodel.h"

OrderFilterModel::OrderFilterModel(QObject *parent)
{

}

void OrderFilterModel::setFilterParam(FilterParam par, QVariant val)
{
    values[par] = val;
    invalidateFilter();
}

void OrderFilterModel::setEnabledFilterParam(FilterParam par, bool flag)
{
    params[par] = flag;
    invalidateFilter();
}

bool OrderFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(sourceModel()->index(source_row, OrdersModel::Column::IS_DELETED, source_parent).data(OrdersModel::Role::Read).toBool())
        return false;

    if(params[FilterParam::NUMBER] && sourceModel()->index(source_row, OrdersModel::Column::NUMBER, source_parent).data(OrdersModel::Role::Read).toString().indexOf(values[FilterParam::NUMBER].toString()) == -1 )
        return false;

    if(params[FilterParam::CLIENT] && sourceModel()->index(source_row, OrdersModel::Column::ID_CLIENT, source_parent).data(OrdersModel::Role::Read) != values[FilterParam::CLIENT])
        return false;

    if(params[FilterParam::STATUS] && sourceModel()->index(source_row, OrdersModel::Column::ID_STATUS, source_parent).data(OrdersModel::Role::Read).toString() != values[FilterParam::STATUS])
        return false;

    return true;
}

bool OrderFilterModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    if(source_column == OrdersModel::Column::IS_DELETED)
        return false;

    return true;
}
