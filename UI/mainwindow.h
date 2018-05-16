#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "commonhead.h"

#include "User/dockusermanage.h"
#include "Agv/dockagvmanage.h"
#include "UserLog/dockuserlog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
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
private:

    void createActions();
    void createStatusBar();
    //状态栏改变
    void statusbar_info(QString msg);
    void statusbar_err(QString msg);

    DockUserManage *dock_user_manage;
    DockAgvManage *dock_agv_manage;
    DockUserLog *dock_user_log;

    QLabel *info_label;
    QLabel *error_label;
};

#endif // MAINWINDOW_H
