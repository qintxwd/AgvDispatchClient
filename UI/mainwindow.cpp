#include "mainwindow.h"

#include "global.h"

#include "MapEdit/mapeditwindow.h"
#include "Monitor/monitordockblock.h"
#include "Monitor/monitordockmaptree.h"
#include "Monitor/monitordockproperty.h"
#include "Monitor/monitordockview.h"
#include <QAction>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    dock_user_manage(nullptr),
    dock_agv_manage(nullptr),
    dock_user_log(nullptr),
    editWindow(nullptr),
    dock_task(nullptr),
    oneMap(g_onemap.clone())
{
    setWindowTitle("agv dispatch client");
    createActions();
    createStatusBar();

    init();
}

void MainWindow::init()
{
    connect(&msgCenter,SIGNAL(sig_connection_connected()),this,SLOT(onServerConnect()));
    connect(&msgCenter,SIGNAL(sig_connection_disconnected()),this,SLOT(onServerDisconnect()));
    connect(&msgCenter,SIGNAL(sig_connection_conntectting()),this,SLOT(onServerConnectting()));

    connect(&msgCenter,SIGNAL(sendRequestFail()),this,SLOT(onSendFail()));
    connect(&msgCenter,SIGNAL(waitResponseTimeOut()),this,SLOT(onWaitResponseTimeOut()));
    connect(&msgCenter,SIGNAL(tip(QString)),this,SLOT(onTip(QString)));
    connect(&msgCenter,SIGNAL(err(int,QString)),this,SLOT(onErr(int,QString)));
    connect(&msgCenter,SIGNAL(sendNewRequest()),this,SLOT(onNewRequest()));
    connect(&msgCenter,SIGNAL(loginSuccess(int)),this,SLOT(updateCurrentUserInfo()));
    connect(&msgCenter,SIGNAL(mapGetSuccess()),this,SLOT(onMapLoad()));

    //connect(dockMapTree,SIGNAL(sig_addFloor(MapFloor*)),dockView,SLOT(slot_addFloor(MapFloor*)));
    connect(dockMapTree,SIGNAL(sig_chooseSpirit(MapSpirit*)),dockProperty,SLOT(slot_showSpirit(MapSpirit*)));
    connect(dockMapTree,SIGNAL(sig_chooseSpirit(MapSpirit*)),dockView,SLOT(slot_selectChanged(MapSpirit *)));

    //connect(dockView,SIGNAL(sig_currentMousePos(QPointF)),this,SLOT(slot_currentMousePos(QPointF)));
    //connect(dockView,SIGNAL(sig_cancelTool()),this,SLOT(slot_cancelTool()));
    //connect(this,SIGNAL(sig_setTool(int)),dockView,SIGNAL(sig_setTool(int)));

    //connect(dockView,SIGNAL(sig_add_remove_spirit()),dockMapTree,SLOT(refresh()));
    //connect(dockView,SIGNAL(sig_add_remove_spirit()),dockProperty,SLOT(slot_shownull()));
    connect(dockView,SIGNAL(sig_chooseChanged(MapSpirit*)),dockMapTree,SLOT(slot_chooseChanged(MapSpirit*)));
    connect(dockView,SIGNAL(sig_chooseChanged(MapSpirit*)),dockProperty,SLOT(slot_showSpirit(MapSpirit*)));
    //connect(dockView,SIGNAL(sig_propertyChanged(MapSpirit*)),dockProperty,SLOT(slot_propertyChanged(MapSpirit*)));

    //属性窗口 修改 到 地图窗口的信号
    //    connect(dockProperty,SIGNAL(sig_propertyChanged(MapSpirit *)),dockView,SIGNAL(sig_propertyChangedFromProperty(MapSpirit*)));
    //    connect(dockProperty,SIGNAL(sig_propertyChanged(MapSpirit *)),dockMapTree,SLOT(refresh()));

    connect(blockView,SIGNAL(sig_chooseSpirit(MapSpirit*)),dockProperty,SLOT(slot_showSpirit(MapSpirit *)));

    connect(this,SIGNAL(sig_setSelectHand()),dockView,SIGNAL(sig_selectHand()));
    connect(this,SIGNAL(sig_setSelectSelect()),dockView,SIGNAL(sig_selectSelect()));
    //connect(this,SIGNAL(sig_addBkg(int)),dockView,SLOT(slot_addBkg(int)));
}

void MainWindow::onServerConnect()
{
    statusbar_info(QStringLiteral("已连接"));
}

void MainWindow::onServerDisconnect()
{
    statusbar_info(QStringLiteral("连接已断开"));
}

void MainWindow::onServerConnectting()
{
    statusbar_info(QStringLiteral("尝试连接服务器..."));
}

void MainWindow::onSendFail()
{
    statusbar_err(QStringLiteral("发送请求失败"));
}

void MainWindow::onWaitResponseTimeOut()
{
    statusbar_err(QStringLiteral("等待响应超时"));
}
void MainWindow::onTip(QString s)
{
    statusbar_info(s);
}
void MainWindow::onErr(int errcode,QString info)
{
    statusbar_err(getErrorString(errcode)+"  "+info);
}

void MainWindow::onNewRequest()
{
    statusbar_err("");
    statusbar_info("");
}

void MainWindow::updateCurrentUserInfo()
{
    usernamelabel->setText(QStringLiteral("当前用户:")+current_user_info.username);
    QString role_name = "";
    switch (current_user_info.role) {
    case USER_ROLE_VISITOR:
        role_name =QStringLiteral( "未登录");
        break;
    case USER_ROLE_OPERATOR:
        role_name =QStringLiteral( "普通用户");
        break;
    case USER_ROLE_ADMIN:
        role_name =QStringLiteral( "管理员");
        break;
    case USER_ROLE_SUPER_ADMIN:
        role_name =QStringLiteral( "超级管理员");
        break;
    case USER_ROLE_DEVELOP:
        role_name =QStringLiteral( "开发人员");
        break;
    default:
        break;
    }
    userrolelabel->setText(QStringLiteral("当前角色:")+role_name);
}


void MainWindow::createStatusBar()
{
    statusbar = new QStatusBar(this);
    usernamelabel = new QLabel(QStringLiteral("当前用户:")+current_user_info.username);
    QString role_name = "";
    switch (current_user_info.role) {
    case USER_ROLE_VISITOR:
        role_name =QStringLiteral( "未登录");
        break;
    case USER_ROLE_OPERATOR:
        role_name =QStringLiteral( "普通用户");
        break;
    case USER_ROLE_ADMIN:
        role_name =QStringLiteral( "管理员");
        break;
    case USER_ROLE_SUPER_ADMIN:
        role_name =QStringLiteral( "超级管理员");
        break;
    case USER_ROLE_DEVELOP:
        role_name =QStringLiteral( "开发人员");
        break;
    default:
        break;
    }
    userrolelabel = new QLabel(QStringLiteral("当前角色:")+role_name);
    info_label = new QLabel(QStringLiteral("信息:"));
    error_label = new QLabel(QStringLiteral("错误:"));
    usernamelabel->setMinimumWidth(200);
    userrolelabel->setMinimumWidth(200);
    info_label->setMinimumWidth(200);

    statusbar->addWidget(usernamelabel);
    statusbar->addWidget(userrolelabel);
    statusbar->addWidget(info_label);
    statusbar->addWidget(error_label);

    setStatusBar(statusbar);
}

void MainWindow::createActions()
{
    QMenu *viewsMenu = menuBar()->addMenu(tr("Views"));
    QToolBar *viewsToolBar = addToolBar(tr("Views"));

    dock_user_manage = new DockUserManage();
    addDockWidget(Qt::RightDockWidgetArea, dock_user_manage);
    viewsMenu->addAction(dock_user_manage->toggleViewAction());
    viewsToolBar->addAction(dock_user_manage->toggleViewAction());
    dock_agv_manage = new DockAgvManage();
    addDockWidget(Qt::RightDockWidgetArea, dock_agv_manage);
    viewsMenu->addAction(dock_agv_manage->toggleViewAction());
    viewsToolBar->addAction(dock_agv_manage->toggleViewAction());
    dock_user_log = new DockUserLog();
    addDockWidget(Qt::RightDockWidgetArea,dock_user_log);
    viewsMenu->addAction(dock_user_log->toggleViewAction());
    viewsToolBar->addAction(dock_user_log->toggleViewAction());
    dock_task = new DockTask();
    addDockWidget(Qt::LeftDockWidgetArea,dock_task);
    viewsMenu->addAction(dock_task->toggleViewAction());
    viewsToolBar->addAction(dock_task->toggleViewAction());

    action_map_edit = new QAction("Map Edit", this);
    action_map_edit->setCheckable(false);
    viewsMenu->addAction(action_map_edit);
    viewsToolBar->addAction(action_map_edit);

    connect(action_map_edit,SIGNAL(triggered(bool)),this,SLOT(onMapEdit()));

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"));
    aboutAct->setStatusTip(tr("Show the application's About box"));

    QAction *aboutQtAct = helpMenu->addAction(tr("About &HRG"));
    aboutQtAct->setStatusTip(tr("Show the HRG's About box"));

    //////////////////////////////////新增加

    dockMapTree = new MonitorDockMapTree(oneMap);
    dockProperty = new MonitorDockProperty(oneMap);
    dockView = new MonitorDockView(oneMap);
    blockView = new MonitorDockBlock(oneMap);

    addDockWidget(Qt::LeftDockWidgetArea,dockMapTree);
    addDockWidget(Qt::LeftDockWidgetArea,dockProperty);
    //    addDockWidget(Qt::RightDockWidgetArea,dockView);
    tabifyDockWidget(dockMapTree,blockView);

    addDockWidget(Qt::LeftDockWidgetArea,dockMapTree);
    addDockWidget(Qt::LeftDockWidgetArea,dockProperty);
    addDockWidget(Qt::RightDockWidgetArea,dockView);
    tabifyDockWidget(dockMapTree,blockView);

    setCentralWidget(dockView);

    QToolBar *selectToolBar = addToolBar(tr("Selects"));

    selectSelect = new QAction(this);
    selectSelect->setText("select");
    selectSelect->setObjectName("selectSelect");
    selectSelect->setCheckable(true);
    selectToolBar->addAction(selectSelect);

    selectHand = new QAction(this);
    selectHand->setText("drag");
    selectHand->setObjectName("selectHand");
    selectHand->setCheckable(true);
    selectToolBar->addAction(selectHand);

    viewsMenu->addAction(dockMapTree->toggleViewAction());
    viewsToolBar->addAction(dockMapTree->toggleViewAction());

    viewsMenu->addAction(dockProperty->toggleViewAction());
    viewsToolBar->addAction(dockProperty->toggleViewAction());

    viewsMenu->addAction(blockView->toggleViewAction());
    viewsToolBar->addAction(blockView->toggleViewAction());
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Application"),
                       tr("The <b>HRG Dispatch client</b> connect to dispatch server,<br>"
                          "and show the ui for user to watch the status of agvs and<br>"
                          "other information about the agv dispatch system."));
}

void MainWindow::aboutHrg()
{
    QMessageBox::about(this, tr("About Hit Robot Group"),
                       tr("<b>Hit Robot Group</b><br>"
                          "we have the top robotics research scientists in<br>"
                          "the country and we are in the robot technology <br>"
                          "subject frountirer struggle,unity,hard work of<br>"
                          "high level research team.<br>"
                          "To get more information about us,you can visit our<br>"
                          " website: <a href=\'http://www.hrgrobotics.com/\'>HRG</a>"));
}

void MainWindow::statusbar_info(QString msg)
{
    info_label->setText(QStringLiteral("信息:")+msg);
}

void MainWindow::statusbar_err(QString msg)
{
    error_label->setText(QStringLiteral("错误:")+msg);
}

void MainWindow::onMapEdit()
{
    if(editWindow){
        editWindow->close();
        delete editWindow;
    }
    editWindow = new MapEditWindow(g_onemap.clone());
    editWindow->show();
}

void MainWindow::onMapLoad()
{
    oneMap = g_onemap.clone();
    //TODO:重新载入
}


void MainWindow::on_selectSelect_triggered(bool b)
{
    selectHand->setChecked(false);
    emit sig_setSelectSelect();
}

void MainWindow::on_selectHand_triggered(bool b)
{
    if(b){
        selectSelect->setChecked(false);
        emit sig_setSelectHand();
    }
    else
        emit sig_setSelectSelect();
}
