#ifndef FILESMODEL_H
#define FILESMODEL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QAbstractTableModel>
#include <QObject>
#include <QDebug>
#include <QPixmap>

class FilesModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    enum Column {
        ID = 0,
        FILE,
        LAST,
        STATE_ROW
    };

    typedef QHash< FilesModel::Column, QVariant > fileObject;

    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;

    void appendRow( const fileObject& newFile );
    void updateRow( int row, const fileObject& file );
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

    fileObject getDataById(int id);
    int getRowByData(Column column, QVariant data);

private:

    typedef QList< fileObject > DataList;
    DataList model;

    QSqlDatabase *db;
    QSqlQuery query;

    QString table;
public:
    explicit FilesModel(QObject *parent = nullptr);
};

typedef QHash< FilesModel::Column, QVariant > fileObject;

#endif // FILESMODEL_H
