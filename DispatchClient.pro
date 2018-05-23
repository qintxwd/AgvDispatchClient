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
    data/mapspirit.cpp \
    data/mappoint.cpp \
    data/mapfloor.cpp \
    data/mappath.cpp \
    data/onemap.cpp \
    data/mapblock.cpp \
    data/mapgroup.cpp \
    data/mapbackground.cpp \
    UI/MapEdit/dockmaptree.cpp \
    UI/MapEdit/dockproperty.cpp \
    UI/MapEdit/dockview.cpp \
    UI/MapEdit/scene.cpp \
    UI/MapEdit/mapitemstation.cpp \
    UI/MapEdit/mapitemstationname.cpp \
    UI/MapEdit/mapitemquadraticbezier.cpp \
    UI/MapEdit/mapitemcubicbezier.cpp \
    UI/MapEdit/mapitembkg.cpp \
    UI/MapEdit/mapeditwindow.cpp \
    UI/MapEdit/maptreeitem.cpp \
    UI/MapEdit/maptreemodel.cpp \
    UI/MapEdit/viewer.cpp \
    UI/MapEdit/dialogrootpath.cpp \
    UI/MapEdit/dockblock.cpp \
    UI/MapEdit/dialogblockedit.cpp \
    UI/MapEdit/mapitemline.cpp \
    base64.cpp \
    UI/UserLog/dockuserlog.cpp \
    UI/Monitor/monitormaptreeitem.cpp \
    UI/Monitor/monitordialogblockedit.cpp \
    UI/Monitor/monitordockblock.cpp \
    UI/Monitor/monitordockmaptree.cpp \
    UI/Monitor/monitordockproperty.cpp \
    UI/Monitor/monitordockview.cpp \
    UI/Monitor/monitormapitembkg.cpp \
    UI/Monitor/monitormapitemcubicbezier.cpp \
    UI/Monitor/monitormapitemline.cpp \
    UI/Monitor/monitormapitemquadraticbezier.cpp \
    UI/Monitor/monitormapitemstation.cpp \
    UI/Monitor/monitormaptreemodel.cpp \
    UI/Monitor/monitorscene.cpp \
    UI/Monitor/monitorviewer.cpp \
    UI/Monitor/monitormapitemstationname.cpp

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
    data/mapspirit.h \
    data/mappoint.h \
    data/mapfloor.h \
    data/mappath.h \
    data/onemap.h \
    data/mapblock.h \
    data/mapgroup.h \
    data/mapbackground.h \
    UI/MapEdit/dockproperty.h \
    UI/MapEdit/dockview.h \
    UI/MapEdit/scene.h \
    UI/MapEdit/mapitemstation.h \
    UI/MapEdit/mapitemstationname.h \
    UI/MapEdit/mapitemquadraticbezier.h \
    UI/MapEdit/mapitemcubicbezier.h \
    UI/MapEdit/mapitembkg.h \
    UI/MapEdit/viewer.h \
    UI/MapEdit/dialogrootpath.h \
    UI/MapEdit/dockblock.h \
    UI/MapEdit/dialogblockedit.h \
    UI/MapEdit/mapitemline.h \
    UI/MapEdit/mapeditwindow.h \
    UI/MapEdit/maptreeitem.h \
    UI/MapEdit/maptreemodel.h \
    UI/MapEdit/dockmaptree.h \
    base64.h \
    UI/UserLog/dockuserlog.h \
    UI/Monitor/monitormaptreeitem.h \
    UI/Monitor/monitordialogblockedit.h \
    UI/Monitor/monitordockblock.h \
    UI/Monitor/monitordockmaptree.h \
    UI/Monitor/monitordockproperty.h \
    UI/Monitor/monitordockview.h \
    UI/Monitor/monitormapitembkg.h \
    UI/Monitor/monitormapitemcubicbezier.h \
    UI/Monitor/monitormapitemline.h \
    UI/Monitor/monitormapitemquadraticbezier.h \
    UI/Monitor/monitormapitemstation.h \
    UI/Monitor/monitormaptreemodel.h \
    UI/Monitor/monitorscene.h \
    UI/Monitor/monitorviewer.h \
    UI/Monitor/monitormapitemstationname.h

FORMS +=

DISTFILES += \
    config.conf

RESOURCES += \
    images.qrc




win32:CONFIG(release, debug|release): LIBS += -LD:/thirdparty/jsoncpp/lib/release/ -ljsoncpp_static
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/thirdparty/jsoncpp/lib/debug/ -ljsoncpp_static
else:unix:!macx: LIBS += -LD:/thirdparty/jsoncpp/lib/ -ljsoncpp_static

INCLUDEPATH += D:/thirdparty/jsoncpp/include
DEPENDPATH += D:/thirdparty/jsoncpp/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += D:/thirdparty/jsoncpp/lib/release/libjsoncpp_static.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += D:/thirdparty/jsoncpp/lib/debug/libjsoncpp_static.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += D:/thirdparty/jsoncpp/lib/release/jsoncpp_static.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += D:/thirdparty/jsoncpp/lib/debug/jsoncpp_static.lib
else:unix:!macx: PRE_TARGETDEPS += D:/thirdparty/jsoncpp/lib/libjsoncpp_static.a
