#-------------------------------------------------
#
# Project created by QtCreator 2018-04-13T10:53:05
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
TARGET = DispatchClient
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    global.cpp \
    logindialog.cpp \
    common.cpp \
    msgcenter.cpp \
    configure.cpp \
    serverconnection.cpp \
    UI/mainwindow.cpp \
    UI/User/dialogadduser.cpp \
    UI/User/dialogmodifyuser.cpp \
    UI/User/dockusermanage.cpp \
    UI/widgetwaiting.cpp \
    UI/Agv/dockagvmanage.cpp \
    UI/Agv/dialogaddagv.cpp \
    UI/Agv/dialogmodifyagv.cpp \
    UI/MapEdit/mapeditwindow.cpp \
    UI/MapEdit/maptreeitem.cpp \
    UI/MapEdit/maptreemodel.cpp \
    UI/MapEdit/dockmaptree.cpp \
    data/mapspirit.cpp \
    data/mappoint.cpp \
    data/mapfloor.cpp \
    data/mappath.cpp \
    data/onemap.cpp \
    UI/MapEdit/dockproperty.cpp

HEADERS += \
    global.h \
    logindialog.h \
    common.h \
    msgcenter.h \
    commonhead.h \
    protocol.h \
    configure.h \
    serverconnection.h \
    UI/mainwindow.h \
    UI/User/dialogadduser.h \
    UI/User/dialogmodifyuser.h \
    UI/User/dockusermanage.h \
    UI/widgetwaiting.h \
    UI/Agv/dockagvmanage.h \
    UI/Agv/dialogaddagv.h \
    UI/Agv/dialogmodifyagv.h \
    UI/MapEdit/mapeditwindow.h \
    UI/MapEdit/maptreeitem.h \
    UI/MapEdit/maptreemodel.h \
    UI/MapEdit/dockmaptree.h \
    data/mapspirit.h \
    data/mappoint.h \
    data/mapfloor.h \
    data/mappath.h \
    data/onemap.h \
    UI/MapEdit/dockproperty.h

FORMS +=

DISTFILES += \
    config.conf

RESOURCES += \
    images.qrc
