#ifndef MAPEDITWINDOW_H
#define MAPEDITWINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include "dockview.h"
#include "dockmaptree.h"
#include "dockproperty.h"

class MapEditWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MapEditWindow(OneMap _oneMap,QWidget *parent = nullptr);
    void init();
signals:

public slots:

    void about();
    void aboutHrg();

    void onServerConnect();
    void onServerDisconnect();
    void onServerConnectting();

    void onSendFail();
    void onWaitResponseTimeOut();
    void onTip(QString s);
    void onErr(int errcode,QString info);
    void onNewRequest();

    void slot_currentMousePos(QPointF pos);
private:

    void createActions();
    void createStatusBar();

    //状态栏改变
    void statusbar_info(QString msg);
    void statusbar_err(QString msg);
    void statusbar_pos(QString msg);

    QLabel *info_label;
    QLabel *error_label;

    QLabel *pos_label;

    OneMap oneMap;//地图编辑器的一个map

    DockMapTree *dockMapTree;
    DockProperty *dockProperty;
    DockView *dockView;

    QAction *toolErase;
    QAction *toolStationDraw;
    QAction *toolStationReport;
    QAction *toolStationHalt;
    QAction *toolStationCharge;
    QAction *toolStationLoad;
    QAction *toolStationUnload;
    QAction *toolStationLoadUnload;
    QAction *toolLine;
    QAction *toolQb;
    QAction *toolCb;
};

#endif // MAPEDITWINDOW_H
