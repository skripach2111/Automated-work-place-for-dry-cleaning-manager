#ifndef DATABASEMODULE_H
#define DATABASEMODULE_H


#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>

#include "clientsmodel.h"
#include "managersmodel.h"
#include "typeofservicesmodel.h"
#include "servicesmodel.h"
#include "filesmodel.h"
#include "ordersmodel.h"
#include "ordersservicesmodel.h"
#include "statusesmodel.h"

class DatabaseModule : public QObject
{
    Q_OBJECT

    QSqlDatabase db;

public:
    DatabaseModule(QObject *parent = nullptr);

    bool connect();
    void disconnect() { db.close(); }

    QSqlError lastError() { return db.lastError(); }

    void setDbName(QString name) { db.setDatabaseName(name); }
    void setHostAddress(QString address) { db.setHostName(address); }
    void setHostPort(int port) { db.setPort(port); }

    void selectTables();

    ClientsModel* clientsModel;
    ManagersModel* managersModel;
    TypeOfServicesModel* typeOfServicesModel;
    ServicesModel* servicesModel;
    FilesModel* filesModel;
    OrdersModel* ordersModel;
    OrdersServicesModel* ordersServicesModel;
    StatusesModel* statusesModel;

    QSqlTableModel* zap1_model;
    QSqlTableModel* zap2_model;
    QSqlTableModel* zap3_model;

    QSqlQuery zap1;
    QSqlQuery zap2;
    QSqlQuery zap3;
};

#endif // DATABASEMODULE_H
