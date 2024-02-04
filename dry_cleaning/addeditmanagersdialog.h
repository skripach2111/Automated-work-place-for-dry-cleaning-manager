#ifndef ADDEDITMANAGERSDIALOG_H
#define ADDEDITMANAGERSDIALOG_H

#include <QDialog>
#include "managersmodel.h"

namespace Ui {
class AddEditManagersDialog;
}

class AddEditManagersDialog : public QDialog
{
    Q_OBJECT

    managerObject manager;
    bool save = false;


public:
    explicit AddEditManagersDialog(QWidget *parent = nullptr);
    ~AddEditManagersDialog();

    void setDialog(managerObject data);
    managerObject getData();

    bool isSave();

private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::AddEditManagersDialog *ui;
};

#endif // ADDEDITMANAGERSDIALOG_H
