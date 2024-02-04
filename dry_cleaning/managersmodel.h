#ifndef MANAGERSMODEL_H
#define MANAGERSMODEL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QAbstractTableModel>
#include <QObject>
#include <QDebug>

class ManagersModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    enum Column {
        ID = 0,
        PIB,
        PHONE_NUMBER,
        LAST,
        IS_DELETED,
        STATE_ROW
    };

    typedef QHash< ManagersModel::Column, QVariant > managerObject;

    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;

    void appendRow( const managerObject& newManager );
    void updateRow( int row, const managerObject& manager );
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
    managerObject getObjectByRow(int row);

private:

    typedef QList< managerObject > DataList;
    DataList model;

    QSqlDatabase *db;
    QSqlQuery query;

    QString table;

public:
    explicit ManagersModel(QObject *parent = nullptr);
};

typedef QHash< ManagersModel::Column, QVariant > managerObject;

#endif // MANAGERSMODEL_H
