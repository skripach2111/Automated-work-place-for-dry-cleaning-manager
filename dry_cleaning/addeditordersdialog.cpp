#include "addeditordersdialog.h"
#include "ui_addeditordersdialog.h"

AddEditOrdersDialog::AddEditOrdersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEditOrdersDialog)
{
    ui->setupUi(this);

    clientsFilter = new ClientsFilterModel(this);
    ui->comboBox_client->setModel(clientsFilter);
    managersFilter = new ManagerFilterModel(this);
    ui->comboBox_managers->setModel(managersFilter);
    statusesFilter = new StatusesFilterModel(this);
    ui->comboBox_status->setModel(statusesFilter);
}

AddEditOrdersDialog::~AddEditOrdersDialog()
{
    delete ui;
}

void AddEditOrdersDialog::setDialog(orderObject data, ClientsModel *clientsModel, ManagersModel *managersModel, ServicesModel *servicesModel, OrdersServicesModel *ordersServicesModel, StatusesModel *statusesModel)
{
    setDialog(clientsModel, managersModel, servicesModel, ordersServicesModel, statusesModel);

    isEdit = true;

    order = data;

    ui->comboBox_client->setCurrentIndex(clientsFilter->mapFromSource(clients->index(clients->getRowByData(ClientsModel::Column::ID, order[OrdersModel::Column::ID_CLIENT]), ClientsModel::Column::PIB)).row());
    ui->comboBox_managers->setCurrentIndex(managersFilter->mapFromSource(managers->index(managers->getRowByData(ManagersModel::Column::ID, order[OrdersModel::Column::ID_MANAGER]), ManagersModel::Column::PIB)).row());
    ui->comboBox_status->setCurrentIndex(statusesFilter->mapFromSource(statuses->index(statuses->getRowByData(StatusesModel::Column::ID, order[OrdersModel::Column::ID_STATUS]), StatusesModel::Column::TITLE)).row());
    ui->plainTextEdit_comment->setPlainText(order[OrdersModel::Column::COMMENT].toString());
    ui->dateTimeEdit_dateTimeOrder->setDateTime(order[OrdersModel::Column::DATE_TIME_ORDER].toDateTime());
    ui->dateEdit_dateCompletion->setDate(order[OrdersModel::Column::DATE_OF_COMPLETION].toDate());
    ui->doubleSpinBox_totalCost->setValue(order[OrdersModel::Column::TOTAL_COST].toDouble());

    ui->comboBox_client->setEnabled(false);
    ui->comboBox_managers->setEnabled(false);
    ui->dateEdit_dateCompletion->setReadOnly(false);
    ui->dateTimeEdit_dateTimeOrder->setReadOnly(false);
    ui->pushButton_services_add->setEnabled(false);
    ui->pushButton_services_del->setEnabled(false);

    orderServiceList = ordersServices->getDataById(order[OrdersModel::Column::NUMBER].toInt());
    ReWriteTableServices();
}

void AddEditOrdersDialog::setDialog(ClientsModel *clientsModel, ManagersModel *managersModel, ServicesModel *servicesModel, OrdersServicesModel *ordersServicesModel, StatusesModel *statusesModel)
{
    clients = clientsModel;
    clientsFilter->setSourceModel(clients);
    managers = managersModel;
    managersFilter->setSourceModel(managers);
    services = servicesModel;
    ordersServices = ordersServicesModel;
    statuses = statusesModel;
    statusesFilter->setSourceModel(statuses);
}

orderObject AddEditOrdersDialog::getOrderData()
{
    return order;
}

QList<orderServiceObject> AddEditOrdersDialog::getOrderServicesData()
{
    return orderServiceList;
}

bool AddEditOrdersDialog::isSave()
{
    return save;
}

void AddEditOrdersDialog::ReWriteTableServices()
{
    ui->tableWidget_services->setRowCount(0);
    ui->doubleSpinBox_totalCost->setValue(0);

    for(int i = 0; i < orderServiceList.size(); i++)
    {
        ui->tableWidget_services->insertRow(i);
        ui->tableWidget_services->setItem(i, 0, new QTableWidgetItem(services->getDataById(orderServiceList[i][OrdersServicesModel::Column::ID_SERVICE].toInt(), ServicesModel::Column::TITLE).toString()));
        ui->tableWidget_services->setItem(i, 1, new QTableWidgetItem(orderServiceList[i][OrdersServicesModel::Column::NUMBER].toString()));
        ui->tableWidget_services->setItem(i, 2, new QTableWidgetItem(QVariant(services->getDataById(orderServiceList[i][OrdersServicesModel::Column::ID_SERVICE].toInt(), ServicesModel::Column::COST).toFloat()*orderServiceList[i][OrdersServicesModel::Column::NUMBER].toFloat()).toString()));

        ui->doubleSpinBox_totalCost->setValue(ui->doubleSpinBox_totalCost->value()+services->getDataById(orderServiceList[i][OrdersServicesModel::Column::ID_SERVICE].toInt(), ServicesModel::Column::COST).toFloat()*orderServiceList[i][OrdersServicesModel::Column::NUMBER].toFloat());
    }
}

void AddEditOrdersDialog::on_pushButton_services_add_clicked()
{
    SelectServiceDialog selectDialog;

    selectDialog.setDialog(services);

    selectDialog.exec();

    if(!selectDialog.isSave())
        return;

    orderServiceList.append(selectDialog.getData());
    ReWriteTableServices();
}


void AddEditOrdersDialog::on_pushButton_services_del_clicked()
{
    if(ui->tableWidget_services->currentRow() == -1)
        return;

    orderServiceList.removeAt(ui->tableWidget_services->currentRow());
    ui->tableWidget_services->removeRow(ui->tableWidget_services->currentRow());

    ReWriteTableServices();
}


void AddEditOrdersDialog::on_pushButton_save_clicked()
{
    if(ui->comboBox_client->currentIndex() == -1)
        return;
    if(ui->comboBox_managers->currentIndex() == -1)
        return;
    if(ui->comboBox_status->currentIndex() == -1)
        return;
    if(ui->doubleSpinBox_totalCost->value() == 0)
        return;

    order[OrdersModel::Column::ID_CLIENT] = clients->index(clientsFilter->mapToSource(clientsFilter->index(ui->comboBox_client->currentIndex(), 0)).row(), ClientsModel::Column::ID).data(ClientsModel::Role::Read);
    order[OrdersModel::Column::ID_MANAGER] = managers->index(managersFilter->mapToSource(managersFilter->index(ui->comboBox_managers->currentIndex(), 0)).row(), ManagersModel::Column::ID).data(ManagersModel::Role::Read);
    order[OrdersModel::Column::ID_STATUS] = statuses->index(statusesFilter->mapToSource(statusesFilter->index(ui->comboBox_status->currentIndex(), 0)).row(), StatusesModel::Column::ID).data(StatusesModel::Role::Read);
    order[OrdersModel::Column::DATE_OF_COMPLETION] = ui->dateEdit_dateCompletion->date();
    order[OrdersModel::Column::DATE_TIME_ORDER] = ui->dateTimeEdit_dateTimeOrder->dateTime();
    order[OrdersModel::Column::TOTAL_COST] = ui->doubleSpinBox_totalCost->value();
    order[OrdersModel::Column::COMMENT] = ui->plainTextEdit_comment->toPlainText();

    save = true;

    close();
}


void AddEditOrdersDialog::on_pushButton_cancel_clicked()
{
    close();
}

