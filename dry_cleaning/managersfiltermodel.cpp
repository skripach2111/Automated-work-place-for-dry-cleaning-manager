#include "managersfiltermodel.h"

ManagerFilterModel::ManagerFilterModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}

void ManagerFilterModel::setFilterParam(FilterParam par, QVariant val)
{
    values[par] = val;
    invalidateFilter();
}

void ManagerFilterModel::setEnabledFilterParam(FilterParam par, bool flag)
{
    params[par] = flag;
    invalidateFilter();
}

bool ManagerFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(sourceModel()->index(source_row, ManagersModel::Column::IS_DELETED, source_parent).data(ManagersModel::Role::Read).toBool())
        return false;

    if(params[FilterParam::PIB] && sourceModel()->index(source_row, ManagersModel::Column::PIB, source_parent).data(ManagersModel::Role::Read).toString().indexOf(values[FilterParam::PIB].toString()) == -1 )
        return false;

    if(params[FilterParam::PHONE_NUMBER] && sourceModel()->index(source_row, ManagersModel::Column::PHONE_NUMBER, source_parent).data(ManagersModel::Role::Read).toString().indexOf(values[FilterParam::PHONE_NUMBER].toString()) == -1 )
        return false;

    return true;
}

bool ManagerFilterModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    if(source_column == ManagersModel::Column::ID || source_column == ManagersModel::Column::IS_DELETED)
        return false;

    return true;
}
