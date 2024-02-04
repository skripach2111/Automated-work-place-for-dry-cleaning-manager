#ifndef SERVICESFILTERMODEL_H
#define SERVICESFILTERMODEL_H

#include <QSortFilterProxyModel>
#include <QObject>

#include "servicesmodel.h"

class ServicesFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ServicesFilterModel(QObject *parent = nullptr);

    enum FilterParam {
        TITLE = 0,
        ONLY_COST,
        TYPE
    };

    void setFilterParam(FilterParam par, QVariant val);
    void setEnabledFilterParam(FilterParam par, bool flag);

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;


private:
    QHash <FilterParam, bool> params;
    QHash <FilterParam, QVariant> values;
};

#endif // SERVICESFILTERMODEL_H
