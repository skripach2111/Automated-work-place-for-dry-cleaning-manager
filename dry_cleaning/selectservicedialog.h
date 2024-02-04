#ifndef SELECTSERVICEDIALOG_H
#define SELECTSERVICEDIALOG_H

#include <QDialog>
#include "servicesfiltermodel.h"
#include "ordersservicesmodel.h"

namespace Ui {
class SelectServiceDialog;
}

class SelectServiceDialog : public QDialog
{
    Q_OBJECT

    orderServiceObject service;
    ServicesModel* model;
    ServicesFilterModel* filterModel;
    bool save = false;

public:
    explicit SelectServiceDialog(QWidget *parent = nullptr);
    ~SelectServiceDialog();

    void setDialog(ServicesModel* servicesModel);
    orderServiceObject getData();

    bool isSave();

private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::SelectServiceDialog *ui;
};

#endif // SELECTSERVICEDIALOG_H
