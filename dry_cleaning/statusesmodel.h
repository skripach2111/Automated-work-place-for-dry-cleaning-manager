#ifndef STATUSESMODEL_H
#define STATUSESMODEL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QAbstractTableModel>
#include <QObject>
#include <QDebug>

class StatusesModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    enum Column {
        ID = 0,
        TITLE,
        IS_DELETED,
        LAST,
        STATE_ROW
    };

    typedef QHash< StatusesModel::Column, QVariant > statusObject;

    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;

    void appendRow( const statusObject& newStatus );
    void updateRow( int row, const statusObject& status );
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
    statusObject getObjectByRow(int row);

private:

    typedef QList< statusObject > DataList;
    DataList model;

    QSqlDatabase *db;
    QSqlQuery query;

    QString table;
public:
    explicit StatusesModel(QObject *parent = nullptr);
};

typedef QHash< StatusesModel::Column, QVariant > statusObject;

#endif // STATUSESMODEL_H
