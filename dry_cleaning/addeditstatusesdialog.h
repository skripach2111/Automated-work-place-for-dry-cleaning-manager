#ifndef ADDEDITSTATUSESDIALOG_H
#define ADDEDITSTATUSESDIALOG_H

#include <QDialog>
#include "statusesmodel.h"
#include "typeofservicesfiltermodel.h"

namespace Ui {
class AddEditStatusesDialog;
}

class AddEditStatusesDialog : public QDialog
{
    Q_OBJECT

    statusObject status;
    bool save = false;
    TypeOfServicesFilterModel* filterTypes;

public:
    explicit AddEditStatusesDialog(QWidget *parent = nullptr);
    ~AddEditStatusesDialog();

    void setDialog(statusObject data);
    statusObject getData();

    bool isSave();

private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::AddEditStatusesDialog *ui;
};

#endif // ADDEDITSTATUSESDIALOG_H
