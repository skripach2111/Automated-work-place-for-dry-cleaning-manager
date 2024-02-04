#include "addeditmanagersdialog.h"
#include "ui_addeditmanagersdialog.h"

AddEditManagersDialog::AddEditManagersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEditManagersDialog)
{
    ui->setupUi(this);
}

AddEditManagersDialog::~AddEditManagersDialog()
{
    delete ui;
}

void AddEditManagersDialog::setDialog(managerObject data)
{
    ui->label_title->setText("Редагування");

    manager = data;

    ui->lineEdit_pib->setText(manager[ManagersModel::Column::PIB].toString());
    ui->lineEdit_phoneNumber->setText(manager[ManagersModel::Column::PHONE_NUMBER].toString());
}

managerObject AddEditManagersDialog::getData()
{
    return manager;
}

bool AddEditManagersDialog::isSave()
{
    return save;
}

void AddEditManagersDialog::on_pushButton_save_clicked()
{
    manager[ManagersModel::Column::PIB] = ui->lineEdit_pib->text();
    manager[ManagersModel::Column::PHONE_NUMBER] = ui->lineEdit_phoneNumber->text();

    save = true;

    close();
}
void AddEditManagersDialog::on_pushButton_cancel_clicked()
{
    close();
}

