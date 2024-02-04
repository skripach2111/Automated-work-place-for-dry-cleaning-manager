#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile styleF;
    styleF.setFileName(":/styles/style.qss");
    styleF.open(QFile::ReadOnly);
    QString qssStr = styleF.readAll();
    a.setStyleSheet(qssStr);

    MainWindow w;
    w.show();
    return a.exec();
}
