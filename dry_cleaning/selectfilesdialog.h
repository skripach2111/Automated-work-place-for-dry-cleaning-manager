#ifndef SELECTFILESDIALOG_H
#define SELECTFILESDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QBuffer>

#include "filesmodel.h"

#include <QStyledItemDelegate>
#include <QPainter>
#include <QPixmap>
#include <QImage>

namespace Ui {
class SelectFilesDialog;
}

class SelectFilesDialog : public QDialog
{
    Q_OBJECT

    int currentRow = -1;
    fileObject file;
    FilesModel* model;
    bool save = false;
    bool saveFile = false;

public:
    explicit SelectFilesDialog(QWidget *parent = nullptr);
    ~SelectFilesDialog();

    void setDialog(FilesModel* filesModel);
    void setDialog(FilesModel* filesModel, fileObject data);
    fileObject getData();

    bool isSave();

private slots:
    void on_pushButton_upload_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::SelectFilesDialog *ui;
};

#endif // SELECTFILESDIALOG_H
