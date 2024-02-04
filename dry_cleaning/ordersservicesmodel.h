#ifndef ORDERSSERVICESMODEL_H
#define ORDERSSERVICESMODEL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QAbstractTableModel>
#include <QObject>
#include <QDebug>

class OrdersServicesModel : public QAbstractTableModel
{
    Q_OBJECT

    int id_for_del;

public:

    enum Column {
        ID_ORDER = 0,
        ID_SERVICE,
        NUMBER,
        LAST,
        STATE_ROW
    };

    typedef QHash< OrdersServicesModel::Column, QVariant > orderServiceObject;

    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;

    void appendRow( const orderServiceObject& newOrderService );
    void updateRow( int& row, const orderServiceObject& orderService );
    void removeRow( int id );

    bool select();
    bool saveChanges();
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

    QList <orderServiceObject> getDataById(int idOrder);

private:

    typedef QList< orderServiceObject > DataList;
    DataList model;

    QSqlDatabase *db;
    QSqlQuery query;

    QString table;
public:
    explicit OrdersServicesModel(QObject *parent = nullptr);
};

typedef QHash< OrdersServicesModel::Column, QVariant > orderServiceObject;

#endif // ORDERSSERVICESMODEL_H
