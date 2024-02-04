#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(!dbm.connect())
    {
        QMessageBox::critical(nullptr, "Критична помилка!", "Неможливо під'єднатися до бази даних.\nПрограму буде завершено.");
        close();
    }

    ui->stackedWidget->setCurrentIndex(PAGES::HELLO);
    dbm.selectTables();

    clientsFilterModel = new ClientsFilterModel(this);
    managerFilterModel = new ManagerFilterModel(this);
    typeOfServicesFilterModel = new TypeOfServicesFilterModel(this);
    servicesFilterModel = new ServicesFilterModel(this);
    ordersFilterModel = new OrderFilterModel(this);
    statusesFilterModel = new StatusesFilterModel(this);

    clientsFilterModel->setSourceModel(dbm.clientsModel);
    managerFilterModel->setSourceModel(dbm.managersModel);
    typeOfServicesFilterModel->setSourceModel(dbm.typeOfServicesModel);
    servicesFilterModel->setSourceModel(dbm.servicesModel);
    ordersFilterModel->setSourceModel(dbm.ordersModel);
    statusesFilterModel->setSourceModel(dbm.statusesModel);

    ui->tableView_clients->setModel(clientsFilterModel);
    ui->tableView_managers->setModel(managerFilterModel);
    ui->listView_types_of_services->setModel(typeOfServicesFilterModel);
    ui->tableView_services->setModel(servicesFilterModel);
    ui->tableView_orders->setModel(ordersFilterModel);
    ui->listView_statuses->setModel(statusesFilterModel);

    ui->comboBox_clients_orders_search->setModel(clientsFilterModel);
    ui->comboBox_type_services_search->setModel(typeOfServicesFilterModel);
    ui->comboBox_status_orders_search->setModel(statusesFilterModel);

    ui->tableView_zap_1->setModel(dbm.zap1_model);
    ui->tableView_zap_2->setModel(dbm.zap2_model);
    ui->tableView_zap_3->setModel(dbm.zap3_model);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clients_clicked()
{
    ui->stackedWidget->setCurrentIndex(PAGES::CLIENTS);
}


void MainWindow::on_pushButton_managers_clicked()
{
    ui->stackedWidget->setCurrentIndex(PAGES::MANAGERS);
}


void MainWindow::on_pushButton_type_of_services_clicked()
{
    ui->stackedWidget->setCurrentIndex(PAGES::TYPES_OF_SERVICES);
}


void MainWindow::on_pushButton_services_clicked()
{
    ui->stackedWidget->setCurrentIndex(PAGES::SERVICES);
}


void MainWindow::on_pushButton_orders_clicked()
{
    ui->stackedWidget->setCurrentIndex(PAGES::ORDERS);
}


void MainWindow::on_pushButton_statuses_clicked()
{
    ui->stackedWidget->setCurrentIndex(PAGES::STATUSES);
}


void MainWindow::on_pushButton_clients_search_clicked()
{
    if(ui->lineEdit_pib_clients_search->text().size() > 0)
    {
        clientsFilterModel->setEnabledFilterParam(ClientsFilterModel::FilterParam::PIB, true);
        clientsFilterModel->setFilterParam(ClientsFilterModel::FilterParam::PIB, ui->lineEdit_pib_clients_search->text());
    }
    else
        clientsFilterModel->setEnabledFilterParam(ClientsFilterModel::FilterParam::PIB, false);

    if(ui->lineEdit_phoneNumber_clients_search->text().size() > 0)
    {
        clientsFilterModel->setEnabledFilterParam(ClientsFilterModel::FilterParam::PHONE_NUMBER, true);
        clientsFilterModel->setFilterParam(ClientsFilterModel::FilterParam::PHONE_NUMBER, ui->lineEdit_phoneNumber_clients_search->text());
    }
    else
        clientsFilterModel->setEnabledFilterParam(ClientsFilterModel::FilterParam::PHONE_NUMBER, false);

    if(ui->lineEdit_email_clients_search->text().size() > 0)
    {
        clientsFilterModel->setEnabledFilterParam(ClientsFilterModel::FilterParam::EMAIL, true);
        clientsFilterModel->setFilterParam(ClientsFilterModel::FilterParam::EMAIL, ui->lineEdit_email_clients_search->text());
    }
    else
        clientsFilterModel->setEnabledFilterParam(ClientsFilterModel::FilterParam::EMAIL, false);
}


void MainWindow::on_pushButton_managers_search_clicked()
{
    if(ui->lineEdit_pib_managers_search->text().size() > 0)
    {
        managerFilterModel->setEnabledFilterParam(ManagerFilterModel::FilterParam::PIB, true);
        managerFilterModel->setFilterParam(ManagerFilterModel::FilterParam::PIB, ui->lineEdit_pib_managers_search->text());
    }
    else
        managerFilterModel->setEnabledFilterParam(ManagerFilterModel::FilterParam::PIB, false);

    if(ui->lineEdit_phoneNumber_managers_search->text().size() > 0)
    {
        managerFilterModel->setEnabledFilterParam(ManagerFilterModel::FilterParam::PHONE_NUMBER, true);
        managerFilterModel->setFilterParam(ManagerFilterModel::FilterParam::PHONE_NUMBER, ui->lineEdit_phoneNumber_managers_search->text());
    }
    else
        managerFilterModel->setEnabledFilterParam(ManagerFilterModel::FilterParam::PHONE_NUMBER, false);
}


void MainWindow::on_pushButton_types_of_services_search_clicked()
{
    if(ui->lineEdit_title_types_of_services_search->text().size() > 0)
    {
        typeOfServicesFilterModel->setEnabledFilterParam(TypeOfServicesFilterModel::FilterParam::TITLE, true);
        typeOfServicesFilterModel->setFilterParam(TypeOfServicesFilterModel::FilterParam::TITLE, ui->lineEdit_title_types_of_services_search->text());
    }
    else
        typeOfServicesFilterModel->setEnabledFilterParam(TypeOfServicesFilterModel::FilterParam::TITLE, false);
}


void MainWindow::on_pushButton_services_search_clicked()
{
    if(ui->lineEdit_title_services_search->text().size() > 0)
    {
        servicesFilterModel->setEnabledFilterParam(ServicesFilterModel::FilterParam::TITLE, true);
        servicesFilterModel->setFilterParam(ServicesFilterModel::FilterParam::TITLE, ui->lineEdit_title_services_search->text());
    }
    else
        servicesFilterModel->setEnabledFilterParam(ServicesFilterModel::FilterParam::TITLE, false);

    if(ui->checkBox_enable_type_combo->isChecked())
    {
        servicesFilterModel->setEnabledFilterParam(ServicesFilterModel::FilterParam::TYPE, true);
        servicesFilterModel->setFilterParam(ServicesFilterModel::FilterParam::TYPE, dbm.typeOfServicesModel->index(typeOfServicesFilterModel->mapToSource(typeOfServicesFilterModel->index(ui->comboBox_type_services_search->currentIndex(), 0)).row(), TypeOfServicesModel::Column::ID).data(TypeOfServicesModel::Role::Read));
    }
    else
        servicesFilterModel->setEnabledFilterParam(ServicesFilterModel::FilterParam::TYPE, false);
}


void MainWindow::on_pushButton_orders_search_clicked()
{
    if(ui->lineEdit_number_orders_search->text().size() > 0)
    {
        ordersFilterModel->setEnabledFilterParam(OrderFilterModel::FilterParam::NUMBER, true);
        ordersFilterModel->setFilterParam(OrderFilterModel::FilterParam::NUMBER, ui->lineEdit_number_orders_search->text());
    }
    else
        ordersFilterModel->setEnabledFilterParam(OrderFilterModel::FilterParam::NUMBER, false);

    if(ui->checkBox_enabled_clients_combo->isChecked())
    {
        ordersFilterModel->setEnabledFilterParam(OrderFilterModel::FilterParam::CLIENT, true);
        ordersFilterModel->setFilterParam(OrderFilterModel::FilterParam::CLIENT, dbm.clientsModel->index(clientsFilterModel->mapToSource(clientsFilterModel->index(ui->comboBox_clients_orders_search->currentIndex(), 0)).row(), ClientsModel::Column::ID).data(ClientsModel::Role::Read));
    }
    else
        ordersFilterModel->setEnabledFilterParam(OrderFilterModel::FilterParam::CLIENT, false);

    if(ui->checkBox_enable_statuses_combo->isChecked())
    {
        ordersFilterModel->setEnabledFilterParam(OrderFilterModel::FilterParam::STATUS, true);
        ordersFilterModel->setFilterParam(OrderFilterModel::FilterParam::STATUS, dbm.statusesModel->index(statusesFilterModel->mapToSource(statusesFilterModel->index(ui->comboBox_status_orders_search->currentIndex(), 0)).row(), StatusesModel::Column::ID).data(StatusesModel::Role::Read));
    }
    else
        ordersFilterModel->setEnabledFilterParam(OrderFilterModel::FilterParam::STATUS, false);
}


void MainWindow::on_pushButton_statuses_search_clicked()
{
    if(ui->lineEdit_title_statuses_search->text().size() > 0)
    {
        statusesFilterModel->setEnabledFilterParam(StatusesFilterModel::FilterParam::TITLE, true);
        statusesFilterModel->setFilterParam(StatusesFilterModel::FilterParam::TITLE, ui->lineEdit_title_statuses_search->text());
    }
    else
        statusesFilterModel->setEnabledFilterParam(StatusesFilterModel::FilterParam::TITLE, false);
}

void MainWindow::on_pushButton_exit_clicked()
{
    close();
}


void MainWindow::on_pushButton_clients_add_clicked()
{
    AddEditClientsDialog dialog;

    dialog.exec();

    if(!dialog.isSave())
        return;

    dbm.clientsModel->appendRow(dialog.getData());
    dbm.clientsModel->saveChanges();
    dbm.clientsModel->select();
}


void MainWindow::on_pushButton_clients_edit_clicked()
{
    if(ui->tableView_clients->currentIndex().row() == -1)
        return;

    AddEditClientsDialog dialog;

    dialog.setDialog(dbm.clientsModel->getObjectByRow(clientsFilterModel->mapToSource(ui->tableView_clients->currentIndex()).row()));
    dialog.exec();

    if(!dialog.isSave())
        return;

    dbm.clientsModel->updateRow(clientsFilterModel->mapToSource(ui->tableView_clients->currentIndex()).row(), dialog.getData());
    dbm.clientsModel->saveChanges();
    dbm.clientsModel->select();
}


void MainWindow::on_pushButton_clients_del_clicked()
{
    if(ui->tableView_clients->currentIndex().row() == -1)
        return;

    dbm.clientsModel->removeRow(clientsFilterModel->mapToSource(ui->tableView_clients->currentIndex()).row());
    dbm.clientsModel->saveChanges();
    dbm.clientsModel->select();
}


void MainWindow::on_pushButton_managers_add_clicked()
{
    AddEditManagersDialog dialog;

    dialog.exec();

    if(!dialog.isSave())
        return;

    dbm.managersModel->appendRow(dialog.getData());
    dbm.managersModel->saveChanges();
    dbm.managersModel->select();
}


void MainWindow::on_pushButton_managers_edit_clicked()
{
    if(ui->tableView_managers->currentIndex().row() == -1)
        return;

    AddEditManagersDialog dialog;

    dialog.setDialog(dbm.managersModel->getObjectByRow(managerFilterModel->mapToSource(ui->tableView_managers->currentIndex()).row()));
    dialog.exec();

    if(!dialog.isSave())
        return;

    dbm.managersModel->updateRow(managerFilterModel->mapToSource(ui->tableView_managers->currentIndex()).row(), dialog.getData());
    dbm.managersModel->saveChanges();
    dbm.managersModel->select();
}


void MainWindow::on_pushButton_managers_del_clicked()
{
    if(ui->tableView_managers->currentIndex().row() == -1)
        return;

    dbm.managersModel->removeRow(managerFilterModel->mapToSource(ui->tableView_managers->currentIndex()).row());
    dbm.managersModel->saveChanges();
    dbm.managersModel->select();
}


void MainWindow::on_pushButton_types_of_services_add_clicked()
{
    AddEditTypesOfServicesDialog dialog;

    dialog.exec();

    if(!dialog.isSave())
        return;

    dbm.typeOfServicesModel->appendRow(dialog.getData());
    dbm.typeOfServicesModel->saveChanges();
    dbm.typeOfServicesModel->select();
}


void MainWindow::on_pushButton_ypes_of_services_edit_clicked()
{
    if(ui->listView_types_of_services->currentIndex().row() == -1)
        return;

    AddEditTypesOfServicesDialog dialog;

    dialog.setDialog(dbm.typeOfServicesModel->getObjectByRow(typeOfServicesFilterModel->mapToSource(ui->listView_types_of_services->currentIndex()).row()));
    dialog.exec();

    if(!dialog.isSave())
        return;

    dbm.typeOfServicesModel->updateRow(typeOfServicesFilterModel->mapToSource(ui->listView_types_of_services->currentIndex()).row(), dialog.getData());
    dbm.typeOfServicesModel->saveChanges();
    dbm.typeOfServicesModel->select();
}


void MainWindow::on_pushButton_types_of_services_del_clicked()
{
    if(ui->listView_types_of_services->currentIndex().row() == -1)
        return;

    dbm.typeOfServicesModel->removeRow(typeOfServicesFilterModel->mapToSource(ui->listView_types_of_services->currentIndex()).row());
    dbm.typeOfServicesModel->saveChanges();
    dbm.typeOfServicesModel->select();
}


void MainWindow::on_pushButton_services_add_clicked()
{
    AddEditServicesDialog dialog;

    dialog.setDialog(dbm.filesModel, dbm.typeOfServicesModel);
    dialog.exec();

    if(!dialog.isSave())
        return;

    dbm.servicesModel->appendRow(dialog.getData());
    dbm.servicesModel->saveChanges();
    dbm.servicesModel->select();
}


void MainWindow::on_pushButton_services_edit_clicked()
{
    if(ui->tableView_services->currentIndex().row() == -1)
        return;

    AddEditServicesDialog dialog;

    dialog.setDialog(dbm.servicesModel->getObjectByRow(servicesFilterModel->mapToSource(ui->tableView_services->currentIndex()).row()), dbm.filesModel, dbm.typeOfServicesModel);
    dialog.exec();

    if(!dialog.isSave())
        return;

    dbm.servicesModel->updateRow(servicesFilterModel->mapToSource(ui->tableView_services->currentIndex()).row(), dialog.getData());
    dbm.servicesModel->saveChanges();
    dbm.servicesModel->select();
}


void MainWindow::on_pushButton_services_del_clicked()
{
    if(ui->tableView_services->currentIndex().row() == -1)
        return;

    dbm.servicesModel->removeRow(servicesFilterModel->mapToSource(ui->tableView_services->currentIndex()).row());
    dbm.servicesModel->saveChanges();
    dbm.servicesModel->select();
}


void MainWindow::on_pushButton_orders_add_clicked()
{
    AddEditOrdersDialog dialog;

    dialog.setDialog(dbm.clientsModel, dbm.managersModel, dbm.servicesModel, dbm.ordersServicesModel, dbm.statusesModel);
    dialog.exec();

    if(!dialog.isSave())
        return;

    dbm.ordersModel->appendRow(dialog.getOrderData());
    QVariant number = dbm.ordersModel->saveChanges();
    dbm.ordersModel->select();

    QList<orderServiceObject> listServices = dialog.getOrderServicesData();

    for(int i = 0; i < listServices.size(); i++)
        listServices[i][OrdersServicesModel::Column::ID_ORDER] = number;

    for(int i = 0; i < listServices.size(); i++)
        dbm.ordersServicesModel->appendRow(listServices[i]);

    dbm.ordersServicesModel->saveChanges();
    dbm.ordersServicesModel->select();
}


void MainWindow::on_pushButton_orders_edit_clicked()
{
    if(ui->tableView_orders->currentIndex().row() == -1)
        return;

    AddEditOrdersDialog dialog;

    dialog.setDialog(dbm.ordersModel->getObjectByRow(ordersFilterModel->mapToSource(ui->tableView_orders->currentIndex()).row()), dbm.clientsModel, dbm.managersModel, dbm.servicesModel, dbm.ordersServicesModel, dbm.statusesModel);
    dialog.exec();

    if(!dialog.isSave())
        return;

    dbm.ordersModel->updateRow(ordersFilterModel->mapToSource(ui->tableView_orders->currentIndex()).row(), dialog.getOrderData());
    dbm.ordersModel->saveChanges();
    dbm.ordersModel->select();
}


void MainWindow::on_pushButton_statuses_add_clicked()
{
    AddEditStatusesDialog dialog;

    dialog.exec();

    if(!dialog.isSave())
        return;

    dbm.statusesModel->appendRow(dialog.getData());
    dbm.statusesModel->saveChanges();
    dbm.statusesModel->select();
}


void MainWindow::on_pushButton_statuses_edit_clicked()
{
    if(ui->listView_statuses->currentIndex().row() == -1)
        return;

    AddEditStatusesDialog dialog;

    dialog.setDialog(dbm.statusesModel->getObjectByRow(statusesFilterModel->mapToSource(ui->listView_statuses->currentIndex()).row()));
    dialog.exec();

    if(!dialog.isSave())
        return;

    dbm.statusesModel->updateRow(statusesFilterModel->mapToSource(ui->listView_statuses->currentIndex()).row(), dialog.getData());
    dbm.statusesModel->saveChanges();
    dbm.statusesModel->select();
}


void MainWindow::on_pushButton_statuses_del_clicked()
{
    if(ui->listView_statuses->currentIndex().row() == -1)
        return;

    dbm.statusesModel->removeRow(statusesFilterModel->mapToSource(ui->listView_statuses->currentIndex()).row());
    dbm.statusesModel->saveChanges();
    dbm.statusesModel->select();
}


void MainWindow::on_pushButton_clicked()
{
    dbm.zap1.exec();
    dbm.zap2.exec();
    dbm.zap3.exec();

    ui->stackedWidget_2->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(PAGES::STAT);
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}


void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);
}


void MainWindow::on_pushButton_4_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(2);
}

