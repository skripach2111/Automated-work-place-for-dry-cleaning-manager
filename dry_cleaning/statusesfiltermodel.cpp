#include "statusesfiltermodel.h"

StatusesFilterModel::StatusesFilterModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}

void StatusesFilterModel::setFilterParam(FilterParam par, QVariant val)
{
    values[par] = val;
    invalidateFilter();
}

void StatusesFilterModel::setEnabledFilterParam(FilterParam par, bool flag)
{
    params[par] = flag;
    invalidateFilter();
}

bool StatusesFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(sourceModel()->index(source_row, StatusesModel::Column::IS_DELETED, source_parent).data(StatusesModel::Role::Read).toBool())
        return false;

    if(params[FilterParam::TITLE] && sourceModel()->index(source_row, StatusesModel::Column::TITLE, source_parent).data(StatusesModel::Role::Read).toString().indexOf(values[FilterParam::TITLE].toString()) == -1 )
        return false;

    return true;
}

bool StatusesFilterModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    if(source_column == StatusesModel::Column::ID || source_column == StatusesModel::Column::IS_DELETED)
        return false;

    return true;
}
