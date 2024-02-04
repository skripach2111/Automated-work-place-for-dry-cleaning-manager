#include "filesmodel.h"

FilesModel::FilesModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

int FilesModel::rowCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return model.count();
}

int FilesModel::columnCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return LAST;
}

QVariant FilesModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if( orientation == Qt::Vertical ) {
        return section;
    }

    switch( section ) {
    case ID:
        return "ID";
    case FILE:
        return "FILE";
    }

    return QVariant();
}

QVariant FilesModel::data( const QModelIndex& index, int role ) const {
    if(!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
    {
        return QVariant();
    }
    case FilesModel::Role::Read:
    {
        return model[ index.row() ][ Column( index.column() ) ];
    }
    case Qt::DecorationRole: //image
    {
        QPixmap pixmap;
        pixmap.loadFromData(model[index.row()][FILE].toByteArray());
        pixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio);
        return QVariant(pixmap);
    }
    default:
    {
        return QVariant();
    }
    }
}

Qt::ItemFlags FilesModel::flags( const QModelIndex& index ) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );


    return flags;
}

void FilesModel::appendRow( const fileObject& newFile ) {
    fileObject record;
    record[ ID ] = newFile[ ID ];
    record[ FILE ] = newFile[ FILE ];
    record[ STATE_ROW ] = (int)StatesRows::ADDED;

    int row = model.count();
    beginInsertRows( QModelIndex(), row, row );
    model.append( record );
    endInsertRows();
}

void FilesModel::updateRow( int row, const fileObject& file )
{
    beginResetModel();

    model[ row ][ ID ] = file[ ID ];
    model[ row ][ FILE ] = file[ FILE ];
    model[ row ][ STATE_ROW ] = (int)StatesRows::EDITED;

    endResetModel();
}

void FilesModel::removeRow(int row)
{
    model[ row ][ STATE_ROW ] = StatesRows::DELETED;
}

bool FilesModel::select()
{
    beginResetModel();
    beginRemoveRows(createIndex(0, 0), 0, model.count());
    while(model.count() != 0)
        model.removeFirst();
    endRemoveRows();

    query.prepare(QString("SELECT * FROM %1").arg(table));
    query.exec();
    if(query.next())
    {
        int row = model.count();
        beginInsertRows( createIndex(0, 0), row, row+query.size()-1 );

        fileObject record;
        do
        {
            record[ ID ] = query.value( ID );
            record[ FILE ] = query.value( FILE );
            record[ STATE_ROW ] = StatesRows::NOT_EDITED;

            model.append( record );

        }while(query.next());

        endInsertRows();
    }

    endResetModel();

    return false;
}


QVariant FilesModel::saveChanges()
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[ i ][ STATE_ROW ] != StatesRows::NOT_EDITED)
        {
            qDebug() << "!= NOT_EDIT";
            if(model[ i ][ STATE_ROW ] == StatesRows::ADDED)
            {
                qDebug() << "ADDED";
                query.prepare(QString("INSERT INTO %1 (file) VALUES (:file)").arg(table));
                query.bindValue(":file", model[ i ][ FILE ].toByteArray(), QSql::In | QSql::Binary );
                query.exec();
            }
            else if(model[ i ][ STATE_ROW ] == StatesRows::EDITED)
            {
                qDebug() << "EDITED";
                query.prepare(QString("UPDATE %1 SET file = :file WHERE id = :id").arg(table));
                query.bindValue(":file", model[ i ][ FILE ].toByteArray(), QSql::In | QSql::Binary);
                query.bindValue(":id", model[ i ][ ID ]);

                query.exec();
            }
            else if(model[ i ][ STATE_ROW ] == StatesRows::DELETED)
            {
                qDebug() << "DELETED";
                query.prepare(QString("DELETE FROM %1 WHERE id = :id").arg(table));
                query.bindValue(":id", model[ i ][ ID ]);

                query.exec();
            }
        }
    }

    qDebug() << query.lastError();

    query.prepare("SELECT LAST_INSERT_ID();");
    query.exec();
    query.next();

    qDebug() << query.lastError();

    return query.value(0);
}

void FilesModel::setTable(QString t, QSqlDatabase *database)
{
    table = t;
    db = database;
}

FilesModel::fileObject FilesModel::getDataById(int id)
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[i][ID].toInt() == id)
            return model[i];
    }
}

int FilesModel::getRowByData(Column column, QVariant data)
{
    for(int i = 0; i < model.size(); i++)
        if(model[i][column] == data)
            return i;
}

bool FilesModel::setData( const QModelIndex& index, const QVariant& value, int role ) {
    if( !index.isValid() || role != Qt::EditRole || model.count() <= index.row() ) {
        return false;
    }

    model[ index.row() ][ Column( index.column() ) ] = value;
    emit dataChanged( index, index );

    return true;
}
