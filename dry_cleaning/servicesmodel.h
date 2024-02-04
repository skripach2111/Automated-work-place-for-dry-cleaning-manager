#ifndef SERVICESMODEL_H
#define SERVICESMODEL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QAbstractTableModel>
#include <QObject>
#include <QDebug>

#include "typeofservicesmodel.h"
#include "filesmodel.h"

class ServicesModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    TypeOfServicesModel* typeModel;
    FilesModel* fileModel;

    enum Column {
        ID = 0,
        TITLE,
        ID_TYPE,
        ID_IMAGE,
        DESCRIPTION,
        COST,
        IS_DELETED,
        LAST,
        STATE_ROW
    };

    typedef QHash< ServicesModel::Column, QVariant > serviceObject;

    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;

    void appendRow( const serviceObject& newObject );
    void updateRow( int row, const serviceObject& object );
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

    QVariant getDataById(int id, Column column);
    int getRowByData(Column column, QVariant data);
    serviceObject getObjectByRow(int row);

private:

    typedef QList< serviceObject > DataList;
    DataList model;

    QSqlDatabase *db;
    QSqlQuery query;

    QString table;
public:
    explicit ServicesModel(QObject *parent = nullptr);
};

typedef QHash< ServicesModel::Column, QVariant > serviceObject;

#endif // SERVICESMODEL_H
