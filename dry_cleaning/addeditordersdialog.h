#ifndef ADDEDITORDERSDIALOG_H
#define ADDEDITORDERSDIALOG_H

#include <QDialog>

#include "clientsfiltermodel.h"
#include "managersfiltermodel.h"
#include "statusesfiltermodel.h"
#include "servicesfiltermodel.h"
#include "selectservicedialog.h"
#include "ordersservicesmodel.h"
#include "ordersmodel.h"

namespace Ui {
class AddEditOrdersDialog;
}

class AddEditOrdersDialog : public QDialog
{
    Q_OBJECT

    ClientsModel* clients;
    ClientsFilterModel* clientsFilter;
    ManagersModel* managers;
    ManagerFilterModel* managersFilter;
    ServicesModel* services;
    OrdersServicesModel* ordersServices;
    StatusesModel* statuses;
    StatusesFilterModel* statusesFilter;

    QList<orderServiceObject> orderServiceList;

    orderObject order;

    bool save = false;
    bool isEdit = false;

public:
    explicit AddEditOrdersDialog(QWidget *parent = nullptr);
    ~AddEditOrdersDialog();

    void setDialog(orderObject data, ClientsModel* clientsModel, ManagersModel* managersModel, ServicesModel* servicesModel, OrdersServicesModel* ordersServicesModel, StatusesModel* statusesModel);
    void setDialog(ClientsModel* clientsModel, ManagersModel* managersModel, ServicesModel* servicesModel, OrdersServicesModel* ordersServicesModel, StatusesModel* statusesModel);

    orderObject getOrderData();
    QList<orderServiceObject> getOrderServicesData();

    bool isSave();

private slots:
    void on_pushButton_services_add_clicked();

    void on_pushButton_services_del_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::AddEditOrdersDialog *ui;

    void ReWriteTableServices();
};

#endif // ADDEDITORDERSDIALOG_H
