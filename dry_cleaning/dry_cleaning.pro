QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addeditclientsdialog.cpp \
    addeditmanagersdialog.cpp \
    addeditordersdialog.cpp \
    addeditservicesdialog.cpp \
    addeditstatusesdialog.cpp \
    addedittypesofservicesdialog.cpp \
    clientsfiltermodel.cpp \
    clientsmodel.cpp \
    databasemodule.cpp \
    filesmodel.cpp \
    main.cpp \
    mainwindow.cpp \
    managersfiltermodel.cpp \
    managersmodel.cpp \
    ordersfiltermodel.cpp \
    ordersmodel.cpp \
    ordersservicesmodel.cpp \
    selectfilesdialog.cpp \
    selectservicedialog.cpp \
    servicesfiltermodel.cpp \
    servicesmodel.cpp \
    statusesfiltermodel.cpp \
    statusesmodel.cpp \
    typeofservicesfiltermodel.cpp \
    typeofservicesmodel.cpp

HEADERS += \
    addeditclientsdialog.h \
    addeditmanagersdialog.h \
    addeditordersdialog.h \
    addeditservicesdialog.h \
    addeditstatusesdialog.h \
    addedittypesofservicesdialog.h \
    clientsfiltermodel.h \
    clientsmodel.h \
    databasemodule.h \
    filesmodel.h \
    mainwindow.h \
    managersfiltermodel.h \
    managersmodel.h \
    ordersfiltermodel.h \
    ordersmodel.h \
    ordersservicesmodel.h \
    selectfilesdialog.h \
    selectservicedialog.h \
    servicesfiltermodel.h \
    servicesmodel.h \
    statusesfiltermodel.h \
    statusesmodel.h \
    typeofservicesfiltermodel.h \
    typeofservicesmodel.h

FORMS += \
    addeditclientsdialog.ui \
    addeditmanagersdialog.ui \
    addeditordersdialog.ui \
    addeditservicesdialog.ui \
    addeditstatusesdialog.ui \
    addedittypesofservicesdialog.ui \
    mainwindow.ui \
    selectfilesdialog.ui \
    selectservicedialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    main.qrc
