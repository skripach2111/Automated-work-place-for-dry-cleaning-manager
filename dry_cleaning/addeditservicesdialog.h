#ifndef ADDEDITSERVICESDIALOG_H
#define ADDEDITSERVICESDIALOG_H

#include <QDialog>

#include "servicesmodel.h"
#include "selectfilesdialog.h"
#include "typeofservicesfiltermodel.h"

namespace Ui {
class AddEditServicesDialog;
}

class AddEditServicesDialog : public QDialog
{
    Q_OBJECT

    serviceObject service;
    FilesModel* files;
    TypeOfServicesModel* types;
    TypeOfServicesFilterModel* filterTypes;
    bool save = false;
    bool save_file = false;
    bool isEdit = false;

public:
    explicit AddEditServicesDialog(QWidget *parent = nullptr);
    ~AddEditServicesDialog();

    void setDialog(serviceObject data, FilesModel* modelFiles, TypeOfServicesModel* modelTypes);
    void setDialog(FilesModel* modelFiles, TypeOfServicesModel* modelTypes);
    serviceObject getData();

    bool isSave();

private slots:
    void on_pushButton_upload_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::AddEditServicesDialog *ui;
};

#endif // ADDEDITSERVICESDIALOG_H
