#include "databasemodule.h"

#include <QDebug>

DatabaseModule::DatabaseModule(QObject *parent)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("dry_cleaning");
    db.setUserName("dry_cleaning_user");
    db.setPassword("dry_cleaning_user");
}

bool DatabaseModule::connect()
{
    if(db.open())
    {
        clientsModel = new ClientsModel(this);
        clientsModel->setTable("clients", &db);

        managersModel = new ManagersModel(this);
        managersModel->setTable("managers", &db);

        typeOfServicesModel = new TypeOfServicesModel(this);
        typeOfServicesModel->setTable("types_of_services", &db);

        filesModel = new FilesModel(this);
        filesModel->setTable("files", &db);

        servicesModel = new ServicesModel(this);
        servicesModel->setTable("services", &db);
        servicesModel->typeModel = typeOfServicesModel;
        servicesModel->fileModel = filesModel;

        statusesModel = new StatusesModel(this);
        statusesModel->setTable("statuses", &db);

        ordersModel = new OrdersModel(this);
        ordersModel->setTable("orders", &db);
        ordersModel->clients = clientsModel;
        ordersModel->managers = managersModel;
        ordersModel->statuses = statusesModel;

        ordersServicesModel = new OrdersServicesModel(this);
        ordersServicesModel->setTable("orders_services", &db);

        zap1 = QSqlQuery("SELECT s.title AS 'Послуга', COUNT(os.idOrder) AS 'Кількість замовлень'\
                            FROM services s\
                            JOIN orders_services os ON s.id = os.idService\
                            JOIN orders o ON os.idOrder = o.number\
                            WHERE s.isDelete = 0 AND o.isDelete = 0\
                            GROUP BY s.id\
                            ORDER BY COUNT(os.idOrder) DESC;", db);
        zap2 = QSqlQuery("SELECT ts.title AS 'Типи послуг', COUNT(os.idOrder) AS 'Кількість замовлень'\
                            FROM types_of_services ts\
                            JOIN services s ON ts.id = s.idType\
                            JOIN orders_services os ON s.id = os.idService\
                            JOIN orders o ON os.idOrder = o.number\
                            WHERE s.isDelete = 0 AND o.isDelete = 0\
                            GROUP BY ts.id\
                            ORDER BY COUNT(os.idOrder) DESC;", db);
        zap3 = QSqlQuery("SELECT c.pib AS 'ПІБ Клієнта', COUNT(o.number) AS 'Кількість замовлень', SUM(o.total_cost) AS 'Загаьна сума замовлень'\
                            FROM clients c\
                            JOIN orders o ON c.id = o.idClient\
                            WHERE c.isDelete = 0 AND o.isDelete = 0\
                            GROUP BY c.id\
                            ORDER BY COUNT(o.number) DESC;", db);

        zap1_model = new QSqlTableModel(this);
        zap2_model = new QSqlTableModel(this);
        zap3_model = new QSqlTableModel(this);

        zap1_model->setQuery(zap1);
        zap2_model->setQuery(zap2);
        zap3_model->setQuery(zap3);

        return true;
    }
    else
        return false;
}

void DatabaseModule::selectTables()
{
    clientsModel->select();
    managersModel->select();
    typeOfServicesModel->select();
    servicesModel->select();
    filesModel->select();
    ordersModel->select();
    ordersServicesModel->select();
    statusesModel->select();
}


