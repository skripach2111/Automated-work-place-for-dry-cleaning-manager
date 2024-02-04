#include "addeditservicesdialog.h"
#include "ui_addeditservicesdialog.h"

AddEditServicesDialog::AddEditServicesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEditServicesDialog)
{
    ui->setupUi(this);
    filterTypes = new TypeOfServicesFilterModel(this);
    ui->comboBox_type->setModel(filterTypes);
}

AddEditServicesDialog::~AddEditServicesDialog()
{
    delete ui;
}

void AddEditServicesDialog::setDialog(serviceObject data, FilesModel *modelFiles, TypeOfServicesModel *modelTypes)
{
    ui->label_title->setText("Редагування");
    isEdit = true;

    service = data;
    files = modelFiles;
    types = modelTypes;
    filterTypes->setSourceModel(types);

    ui->label_image->setText("");
    QPixmap pixmap;
    pixmap.loadFromData(modelFiles->getDataById(service[ServicesModel::Column::ID_IMAGE].toInt())[FilesModel::Column::FILE].toByteArray());
    ui->label_image->setPixmap(pixmap);
    ui->comboBox_type->setCurrentIndex(filterTypes->mapFromSource(types->index(types->getRowByData(TypeOfServicesModel::Column::ID, service[ServicesModel::Column::ID_TYPE]), TypeOfServicesModel::Column::TITLE)).row());
    ui->lineEdit_title->setText(service[ServicesModel::Column::TITLE].toString());
    ui->plainTextEdit_description->setPlainText(service[ServicesModel::Column::DESCRIPTION].toString());
    ui->doubleSpinBox_cost->setValue(service[ServicesModel::Column::COST].toDouble());
}

void AddEditServicesDialog::setDialog(FilesModel *modelFiles, TypeOfServicesModel *modelTypes)
{
    files = modelFiles;
    types = modelTypes;
    filterTypes->setSourceModel(types);

    ui->comboBox_type->setModel(filterTypes);
}

serviceObject AddEditServicesDialog::getData()
{
    return service;
}

bool AddEditServicesDialog::isSave()
{
    return save;
}

void AddEditServicesDialog::on_pushButton_upload_clicked()
{
    SelectFilesDialog selectDialog;
    if(isEdit)
        selectDialog.setDialog(files, files->getDataById(service[ServicesModel::Column::ID_IMAGE].toInt()));
    else
        selectDialog.setDialog(files);
    selectDialog.exec();

    if(selectDialog.isSave())
    {
        service[ServicesModel::Column::ID_IMAGE] = selectDialog.getData()[FilesModel::Column::ID];
        QPixmap pixmap;
        pixmap.loadFromData(selectDialog.getData()[FilesModel::Column::FILE].toByteArray());
        ui->label_image->setPixmap(pixmap);
    }
}


void AddEditServicesDialog::on_pushButton_save_clicked()
{
    QVariant number = files->saveChanges();
    files->select();

    service[ServicesModel::Column::TITLE] = ui->lineEdit_title->text();
    service[ServicesModel::Column::ID_TYPE] = types->index(filterTypes->mapToSource(filterTypes->index(ui->comboBox_type->currentIndex(), 0)).row(), TypeOfServicesModel::Column::ID).data(TypeOfServicesModel::Column::ID);
    service[ServicesModel::Column::ID_IMAGE] = number;
    service[ServicesModel::Column::COST] = ui->doubleSpinBox_cost->value();
    service[ServicesModel::Column::DESCRIPTION] = ui->plainTextEdit_description->toPlainText();

    save = true;

    close();
}


void AddEditServicesDialog::on_pushButton_cancel_clicked()
{
    files->select();
    close();
}

