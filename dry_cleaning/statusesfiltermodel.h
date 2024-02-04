#ifndef STATUSESFILTERMODEL_H
#define STATUSESFILTERMODEL_H

#include <QSortFilterProxyModel>
#include <QObject>

#include "statusesmodel.h"

class StatusesFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit StatusesFilterModel(QObject *parent = nullptr);

    enum FilterParam {
        TITLE = 0
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

#endif // STATUSESFILTERMODEL_H
