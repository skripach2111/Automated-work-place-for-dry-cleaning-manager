#ifndef TYPEOFSERVICESMODEL_H
#define TYPEOFSERVICESMODEL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QAbstractTableModel>
#include <QObject>
#include <QDebug>

class TypeOfServicesModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    enum Column {
        ID = 0,
        TITLE,
        DESCRIPTION,
        LAST,
        IS_DELETED,
        STATE_ROW
    };

    typedef QHash< TypeOfServicesModel::Column, QVariant > typeOfServicesObject;

    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;

    void appendRow( const typeOfServicesObject& newObject );
    void updateRow( int row, const typeOfServicesObject& object );
    void removeRow( int row );

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

    typeOfServicesObject getDataById(int id);
    int getRowByData(Column column, QVariant data);
    typeOfServicesObject getObjectByRow(int row);

private:

    typedef QList< typeOfServicesObject > DataList;
    DataList model;

    QSqlDatabase *db;
    QSqlQuery query;

    QString table;
public:
    explicit TypeOfServicesModel(QObject *parent = nullptr);
};

typedef QHash< TypeOfServicesModel::Column, QVariant > typeOfServicesObject;

#endif // TYPEOFSERVICESMODEL_H
