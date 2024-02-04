#ifndef ADDEDITTYPESOFSERVICESDIALOG_H
#define ADDEDITTYPESOFSERVICESDIALOG_H

#include <QDialog>
#include "typeofservicesmodel.h"

namespace Ui {
class AddEditTypesOfServicesDialog;
}

class AddEditTypesOfServicesDialog : public QDialog
{
    Q_OBJECT

    typeOfServicesObject typeOfService;
    bool save = false;

public:
    explicit AddEditTypesOfServicesDialog(QWidget *parent = nullptr);
    ~AddEditTypesOfServicesDialog();

    void setDialog(typeOfServicesObject data);
    typeOfServicesObject getData();

    bool isSave();

private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::AddEditTypesOfServicesDialog *ui;
};

#endif // ADDEDITTYPESOFSERVICESDIALOG_H
