#include "selectservicedialog.h"
#include "ui_selectservicedialog.h"

SelectServiceDialog::SelectServiceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectServiceDialog)
{
    ui->setupUi(this);
    filterModel = new ServicesFilterModel(this);
    ui->label_error_services->setVisible(false);
    ui->label_error_number->setVisible(false);
}

SelectServiceDialog::~SelectServiceDialog()
{
    delete ui;
}

void SelectServiceDialog::setDialog(ServicesModel *servicesModel)
{
    model = servicesModel;
    filterModel->setSourceModel(model);
    filterModel->setEnabledFilterParam(ServicesFilterModel::FilterParam::ONLY_COST, true);
    ui->tableView->setModel(filterModel);
}

orderServiceObject SelectServiceDialog::getData()
{
    return service;
}

bool SelectServiceDialog::isSave()
{
    return save;
}

void SelectServiceDialog::on_pushButton_save_clicked()
{
    ui->label_error_services->setVisible(false);
    ui->label_error_number->setVisible(false);

    if(ui->tableView->currentIndex().row() == -1)
        ui->label_error_services->setVisible(true);
    if(ui->spinBox->value() <= 0)
        ui->label_error_number->setVisible(true);

    if(ui->label_error_number->isVisible() || ui->label_error_services->isVisible())
        return;

    service[OrdersServicesModel::Column::ID_SERVICE] = model->index(filterModel->mapToSource(filterModel->index(ui->tableView->currentIndex().row(), 0)).row(), 0).data(ServicesModel::Column::ID);
    service[OrdersServicesModel::Column::NUMBER] = ui->spinBox->value();

    save = true;

    close();
}


void SelectServiceDialog::on_pushButton_cancel_clicked()
{
    close();
}

