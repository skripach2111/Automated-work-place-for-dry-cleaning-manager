#include "ordersmodel.h"

OrdersModel::OrdersModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

int OrdersModel::rowCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return model.count();
}

int OrdersModel::columnCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return LAST;
}

QVariant OrdersModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if( orientation == Qt::Vertical ) {
        return section;
    }

    switch( section ) {
    case NUMBER:
        return "Номер замовлення";
    case ID_CLIENT:
        return "ПІБ клієнта";
    case ID_MANAGER:
        return "ПІБ менеджера";
    case DATE_TIME_ORDER:
        return "Час створення замовлення";
    case DATE_OF_COMPLETION:
        return "Дата виконання";
    case TOTAL_COST:
        return "Загальна вартість";
    case ID_STATUS:
        return "Статус";
    case COMMENT:
        return "Коментар";
    }

    return QVariant();
}

QVariant OrdersModel::data( const QModelIndex& index, int role ) const {
    if(!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
    {
        if(index.column() == ID_CLIENT)
            return clients->getDataById(model[index.row()][ID_CLIENT].toInt(), ClientsModel::Column::PIB);
        else if(index.column() == ID_MANAGER)
            return managers->getDataById(model[index.row()][ID_MANAGER].toInt(), ManagersModel::Column::PIB);
        else if(index.column() == ID_STATUS)
            return statuses->getDataById(model[index.row()][ID_STATUS].toInt(), StatusesModel::Column::TITLE);
        else
            return model[ index.row() ][ Column( index.column() ) ];
    }
    case OrdersModel::Role::Read:
    {
        return model[ index.row() ][ Column( index.column() ) ];
    }
    default:
    {
        return QVariant();
    }
    }
}

Qt::ItemFlags OrdersModel::flags( const QModelIndex& index ) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );


    return flags;
}

void OrdersModel::appendRow( const orderObject& newOrder ) {
    orderObject record;
    record[ NUMBER ] = newOrder[ NUMBER ];
    record[ ID_CLIENT ] = newOrder[ ID_CLIENT ];
    record[ ID_MANAGER ] = newOrder[ ID_MANAGER ];
    record[ DATE_TIME_ORDER ] = newOrder[ DATE_TIME_ORDER ];
    record[ DATE_OF_COMPLETION ] = newOrder[ DATE_OF_COMPLETION ];
    record[ TOTAL_COST ] = newOrder[ TOTAL_COST ];
    record[ ID_STATUS ] = newOrder[ ID_STATUS ];
    record[ COMMENT ] = newOrder[ COMMENT ];
    record[ STATE_ROW ] = (int)StatesRows::ADDED;

    int row = model.count();
    beginInsertRows( QModelIndex(), row, row );
    model.append( record );
    endInsertRows();
}

void OrdersModel::updateRow( int row, const orderObject& order )
{
    beginResetModel();

    model[ row ][ NUMBER ] = order[ NUMBER ];
    model[ row ][ ID_CLIENT ] = order[ ID_CLIENT ];
    model[ row ][ ID_MANAGER ] = order[ ID_MANAGER ];
    model[ row ][ DATE_TIME_ORDER ] = order[ DATE_TIME_ORDER ];
    model[ row ][ DATE_OF_COMPLETION ] = order[ DATE_OF_COMPLETION ];
    model[ row ][ TOTAL_COST ] = order[ TOTAL_COST ];
    model[ row ][ ID_STATUS ] = order[ ID_STATUS ];
    model[ row ][ COMMENT ] = order[ COMMENT ];
    model[ row ][ STATE_ROW ] = (int)StatesRows::EDITED;

    endResetModel();
}

void OrdersModel::removeRow(int row)
{
    model[ row ][ IS_DELETED ] = 1;
    model[ row ][ STATE_ROW ] = StatesRows::DELETED;
}

bool OrdersModel::select()
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

        orderObject record;
        do
        {
            record[ NUMBER ] = query.value( NUMBER );
            record[ ID_CLIENT ] = query.value( ID_CLIENT );
            record[ ID_MANAGER ] = query.value( ID_MANAGER );
            record[ DATE_TIME_ORDER ] = query.value( DATE_TIME_ORDER );
            record[ DATE_OF_COMPLETION ] = query.value( DATE_OF_COMPLETION );
            record[ TOTAL_COST ] = query.value( TOTAL_COST );
            record[ ID_STATUS ] = query.value( ID_STATUS );
            record[ COMMENT ] = query.value( COMMENT );
            record[ IS_DELETED ] = query.value( IS_DELETED );
            record[ STATE_ROW ] = StatesRows::NOT_EDITED;

            model.append( record );

        }while(query.next());

        endInsertRows();
    }

    endResetModel();

    return false;
}


QVariant OrdersModel::saveChanges()
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[ i ][ STATE_ROW ] != StatesRows::NOT_EDITED)
        {
            qDebug() << "!= NOT_EDIT";
            if(model[ i ][ STATE_ROW ] == StatesRows::ADDED)
            {
                qDebug() << "ADDED";
                query.prepare(QString("INSERT INTO %1 (idClient, idManager, dateTime_order, date_of_completion, total_cost, idStatus, comment, isDelete) "
                                      "VALUES (:idClient, :idManager, :dateTime_order, :date_of_completion, :total_cost, :idStatus, :comment, 0)").arg(table));
                query.bindValue(":idClient", model[ i ][ ID_CLIENT ]);
                query.bindValue(":idManager", model[ i ][ ID_MANAGER ]);
                query.bindValue(":dateTime_order", model[ i ][ DATE_TIME_ORDER ]);
                query.bindValue(":date_of_completion", model[ i ][ DATE_OF_COMPLETION ]);
                query.bindValue(":total_cost", model[ i ][ TOTAL_COST ]);
                query.bindValue(":idStatus", model[ i ][ ID_STATUS ]);
                query.bindValue(":comment", model[ i ][ COMMENT ]);
                query.exec();
            }
            else if(model[ i ][ STATE_ROW ] == StatesRows::EDITED)
            {
                qDebug() << "EDITED";
                query.prepare(QString("UPDATE %1 SET idClient = :idClient, idManager = :idManager, dateTime_order = :dateTime_order,"
                                      " date_of_completion = :date_of_completion, total_cost = :total_cost,"
                                      " idStatus = :idStatus, comment = :comment WHERE number = :number").arg(table));
                query.bindValue(":idClient", model[ i ][ ID_CLIENT ]);
                query.bindValue(":idManager", model[ i ][ ID_MANAGER ]);
                query.bindValue(":dateTime_order", model[ i ][ DATE_TIME_ORDER ]);
                query.bindValue(":date_of_completion", model[ i ][ DATE_OF_COMPLETION ]);
                query.bindValue(":total_cost", model[ i ][ TOTAL_COST ]);
                query.bindValue(":idStatus", model[ i ][ ID_STATUS ]);
                query.bindValue(":comment", model[ i ][ COMMENT ]);
                query.bindValue(":number", model[ i ][ NUMBER ]);

                query.exec();
            }
            else if(model[ i ][ STATE_ROW ] == StatesRows::DELETED)
            {
                qDebug() << "DELETED";
                query.prepare(QString("UPDATE %1 SET isDelete = 1 WHERE number = :number").arg(table));
                query.bindValue(":number", model[ i ][ NUMBER ]);

                query.exec();
            }
        }
    }

    qDebug() << query.lastError();

    query.prepare("SELECT LAST_INSERT_ID();");
    query.exec();
    query.next();

    return query.value(0);
}

void OrdersModel::setTable(QString t, QSqlDatabase *database)
{
    table = t;
    db = database;
}

QVariant OrdersModel::getDataById(int id, Column column)
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[i][NUMBER].toInt() == id)
            return model[i][column];
    }

    return QVariant();
}

OrdersModel::orderObject OrdersModel::getObjectByRow(int row)
{
    return model[row];
}

bool OrdersModel::setData( const QModelIndex& index, const QVariant& value, int role ) {
    if( !index.isValid() || role != Qt::EditRole || model.count() <= index.row() ) {
        return false;
    }

    model[ index.row() ][ Column( index.column() ) ] = value;
    emit dataChanged( index, index );

    return true;
}
