#ifndef ORDERSFILTERMODEL_H
#define ORDERSFILTERMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

#include "ordersmodel.h"

class OrderFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit OrderFilterModel(QObject *parent = nullptr);

    enum FilterParam {
        NUMBER = 0,
        CLIENT,
        STATUS
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

#endif // ORDERSFILTERMODEL_H
