#include "addeditstatusesdialog.h"
#include "ui_addeditstatusesdialog.h"

AddEditStatusesDialog::AddEditStatusesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEditStatusesDialog)
{
    ui->setupUi(this);
}

AddEditStatusesDialog::~AddEditStatusesDialog()
{
    delete ui;
}

void AddEditStatusesDialog::setDialog(statusObject data)
{
    ui->label_title->setText("Редагування");

    status = data;

    ui->lineEdit_title->setText(status[StatusesModel::Column::TITLE].toString());
}

statusObject AddEditStatusesDialog::getData()
{
    return status;
}

bool AddEditStatusesDialog::isSave()
{
    return save;
}

void AddEditStatusesDialog::on_pushButton_save_clicked()
{
    status[StatusesModel::Column::TITLE] = ui->lineEdit_title->text();

    save = true;

    close();
}


void AddEditStatusesDialog::on_pushButton_cancel_clicked()
{
    close();
}

