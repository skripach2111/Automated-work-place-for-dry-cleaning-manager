#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "databasemodule.h"

#include "clientsfiltermodel.h"
#include "managersfiltermodel.h"
#include "typeofservicesfiltermodel.h"
#include "servicesfiltermodel.h"
#include "ordersfiltermodel.h"
#include "statusesfiltermodel.h"

#include "addeditclientsdialog.h"
#include "addeditmanagersdialog.h"
#include "addedittypesofservicesdialog.h"
#include "addeditservicesdialog.h"
#include "addeditordersdialog.h"
#include "addeditstatusesdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    DatabaseModule dbm;

    ClientsFilterModel* clientsFilterModel;
    ManagerFilterModel* managerFilterModel;
    TypeOfServicesFilterModel* typeOfServicesFilterModel;
    ServicesFilterModel* servicesFilterModel;
    OrderFilterModel* ordersFilterModel;
    StatusesFilterModel* statusesFilterModel;

    enum PAGES {
        HELLO = 0,
        CLIENTS,
        MANAGERS,
        TYPES_OF_SERVICES,
        SERVICES,
        ORDERS,
        STATUSES,
        STAT
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clients_clicked();

    void on_pushButton_managers_clicked();

    void on_pushButton_type_of_services_clicked();

    void on_pushButton_services_clicked();

    void on_pushButton_orders_clicked();

    void on_pushButton_statuses_clicked();

    void on_pushButton_clients_search_clicked();

    void on_pushButton_managers_search_clicked();

    void on_pushButton_types_of_services_search_clicked();

    void on_pushButton_services_search_clicked();

    void on_pushButton_orders_search_clicked();

    void on_pushButton_statuses_search_clicked();

    void on_pushButton_exit_clicked();

    void on_pushButton_clients_add_clicked();

    void on_pushButton_clients_edit_clicked();

    void on_pushButton_clients_del_clicked();

    void on_pushButton_managers_add_clicked();

    void on_pushButton_managers_edit_clicked();

    void on_pushButton_managers_del_clicked();

    void on_pushButton_types_of_services_add_clicked();

    void on_pushButton_ypes_of_services_edit_clicked();

    void on_pushButton_types_of_services_del_clicked();

    void on_pushButton_services_add_clicked();

    void on_pushButton_services_edit_clicked();

    void on_pushButton_services_del_clicked();

    void on_pushButton_orders_add_clicked();

    void on_pushButton_orders_edit_clicked();

    void on_pushButton_statuses_add_clicked();

    void on_pushButton_statuses_edit_clicked();

    void on_pushButton_statuses_del_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
