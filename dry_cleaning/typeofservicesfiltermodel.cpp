#include "typeofservicesfiltermodel.h"

TypeOfServicesFilterModel::TypeOfServicesFilterModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}

void TypeOfServicesFilterModel::setFilterParam(FilterParam par, QVariant val)
{
    values[par] = val;
    invalidateFilter();
}

void TypeOfServicesFilterModel::setEnabledFilterParam(FilterParam par, bool flag)
{
    params[par] = flag;
    invalidateFilter();
}

bool TypeOfServicesFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(sourceModel()->index(source_row, TypeOfServicesModel::Column::IS_DELETED, source_parent).data(TypeOfServicesModel::Role::Read).toBool())
        return false;

    if(params[FilterParam::TITLE] && sourceModel()->index(source_row, TypeOfServicesModel::Column::TITLE, source_parent).data(TypeOfServicesModel::Role::Read).toString().indexOf(values[FilterParam::TITLE].toString()) == -1 )
        return false;

    return true;
}

bool TypeOfServicesFilterModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    if(source_column == TypeOfServicesModel::Column::ID || source_column == TypeOfServicesModel::Column::IS_DELETED)
        return false;

    return true;
}
