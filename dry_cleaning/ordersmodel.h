#ifndef ORDERSMODEL_H
#define ORDERSMODEL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QAbstractTableModel>
#include <QObject>
#include <QDebug>

#include "clientsmodel.h"
#include "managersmodel.h"
#include "statusesmodel.h"

class OrdersModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    ClientsModel* clients;
    ManagersModel* managers;
    StatusesModel* statuses;

    enum Column {
        NUMBER = 0,
        ID_CLIENT,
        ID_MANAGER,
        DATE_TIME_ORDER,
        DATE_OF_COMPLETION,
        TOTAL_COST,
        ID_STATUS,
        COMMENT,
        IS_DELETED,
        LAST,
        STATE_ROW
    };

    typedef QHash< OrdersModel::Column, QVariant > orderObject;

    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;

    void appendRow( const orderObject& newOrder );
    void updateRow( int row, const orderObject& order );
    void removeRow( int row );

    bool select();
    QVariant saveChanges();
    void setTable(QString t, QSqlDatabase *database);

    enum Role {
        Display = 0x1,
        Read
    };

    enum StatesRows {
        NOT_EDITED = 0x1,
        ADDED,
        EDITED,
        DELETED
    };

    QVariant getDataById(int id, Column column);
    orderObject getObjectByRow(int row);

private:

    typedef QList< orderObject > DataList;
    DataList model;

    QSqlDatabase *db;
    QSqlQuery query;

    QString table;
public:
    explicit OrdersModel(QObject *parent = nullptr);
};

typedef QHash< OrdersModel::Column, QVariant > orderObject;

#endif // ORDERSMODEL_H
