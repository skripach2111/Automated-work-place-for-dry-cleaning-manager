#include "servicesmodel.h"

ServicesModel::ServicesModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

int ServicesModel::rowCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return model.count();
}

int ServicesModel::columnCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return LAST;
}

QVariant ServicesModel::headerData( int section, Qt::Orientation orientation, int role ) const
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
    case ID_TYPE:
        return "Тип послуги";
    case ID_IMAGE:
        return "Зображення";
    case DESCRIPTION:
        return "Опис";
    case COST:
        return "Вартість";
    }

    return QVariant();
}

QVariant ServicesModel::data( const QModelIndex& index, int role ) const {
    if(!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
    {
        if(index.column() == ID_TYPE)
            return typeModel->getDataById(model[index.row()][ID_TYPE].toInt())[TypeOfServicesModel::Column::TITLE];
        return model[ index.row() ][ Column( index.column() ) ];
    }
    case ServicesModel::Role::Read:
    {
        return model[ index.row() ][ Column( index.column() ) ];
    }
    case Qt::DecorationRole: //image
    {
        if(index.column() == TITLE)
        {
            QPixmap pixmap;
            pixmap.loadFromData(fileModel->getDataById(model[ index.row() ][ ID_IMAGE  ].toInt())[FilesModel::Column::FILE].toByteArray());
            pixmap = pixmap.scaled(25, 25, Qt::KeepAspectRatio);
            return QVariant(pixmap);
        }
        return QVariant();
    }
    default:
    {
        return QVariant();
    }
    }
}

Qt::ItemFlags ServicesModel::flags( const QModelIndex& index ) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );


    return flags;
}

void ServicesModel::appendRow( const serviceObject& newService ) {
    serviceObject record;
    record[ ID ] = newService[ ID ];
    record[ TITLE ] = newService[ TITLE ];
    record[ ID_TYPE ] = newService[ ID_TYPE ];
    record[ ID_IMAGE ] = newService[ ID_IMAGE ];
    record[ DESCRIPTION ] = newService[ DESCRIPTION ];
    record[ COST ] = newService[ COST ];
    record[ STATE_ROW ] = (int)StatesRows::ADDED;

    int row = model.count();
    beginInsertRows( QModelIndex(), row, row );
    model.append( record );
    endInsertRows();
}

void ServicesModel::updateRow( int row, const serviceObject& service )
{
    beginResetModel();

    model[ row ][ ID ] = service[ ID ];
    model[ row ][ TITLE ] = service[ TITLE ];
    model[ row ][ ID_TYPE ] = service[ ID_TYPE ];
    model[ row ][ ID_IMAGE ] = service[ ID_IMAGE ];
    model[ row ][ DESCRIPTION ] = service[ DESCRIPTION ];
    model[ row ][ COST ] = service[ COST ];
    model[ row ][ STATE_ROW ] = (int)StatesRows::EDITED;

    endResetModel();
}

void ServicesModel::removeRow(int row)
{
    model[ row ][ IS_DELETED ] = 1;
    model[ row ][ STATE_ROW ] = StatesRows::DELETED;
}

bool ServicesModel::select()
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

        serviceObject record;
        do
        {
            record[ ID ] = query.value( ID );
            record[ TITLE ] = query.value( TITLE );
            record[ ID_TYPE ] = query.value( ID_TYPE );
            record[ ID_IMAGE ] = query.value( ID_IMAGE );
            record[ DESCRIPTION ] = query.value( DESCRIPTION );
            record[ COST ] = query.value( COST );
            record[ IS_DELETED ] = query.value( IS_DELETED );
            record[ STATE_ROW ] = StatesRows::NOT_EDITED;

            model.append( record );

        }while(query.next());

        endInsertRows();
    }

    endResetModel();

    return false;
}


bool ServicesModel::saveChanges()
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[ i ][ STATE_ROW ] != StatesRows::NOT_EDITED)
        {
            qDebug() << "!= NOT_EDIT";
            if(model[ i ][ STATE_ROW ] == StatesRows::ADDED)
            {
                qDebug() << "ADDED";
                query.prepare(QString("INSERT INTO %1 (title, idType, idImage, description, cost, isDelete) VALUES (:title, :idType, :idImage, :description, :cost, 0)").arg(table));
                query.bindValue(":title", model[ i ][ TITLE ]);
                query.bindValue(":idType", model[ i ][ ID_TYPE ]);
                query.bindValue(":idImage", model[ i ][ ID_IMAGE ]);
                query.bindValue(":description", model[ i ][ DESCRIPTION ]);
                query.bindValue(":cost", model[ i ][ COST ]);
                query.exec();
            }
            else if(model[ i ][ STATE_ROW ] == StatesRows::EDITED)
            {
                qDebug() << "EDITED";
                query.prepare(QString("UPDATE %1 SET title = :title, idType = :idType, idImage = :idImage, description = :description, cost = :cost WHERE id = :id").arg(table));
                query.bindValue(":title", model[ i ][ TITLE ]);
                query.bindValue(":idType", model[ i ][ ID_TYPE ]);
                query.bindValue(":idImage", model[ i ][ ID_IMAGE ]);
                query.bindValue(":description", model[ i ][ DESCRIPTION ]);
                query.bindValue(":cost", model[ i ][ COST ]);
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

void ServicesModel::setTable(QString t, QSqlDatabase *database)
{
    table = t;
    db = database;
}

QVariant ServicesModel::getDataById(int id, Column column)
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[i][ID].toInt() == id)
            return model[i][column];
    }

    return QVariant();
}

int ServicesModel::getRowByData(Column column, QVariant data)
{
    for(int i = 0; i < model.size(); i++)
        if(model[i][column] == data)
            return i;
}

ServicesModel::serviceObject ServicesModel::getObjectByRow(int row)
{
    return model[row];
}

bool ServicesModel::setData( const QModelIndex& index, const QVariant& value, int role ) {
    if( !index.isValid() || role != Qt::EditRole || model.count() <= index.row() ) {
        return false;
    }

    model[ index.row() ][ Column( index.column() ) ] = value;
    emit dataChanged( index, index );

    return true;
}
