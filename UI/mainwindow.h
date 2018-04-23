#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "commonhead.h"

#include "User/widgetusermanage.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void init();
signals:

public slots:
    void showUserManager(bool checked);
    void about();
    void aboutHrg();

    //状态栏改变
    void statusbar_server_msg(QString msg);
    void statusbar_uiChanged(QString uiname);
    void statusbar_msg(QString msg);

    void onServerConnect();
    void onServerDisconnect();
    void onServerConnectting();

    void onSendFail();
    void onWaitResponseTimeOut();
    void onTip(QString s);
    void onErr(int errcode,QString info);
private:
    void createActions();
    void createStatusBar();

    WidgetUserManage *usermanage;

    QLabel *serverlabel;
    QLabel *uilabel;
    QLabel *msglabel;
};

#endif // MAINWINDOW_H
