#include "managersmodel.h"

ManagersModel::ManagersModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

int ManagersModel::rowCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return model.count();
}

int ManagersModel::columnCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return LAST;
}

QVariant ManagersModel::headerData( int section, Qt::Orientation orientation, int role ) const
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
    case PIB:
        return "ПІБ";
    case PHONE_NUMBER:
        return "Номер телефону";
    }

    return QVariant();
}

QVariant ManagersModel::data( const QModelIndex& index, int role ) const {
    if(!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
    {
        return model[ index.row() ][ Column( index.column() ) ];
    }
    case ManagersModel::Role::Read:
    {
        return model[ index.row() ][ Column( index.column() ) ];
    }
    default:
    {
        return QVariant();
    }
    }
}

Qt::ItemFlags ManagersModel::flags( const QModelIndex& index ) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );


    return flags;
}

void ManagersModel::appendRow( const managerObject& newManager ) {
    managerObject record;
    record[ ID ] = newManager[ ID ];
    record[ PIB ] = newManager[ PIB ];
    record[ PHONE_NUMBER ] = newManager[ PHONE_NUMBER ];
    record[ STATE_ROW ] = (int)StatesRows::ADDED;

    int row = model.count();
    beginInsertRows( QModelIndex(), row, row );
    model.append( record );
    endInsertRows();
}

void ManagersModel::updateRow( int row, const managerObject& manager )
{
    beginResetModel();

    model[ row ][ ID ] = manager[ ID ];
    model[ row ][ PIB ] = manager[ PIB ];
    model[ row ][ PHONE_NUMBER ] = manager[ PHONE_NUMBER ];
    model[ row ][ STATE_ROW ] = (int)StatesRows::EDITED;

    endResetModel();
}

void ManagersModel::removeRow(int row)
{
    model[ row ][ IS_DELETED ] = 1;
    model[ row ][ STATE_ROW ] = StatesRows::DELETED;
}

bool ManagersModel::select()
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

        managerObject record;
        do
        {
            record[ ID ] = query.value( ID );
            record[ PIB ] = query.value( PIB );
            record[ PHONE_NUMBER ] = query.value( PHONE_NUMBER );
            record[ IS_DELETED ] = query.value( IS_DELETED );
            record[ STATE_ROW ] = StatesRows::NOT_EDITED;

            model.append( record );

        }while(query.next());

        endInsertRows();
    }

    endResetModel();

    return false;
}


bool ManagersModel::saveChanges()
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[ i ][ STATE_ROW ] != StatesRows::NOT_EDITED)
        {
            qDebug() << "!= NOT_EDIT";
            if(model[ i ][ STATE_ROW ] == StatesRows::ADDED)
            {
                qDebug() << "ADDED";
                query.prepare(QString("INSERT INTO %1 (pib, phone_number, isDelete) VALUES (:pib, :phone_number, 0)").arg(table));
                query.bindValue(":pib", model[ i ][ PIB ]);
                query.bindValue(":phone_number", model[ i ][ PHONE_NUMBER ]);
                query.exec();
            }
            else if(model[ i ][ STATE_ROW ] == StatesRows::EDITED)
            {
                qDebug() << "EDITED";
                query.prepare(QString("UPDATE %1 SET pib = :pib, phone_number = :phone_number WHERE id = :id").arg(table));
                query.bindValue(":pib", model[ i ][ PIB ]);
                query.bindValue(":phone_number", model[ i ][ PHONE_NUMBER ]);
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

void ManagersModel::setTable(QString t, QSqlDatabase *database)
{
    table = t;
    db = database;
}

QVariant ManagersModel::getDataById(int id, Column column)
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[i][ID].toInt() == id)
            return model[i][column];
    }

    return QVariant();
}

int ManagersModel::getRowByData(Column column, QVariant data)
{
    for(int i = 0; i < model.size(); i++)
        if(model[i][column] == data)
            return i;
    return -1;
}

ManagersModel::managerObject ManagersModel::getObjectByRow(int row)
{
    return model[row];
}

bool ManagersModel::setData( const QModelIndex& index, const QVariant& value, int role ) {
    if( !index.isValid() || role != Qt::EditRole || model.count() <= index.row() ) {
        return false;
    }

    model[ index.row() ][ Column( index.column() ) ] = value;
    emit dataChanged( index, index );

    return true;
}
