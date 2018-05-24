#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "commonhead.h"

#include "User/dockusermanage.h"
#include "Agv/dockagvmanage.h"
#include "UserLog/dockuserlog.h"
#include "Task/docktask.h"

class MapEditWindow;
class MonitorDockView;
class MonitorDockMapTree;
class MonitorDockProperty;
class MonitorDockBlock;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void init();
signals:
    void sig_setSelectSelect();
    void sig_setSelectHand();
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
    void updateCurrentUserInfo();

    void onMapEdit();

    void onMapLoad();

    void on_selectSelect_triggered(bool b);
    void on_selectHand_triggered(bool b);
private:

    void createActions();
    void createStatusBar();
    //状态栏改变
    void statusbar_info(QString msg);
    void statusbar_err(QString msg);

    DockUserManage *dock_user_manage;
    DockAgvManage *dock_agv_manage;
    DockUserLog *dock_user_log;
    DockTask *dock_task;

    QLabel *usernamelabel;
    QLabel *userrolelabel;
    QLabel *info_label;
    QLabel *error_label;
    QStatusBar *statusbar;

    QAction *action_map_edit;

    MapEditWindow *editWindow;


    //TODO 新增加
    OneMap *oneMap;
    MonitorDockMapTree *dockMapTree;
    MonitorDockProperty *dockProperty;
    MonitorDockView *dockView;
    MonitorDockBlock *blockView;
    QAction *selectSelect;
    QAction *selectHand;
};

#endif // MAINWINDOW_H
