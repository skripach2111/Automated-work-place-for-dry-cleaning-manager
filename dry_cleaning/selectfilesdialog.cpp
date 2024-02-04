#include "selectfilesdialog.h"
#include "ui_selectfilesdialog.h"

SelectFilesDialog::SelectFilesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectFilesDialog)
{
    ui->setupUi(this);
}

SelectFilesDialog::~SelectFilesDialog()
{
    delete ui;
}

void SelectFilesDialog::setDialog(FilesModel *filesModel)
{
    model = filesModel;
    ui->listView->setModel(model);
}

void SelectFilesDialog::setDialog(FilesModel* filesModel, fileObject data)
{
    file = data;
    model = filesModel;
    ui->listView->setModel(model);
    ui->listView->setModelColumn(FilesModel::Column::FILE);

    currentRow = model->getRowByData(FilesModel::Column::FILE, file[FilesModel::Column::FILE]);
    ui->listView->setCurrentIndex(model->index(currentRow, FilesModel::Column::FILE));
}

fileObject SelectFilesDialog::getData()
{
    return file;
}

bool SelectFilesDialog::isSave()
{
    return save;
}

void SelectFilesDialog::on_pushButton_upload_clicked()
{
    QFileDialog fileDialog;

    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilter("Images (*.png *.jpg *.bmp *.gif)");
    fileDialog.exec();

    if(!fileDialog.selectedFiles().empty())
    {
        QPixmap image = QPixmap(fileDialog.selectedFiles().at(0)).scaled(150, 150);
        QByteArray arr;
        QBuffer buffer(&arr);
        buffer.open(QIODevice::ReadWrite);
        image.save(&buffer, "PNG");
        file[FilesModel::Column::FILE] = arr;
        model->appendRow(file);
        currentRow = model->getRowByData(FilesModel::Column::FILE, file[FilesModel::Column::FILE]);
        ui->listView->setCurrentIndex(model->index(currentRow, FilesModel::Column::FILE));

        saveFile = true;
    }
}


void SelectFilesDialog::on_pushButton_save_clicked()
{
    if(saveFile)
    {
        model->saveChanges();
        model->select();
        currentRow = model->getRowByData(FilesModel::Column::FILE, file[FilesModel::Column::FILE]);
        ui->listView->setCurrentIndex(model->index(currentRow, FilesModel::Column::FILE));
    }
    currentRow = ui->listView->currentIndex().row();

    file[FilesModel::Column::ID] = model->index(currentRow, FilesModel::Column::ID).data(FilesModel::Role::Read);
    file[FilesModel::Column::FILE] = model->index(currentRow, FilesModel::Column::FILE).data(FilesModel::Role::Read);
    save = true;
    close();
}


void SelectFilesDialog::on_pushButton_cancel_clicked()
{
    model->select();
    close();
}

