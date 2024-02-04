#include "typeofservicesmodel.h"

TypeOfServicesModel::TypeOfServicesModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

int TypeOfServicesModel::rowCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return model.count();
}

int TypeOfServicesModel::columnCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return LAST;
}

QVariant TypeOfServicesModel::headerData( int section, Qt::Orientation orientation, int role ) const
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
    case TITLE:
        return "Назва";
    case DESCRIPTION:
        return "Опис";
    }

    return QVariant();
}

QVariant TypeOfServicesModel::data( const QModelIndex& index, int role ) const {
    if(!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
    {
        return model[ index.row() ][ Column( index.column() ) ];
    }
    case TypeOfServicesModel::Role::Read:
    {
        return model[ index.row() ][ Column( index.column() ) ];
    }
    default:
    {
        return QVariant();
    }
    }
}

Qt::ItemFlags TypeOfServicesModel::flags( const QModelIndex& index ) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );


    return flags;
}

void TypeOfServicesModel::appendRow( const typeOfServicesObject& newObject ) {
    typeOfServicesObject record;
    record[ ID ] = newObject[ ID ];
    record[ TITLE ] = newObject[ TITLE ];
    record[ DESCRIPTION ] = newObject[ DESCRIPTION ];
    record[ STATE_ROW ] = (int)StatesRows::ADDED;

    int row = model.count();
    beginInsertRows( QModelIndex(), row, row );
    model.append( record );
    endInsertRows();
}

void TypeOfServicesModel::updateRow( int row, const typeOfServicesObject& object )
{
    beginResetModel();

    model[ row ][ ID ] = object[ ID ];
    model[ row ][ TITLE ] = object[ TITLE ];
    model[ row ][ DESCRIPTION ] = object[ DESCRIPTION ];
    model[ row ][ STATE_ROW ] = (int)StatesRows::EDITED;

    endResetModel();
}

void TypeOfServicesModel::removeRow(int row)
{
    model[ row ][ IS_DELETED ] = 1;
    model[ row ][ STATE_ROW ] = StatesRows::DELETED;
}

bool TypeOfServicesModel::select()
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

        typeOfServicesObject record;
        do
        {
            record[ ID ] = query.value( ID );
            record[ TITLE ] = query.value( TITLE );
            record[ DESCRIPTION ] = query.value( DESCRIPTION );
            record[ IS_DELETED ] = query.value( IS_DELETED );
            record[ STATE_ROW ] = StatesRows::NOT_EDITED;

            model.append( record );

        }while(query.next());

        endInsertRows();
    }

    endResetModel();

    return false;
}


bool TypeOfServicesModel::saveChanges()
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[ i ][ STATE_ROW ] != StatesRows::NOT_EDITED)
        {
            qDebug() << "!= NOT_EDIT";
            if(model[ i ][ STATE_ROW ] == StatesRows::ADDED)
            {
                qDebug() << "ADDED";
                query.prepare(QString("INSERT INTO %1 (title, description, isDelete) VALUES (:title, :description, 0)").arg(table));
                query.bindValue(":title", model[ i ][ TITLE ]);
                query.bindValue(":description", model[ i ][ DESCRIPTION ]);
                query.exec();
            }
            else if(model[ i ][ STATE_ROW ] == StatesRows::EDITED)
            {
                qDebug() << "EDITED";
                query.prepare(QString("UPDATE %1 SET title = :title, description = :description WHERE id = :id").arg(table));
                query.bindValue(":title", model[ i ][ TITLE ]);
                query.bindValue(":description", model[ i ][ DESCRIPTION ]);
                query.bindValue(":id", model[ i ][ ID ]);

                query.exec();
            }
            else if(model[ i ][ STATE_ROW ] == StatesRows::DELETED)
            {
                qDebug() << "DELETED";
                query.prepare(QString("UPDATE %1 SET isDelete = 1 WHERE id = :id").arg(table));
                query.bindValue(":id", model[ i ][ ID ]);

                query.exec();
            }
        }
    }

    qDebug() << query.lastError();

    return true;
}

void TypeOfServicesModel::setTable(QString t, QSqlDatabase *database)
{
    table = t;
    db = database;
}

typeOfServicesObject TypeOfServicesModel::getDataById(int id)
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[i][ID].toInt() == id)
            return model[i];
    }
}

int TypeOfServicesModel::getRowByData(Column column, QVariant data)
{
    for(int i = 0; i < model.size(); i++)
        if(model[i][column] == data)
            return i;
}

TypeOfServicesModel::typeOfServicesObject TypeOfServicesModel::getObjectByRow(int row)
{
    return model[row];
}

bool TypeOfServicesModel::setData( const QModelIndex& index, const QVariant& value, int role ) {
    if( !index.isValid() || role != Qt::EditRole || model.count() <= index.row() ) {
        return false;
    }

    model[ index.row() ][ Column( index.column() ) ] = value;
    emit dataChanged( index, index );

    return true;
}
