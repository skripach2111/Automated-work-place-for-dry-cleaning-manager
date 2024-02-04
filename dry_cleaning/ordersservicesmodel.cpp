#include "ordersservicesmodel.h"

OrdersServicesModel::OrdersServicesModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

int OrdersServicesModel::rowCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return model.count();
}

int OrdersServicesModel::columnCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return LAST;
}

QVariant OrdersServicesModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if( orientation == Qt::Vertical ) {
        return section;
    }

    switch( section ) {
    case ID_ORDER:
        return "ID_ORDER";
    case ID_SERVICE:
        return "ID_SERVICE";
    case NUMBER:
        return "Кількість";
    }

    return QVariant();
}

QVariant OrdersServicesModel::data( const QModelIndex& index, int role ) const {
    if(!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
    {
        return model[ index.row() ][ Column( index.column() ) ];
    }
    case OrdersServicesModel::Role::Read:
    {
        return model[ index.row() ][ Column( index.column() ) ];
    }
    default:
    {
        return QVariant();
    }
    }
}

Qt::ItemFlags OrdersServicesModel::flags( const QModelIndex& index ) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );


    return flags;
}

void OrdersServicesModel::appendRow( const orderServiceObject& newOrderService ) {
    orderServiceObject record;
    record[ ID_ORDER ] = newOrderService[ ID_ORDER ];
    record[ ID_SERVICE ] = newOrderService[ ID_SERVICE ];
    record[ NUMBER ] = newOrderService[ NUMBER ];
    record[ STATE_ROW ] = (int)StatesRows::ADDED;

    int row = model.count();
    beginInsertRows( QModelIndex(), row, row );
    model.append( record );
    endInsertRows();
}

void OrdersServicesModel::updateRow( int& row, const orderServiceObject& orderService )
{
    beginResetModel();

    model[ row ][ ID_ORDER ] = orderService[ ID_ORDER ];
    model[ row ][ ID_SERVICE ] = orderService[ ID_SERVICE ];
    model[ row ][ NUMBER ] = orderService[ NUMBER ];
    model[ row ][ STATE_ROW ] = (int)StatesRows::EDITED;

    endResetModel();
}

void OrdersServicesModel::removeRow(int id)
{
    for(int i = 0; i < model.size(); i++)
        if(model[i][ID_ORDER].toInt() == id)
            model[ i ][ STATE_ROW ] = (int)StatesRows::DELETED;
}

bool OrdersServicesModel::select()
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

        orderServiceObject record;
        do
        {
            record[ ID_ORDER ] = query.value( ID_ORDER );
            record[ ID_SERVICE ] = query.value( ID_SERVICE );
            record[ NUMBER ] = query.value( NUMBER );
            record[ STATE_ROW ] = StatesRows::NOT_EDITED;

            model.append( record );

        }while(query.next());

        endInsertRows();
    }

    endResetModel();

    return false;
}


bool OrdersServicesModel::saveChanges()
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[ i ][ STATE_ROW ] != StatesRows::NOT_EDITED)
        {
            qDebug() << "!= NOT_EDIT";
            if(model[ i ][ STATE_ROW ] == StatesRows::ADDED)
            {
                qDebug() << "ADDED";
                query.prepare(QString("INSERT INTO %1 (idOrder, idService, number) VALUES (:idOrder, :idService, :number)").arg(table));
                query.bindValue(":idOrder", model[ i ][ ID_ORDER ]);
                query.bindValue(":idService", model[ i ][ ID_SERVICE ]);
                query.bindValue(":number", model[ i ][ NUMBER ]);
                query.exec();
            }
            else if(model[ i ][ STATE_ROW ] == StatesRows::EDITED)
            {
                qDebug() << "EDITED";
                query.prepare(QString("UPDATE %1 SET number = :number WHERE idOrder = :idOrder AND idService = :idService").arg(table));
                query.bindValue(":idOrder", model[ i ][ ID_ORDER ]);
                query.bindValue(":idService", model[ i ][ ID_SERVICE ]);
                query.bindValue(":number", model[ i ][ NUMBER ]);

                query.exec();
            }
            else if(model[ i ][ STATE_ROW ] == StatesRows::DELETED)
            {
                qDebug() << "DELETED";
                query.prepare(QString("DELETE FROM WHERE idOrder = :idOrder AND idService = :idService").arg(table));
                query.bindValue(":idOrder", model[ i ][ ID_ORDER ]);
                query.bindValue(":idService", model[ i ][ ID_SERVICE ]);

                query.exec();
            }
        }
    }

    qDebug() << query.lastError();

    return true;
}

void OrdersServicesModel::setTable(QString t, QSqlDatabase *database)
{
    table = t;
    db = database;
}

QList <orderServiceObject> OrdersServicesModel::getDataById(int idOrder)
{
    QList <orderServiceObject> result;

    for(int i = 0; i < model.size(); i++)
    {
        if(model[i][ID_ORDER].toInt() == idOrder)
            result.append(model[i]);
    }

    return result;
}

bool OrdersServicesModel::setData( const QModelIndex& index, const QVariant& value, int role ) {
    if( !index.isValid() || role != Qt::EditRole || model.count() <= index.row() ) {
        return false;
    }

    model[ index.row() ][ Column( index.column() ) ] = value;
    emit dataChanged( index, index );

    return true;
}
