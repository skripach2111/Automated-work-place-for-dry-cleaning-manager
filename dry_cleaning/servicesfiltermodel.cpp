#include "servicesfiltermodel.h"

ServicesFilterModel::ServicesFilterModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}

void ServicesFilterModel::setFilterParam(FilterParam par, QVariant val)
{
    values[par] = val;
    invalidateFilter();
}

void ServicesFilterModel::setEnabledFilterParam(FilterParam par, bool flag)
{
    params[par] = flag;
    invalidateFilter();
}

bool ServicesFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(sourceModel()->index(source_row, ServicesModel::Column::IS_DELETED, source_parent).data(ServicesModel::Role::Read).toBool())
        return false;

    if(params[FilterParam::TITLE] && sourceModel()->index(source_row, ServicesModel::Column::TITLE, source_parent).data(ServicesModel::Role::Read).toString().indexOf(values[FilterParam::TITLE].toString()) == -1 )
        return false;

    if(params[FilterParam::TYPE] && sourceModel()->index(source_row, ServicesModel::Column::ID_TYPE, source_parent).data(ServicesModel::Role::Read).toInt() != values[FilterParam::TYPE].toInt())
        return false;

    return true;
}

bool ServicesFilterModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    if(source_column == ServicesModel::Column::ID || source_column == ServicesModel::Column::ID_IMAGE || source_column == ServicesModel::Column::IS_DELETED)
        return false;

    if(params[FilterParam::ONLY_COST])
        if(source_column != ServicesModel::Column::COST  && source_column != ServicesModel::Column::TITLE)
            return false;

    return true;
}
