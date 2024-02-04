#include "addedittypesofservicesdialog.h"
#include "ui_addedittypesofservicesdialog.h"

AddEditTypesOfServicesDialog::AddEditTypesOfServicesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEditTypesOfServicesDialog)
{
    ui->setupUi(this);
}

AddEditTypesOfServicesDialog::~AddEditTypesOfServicesDialog()
{
    delete ui;
}

void AddEditTypesOfServicesDialog::setDialog(typeOfServicesObject data)
{
    ui->label_title->setText("Редагування");

    typeOfService = data;

    ui->lineEdit_title->setText(typeOfService[TypeOfServicesModel::Column::TITLE].toString());
    ui->plainTextEdit_description->setPlainText(typeOfService[TypeOfServicesModel::Column::DESCRIPTION].toString());
}

typeOfServicesObject AddEditTypesOfServicesDialog::getData()
{
    return typeOfService;
}

bool AddEditTypesOfServicesDialog::isSave()
{
    return save;
}

void AddEditTypesOfServicesDialog::on_pushButton_save_clicked()
{
    typeOfService[TypeOfServicesModel::Column::TITLE] = ui->lineEdit_title->text();
    typeOfService[TypeOfServicesModel::Column::DESCRIPTION] = ui->plainTextEdit_description->toPlainText();

    save = true;

    close();
}


void AddEditTypesOfServicesDialog::on_pushButton_cancel_clicked()
{
    close();
}

