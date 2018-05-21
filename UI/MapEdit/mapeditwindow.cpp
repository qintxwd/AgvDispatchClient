#include "mapeditwindow.h"
#include "global.h"
#include "scene.h"
#include <QFileIconProvider>


MapEditWindow::MapEditWindow(OneMap *_oneMap, QWidget *parent) : QMainWindow(parent),
    oneMap(_oneMap)
{
    setWindowTitle("Map edit");
    createActions();
    createStatusBar();

    init();
}

void MapEditWindow::init()
{
    connect(&msgCenter,SIGNAL(sig_connection_connected()),this,SLOT(onServerConnect()));
    connect(&msgCenter,SIGNAL(sig_connection_disconnected()),this,SLOT(onServerDisconnect()));
    connect(&msgCenter,SIGNAL(sig_connection_conntectting()),this,SLOT(onServerConnectting()));

    connect(&msgCenter,SIGNAL(sendRequestFail()),this,SLOT(onSendFail()));
    connect(&msgCenter,SIGNAL(waitResponseTimeOut()),this,SLOT(onWaitResponseTimeOut()));
    connect(&msgCenter,SIGNAL(tip(QString)),this,SLOT(onTip(QString)));
    connect(&msgCenter,SIGNAL(err(int,QString)),this,SLOT(onErr(int,QString)));
    connect(&msgCenter,SIGNAL(sendNewRequest()),this,SLOT(onNewRequest()));

    //对树 的操作( 只有两个操作 添加楼层、选择不同的节点)
    connect(dockMapTree,SIGNAL(sig_addFloor(MapFloor*)),dockView,SLOT(slot_addFloor(MapFloor*)));
    connect(dockMapTree,SIGNAL(sig_chooseSpirit(MapSpirit*)),dockProperty,SLOT(slot_showSpirit(MapSpirit*)));
    connect(dockMapTree,SIGNAL(sig_chooseSpirit(MapSpirit*)),dockView,SLOT(slot_selectChanged(MapSpirit *)));

    connect(dockView,SIGNAL(sig_currentMousePos(QPointF)),this,SLOT(slot_currentMousePos(QPointF)));
    connect(dockView,SIGNAL(sig_cancelTool()),this,SLOT(slot_cancelTool()));
    connect(this,SIGNAL(sig_setTool(int)),dockView,SIGNAL(sig_setTool(int)));

    connect(dockView,SIGNAL(sig_add_remove_spirit()),dockMapTree,SLOT(refresh()));
    connect(dockView,SIGNAL(sig_add_remove_spirit()),dockProperty,SLOT(slot_shownull()));
    connect(dockView,SIGNAL(sig_chooseChanged(MapSpirit*)),dockMapTree,SLOT(slot_chooseChanged(MapSpirit*)));
    connect(dockView,SIGNAL(sig_chooseChanged(MapSpirit*)),dockProperty,SLOT(slot_showSpirit(MapSpirit*)));
    connect(dockView,SIGNAL(sig_propertyChanged(MapSpirit*)),dockProperty,SLOT(slot_propertyChanged(MapSpirit*)));

    //属性窗口 修改 到 地图窗口的信号
    connect(dockProperty,SIGNAL(sig_propertyChanged(MapSpirit *)),dockView,SIGNAL(sig_propertyChangedFromProperty(MapSpirit*)));
    connect(dockProperty,SIGNAL(sig_propertyChanged(MapSpirit *)),dockMapTree,SLOT(refresh()));

    connect(blockView,SIGNAL(sig_chooseSpirit(MapSpirit*)),dockProperty,SLOT(slot_showSpirit(MapSpirit *)));

    connect(this,SIGNAL(sig_setSelectHand()),dockView,SIGNAL(sig_selectHand()));
    connect(this,SIGNAL(sig_setSelectSelect()),dockView,SIGNAL(sig_selectSelect()));
    connect(this,SIGNAL(sig_addBkg(int)),dockView,SLOT(slot_addBkg(int)));
    //connect(this,SIGNAL(sig_addBkg(MapBackground*)),dockMapTree,SLOT(refresh()));

    QMetaObject::connectSlotsByName(this);
}

void MapEditWindow::onServerConnect()
{
    statusbar_info(QStringLiteral("已连接"));
}

void MapEditWindow::onServerDisconnect()
{
    statusbar_info(QStringLiteral("连接已断开"));
}

void MapEditWindow::onServerConnectting()
{
    statusbar_info(QStringLiteral("尝试连接服务器..."));
}

void MapEditWindow::onSendFail()
{
    statusbar_err(QStringLiteral("发送请求失败"));
}

void MapEditWindow::onWaitResponseTimeOut()
{
    statusbar_err(QStringLiteral("等待响应超时"));
}
void MapEditWindow::onTip(QString s)
{
    statusbar_info(s);
}
void MapEditWindow::onErr(int errcode,QString info)
{
    statusbar_err(getErrorString(errcode)+"  "+info);
}

void MapEditWindow::onNewRequest()
{
    statusbar_err("");
    statusbar_info("");
}

void MapEditWindow::createStatusBar()
{
    QStatusBar *statusbar = new QStatusBar(this);
    QLabel *usernamelabel = new QLabel(QStringLiteral("当前用户:")+QString::fromStdString(std::string(current_user_info.username)));
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
    QLabel *userrolelabel = new QLabel(QStringLiteral("当前角色:")+role_name);
    info_label = new QLabel(QStringLiteral("信息:"));
    error_label = new QLabel(QStringLiteral("错误:"));
    pos_label = new  QLabel(QStringLiteral("坐标:"));
    usernamelabel->setMinimumWidth(200);
    userrolelabel->setMinimumWidth(200);
    info_label->setMinimumWidth(200);
    error_label->setMinimumWidth(400);
    pos_label->setMinimumWidth(100);

    statusbar->addWidget(usernamelabel);
    statusbar->addWidget(userrolelabel);
    statusbar->addWidget(info_label);
    statusbar->addWidget(error_label);

    statusbar->addWidget(pos_label);

    setStatusBar(statusbar);
}

void MapEditWindow::createActions()
{

    dockMapTree = new DockMapTree(oneMap);
    dockProperty = new DockProperty(oneMap);
    dockView = new DockView(oneMap);
    blockView = new DockBlock(oneMap);

    addDockWidget(Qt::LeftDockWidgetArea,dockMapTree);
    addDockWidget(Qt::LeftDockWidgetArea,dockProperty);
    addDockWidget(Qt::RightDockWidgetArea,dockView);
    tabifyDockWidget(dockMapTree,blockView);


    QMenu *viewsMenu = menuBar()->addMenu(tr("Views"));
    QMenu *toolsMenu = menuBar()->addMenu(tr("Tools"));
    QToolBar *viewsToolBar = addToolBar(tr("Views"));
    QToolBar *selectToolBar = addToolBar(tr("Selects"));
    QToolBar *toolsToolBar = addToolBar(tr("Tools"));


    viewsMenu->addAction(dockMapTree->toggleViewAction());
    viewsToolBar->addAction(dockMapTree->toggleViewAction());

    viewsMenu->addAction(dockProperty->toggleViewAction());
    viewsToolBar->addAction(dockProperty->toggleViewAction());

    viewsMenu->addAction(dockView->toggleViewAction());
    viewsToolBar->addAction(dockView->toggleViewAction());

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MapEditWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));

    QAction *aboutQtAct = helpMenu->addAction(tr("About &HRG"), this, &MapEditWindow::aboutHrg);
    aboutQtAct->setStatusTip(tr("Show the HRG's About box"));


    selectSelect = new QAction(this);
    selectSelect->setText("select");
    selectSelect->setObjectName("selectSelect");
    //    QIcon iconSelectSelect;
    //    iconSelectSelect.addFile(":/images/toolbar/edit-delete-2.png",QSize(),QIcon::Normal,QIcon::Off);
    //    selectSelect->setIcon(iconSelectSelect);
    selectSelect->setCheckable(true);
    selectToolBar->addAction(selectSelect);
    toolsMenu->addAction(selectSelect);

    selectHand = new QAction(this);
    selectHand->setText("drag");
    selectHand->setObjectName("selectHand");
    //    QIcon iconSelectHand;
    //    iconSelectHand.addFile(":/images/toolbar/point-report.22.png",QSize(),QIcon::Normal,QIcon::Off);
    //    selectHand->setIcon(iconSelectHand);
    selectHand->setCheckable(true);
    selectToolBar->addAction(selectHand);
    toolsMenu->addAction(selectHand);

    toolErase = new QAction(this);
    toolErase->setText("Eraser");
    toolErase->setObjectName("toolErase");
    QIcon iconErase;
    iconErase.addFile(":/images/toolbar/edit-delete-2.png",QSize(),QIcon::Normal,QIcon::Off);
    toolErase->setIcon(iconErase);
    toolErase->setCheckable(true);
    toolsToolBar->addAction(toolErase);
    toolsMenu->addAction(toolErase);

    toolStationDraw = new QAction(this);
    toolStationDraw->setText("Station Draw");
    toolStationDraw->setObjectName("toolStationDraw");
    QIcon iconStationDraw;
    iconStationDraw.addFile(":/images/toolbar/point-report.22.png",QSize(),QIcon::Normal,QIcon::Off);
    toolStationDraw->setIcon(iconStationDraw);
    toolStationDraw->setCheckable(true);
    toolsToolBar->addAction(toolStationDraw);
    toolsMenu->addAction(toolStationDraw);

    toolStationReport = new QAction(this);
    toolStationReport->setText("Station Report");
    toolStationReport->setObjectName("toolStationReport");
    QIcon iconStationReport;
    iconStationReport.addFile(":/images/toolbar/point-halt.22.png",QSize(),QIcon::Normal,QIcon::Off);
    toolStationReport->setIcon(iconStationReport);
    toolStationReport->setCheckable(true);
    toolsToolBar->addAction(toolStationReport);
    toolsMenu->addAction(toolStationReport);

    toolStationHalt = new QAction(this);
    toolStationHalt->setText("Station Halt");
    toolStationHalt->setObjectName("toolStationHalt");
    QIcon iconStationHalt;
    iconStationHalt.addFile(":/images/toolbar/point-park.22.png",QSize(),QIcon::Normal,QIcon::Off);
    toolStationHalt->setIcon(iconStationHalt);
    toolStationHalt->setCheckable(true);
    toolsToolBar->addAction(toolStationHalt);
    toolsMenu->addAction(toolStationHalt);

    toolStationCharge = new QAction(this);
    toolStationCharge->setText("Station Charge");
    toolStationCharge->setObjectName("toolStationCharge");
    QIcon iconStationCharge;
    iconStationCharge.addFile(":/images/point/ChargingStation.20x20.png",QSize(),QIcon::Normal,QIcon::Off);
    toolStationCharge->setIcon(iconStationCharge);
    toolStationCharge->setCheckable(true);
    toolsToolBar->addAction(toolStationCharge);
    toolsMenu->addAction(toolStationCharge);

    toolStationLoad = new QAction(this);
    toolStationLoad->setText("Station Load");
    toolStationLoad->setObjectName("toolStationLoad");
    QIcon iconStationLoad;
    iconStationLoad.addFile(":/images/point/TransferStation.20x20.png",QSize(),QIcon::Normal,QIcon::Off);
    toolStationLoad->setIcon(iconStationLoad);
    toolStationLoad->setCheckable(true);
    toolsToolBar->addAction(toolStationLoad);
    toolsMenu->addAction(toolStationLoad);

    toolStationUnload = new QAction(this);
    toolStationUnload->setText("Station Unload");
    toolStationUnload->setObjectName("toolStationUnload");
    QIcon iconStationUnload;
    iconStationUnload.addFile(":/images/point/TransferStation.20x20.png",QSize(),QIcon::Normal,QIcon::Off);
    toolStationUnload->setIcon(iconStationUnload);
    toolStationUnload->setCheckable(true);
    toolsToolBar->addAction(toolStationUnload);
    toolsMenu->addAction(toolStationUnload);

    toolStationLoadUnload = new QAction(this);
    toolStationLoadUnload->setText("Station Load Unload");
    toolStationLoadUnload->setObjectName("toolStationLoadUnload");
    QIcon iconStationLoadUnload;
    iconStationLoadUnload.addFile(":/images/point/TransferStation.20x20.png",QSize(),QIcon::Normal,QIcon::Off);
    toolStationLoadUnload->setIcon(iconStationLoadUnload);
    toolStationLoadUnload->setCheckable(true);
    toolsToolBar->addAction(toolStationLoadUnload);
    toolsMenu->addAction(toolStationLoadUnload);

    toolLineDouble = new QAction(this);
    toolLineDouble->setText("Double direction Line");
    toolLineDouble->setObjectName("toolLineDouble");
    QIcon iconLine;
    iconLine.addFile(":/images/toolbar/path-direct.22.png",QSize(),QIcon::Normal,QIcon::Off);
    toolLineDouble->setIcon(iconLine);
    toolLineDouble->setCheckable(true);

    toolLineSingle = new QAction(this);
    toolLineSingle->setText("Single direction Line");
    toolLineSingle->setObjectName("toolLineSingle");
    toolLineSingle->setIcon(iconLine);
    toolLineSingle->setCheckable(true);


    toolQbDouble = new QAction(this);
    toolQbDouble->setText("Double 2-Bezier");
    toolQbDouble->setObjectName("toolQbDouble");
    QIcon iconQb;
    iconQb.addFile(":/images/toolbar/path-bezier.22.png",QSize(),QIcon::Normal,QIcon::Off);
    toolQbDouble->setIcon(iconQb);
    toolQbDouble->setCheckable(true);

    toolQbSingle = new QAction(this);
    toolQbSingle->setText("Single 2-Bezier");
    toolQbSingle->setObjectName("toolQbSingle");
    toolQbSingle->setIcon(iconQb);
    toolQbSingle->setCheckable(true);

    toolCbDouble = new QAction(this);
    toolCbDouble->setText("Double 3-Bezier");
    toolCbDouble->setObjectName("toolCbDouble");
    QIcon iconCb;
    iconCb.addFile(":/images/toolbar/path-bezier.22.png",QSize(),QIcon::Normal,QIcon::Off);
    toolCbDouble->setIcon(iconCb);
    toolCbDouble->setCheckable(true);

    toolCbSingle = new QAction(this);
    toolCbSingle->setText("Single 3-Bezier");
    toolCbSingle->setObjectName("toolCbSingle");
    toolCbSingle->setIcon(iconCb);
    toolCbSingle->setCheckable(true);

    toolLineMenu = new QMenu("Line");
    toolLineMenu->setObjectName("toolLineMenu");
    toolLineMenu->setIcon(iconLine);
    toolLineMenu->addAction(toolLineDouble);
    toolLineMenu->addAction(toolLineSingle);
    connect(toolLineMenu->menuAction(),SIGNAL(triggered(bool)),this,SLOT(on_toolLineDouble_triggered(bool)));
    toolsMenu->addMenu(toolLineMenu);
    toolsToolBar->addAction(toolLineMenu->menuAction());

    toolQbMenu = new QMenu("Line");
    toolQbMenu->setObjectName("toolQbMenu");
    toolQbMenu->setIcon(iconQb);
    toolQbMenu->addAction(toolQbDouble);
    toolQbMenu->addAction(toolQbSingle);
    connect(toolQbMenu->menuAction(),SIGNAL(triggered(bool)),this,SLOT(on_toolQbDouble_triggered(bool)));
    toolsMenu->addMenu(toolQbMenu);
    toolsToolBar->addAction(toolQbMenu->menuAction());

    toolCbMenu = new QMenu("Line");
    toolCbMenu->setObjectName("toolCbMenu");
    toolCbMenu->setIcon(iconCb);
    toolCbMenu->addAction(toolCbDouble);
    toolCbMenu->addAction(toolCbSingle);
    connect(toolCbMenu->menuAction(),SIGNAL(triggered(bool)),this,SLOT(on_toolCbDouble_triggered(bool)));
    toolsMenu->addMenu(toolCbMenu);
    toolsToolBar->addAction(toolCbMenu->menuAction());

    addBkgd = new QAction(this);
    addBkgd->setText("set background img");
    addBkgd->setObjectName("addBkgd");
    //            QIcon iconAddBkg;
    //            iconAddBkg.addFile(":/images/toolbar/path-bezier.22.png",QSize(),QIcon::Normal,QIcon::Off);
    //            addBkgd->setIcon(iconCb);
    addBkgd->setCheckable(true);
    toolsToolBar->addAction(addBkgd);
    toolsMenu->addAction(addBkgd);

}

void MapEditWindow::about()
{
    QMessageBox::about(this, tr("About Application"),
                       tr("The <b>HRG Dispatch client</b> connect to dispatch server,<br>"
                          "and show the ui for user to watch the status of agvs and<br>"
                          "other information about the agv dispatch system."));
}

void MapEditWindow::aboutHrg()
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

void MapEditWindow::statusbar_info(QString msg)
{
    info_label->setText(QStringLiteral("信息:")+msg);
}

void MapEditWindow::statusbar_err(QString msg)
{
    error_label->setText(QStringLiteral("错误:")+msg);
}

void MapEditWindow::statusbar_pos(QString msg)
{
    pos_label->setText(QStringLiteral("坐标:")+msg);
}

void MapEditWindow::slot_currentMousePos(QPointF pos)
{
    statusbar_pos(QString("(%1,%2)").arg(pos.x()).arg(pos.y()));
}

void MapEditWindow::slot_cancelTool()
{
    toolErase->setChecked(false);
    toolStationDraw->setChecked(false);
    toolStationReport->setChecked(false);
    toolStationHalt->setChecked(false);
    toolStationCharge->setChecked(false);
    toolStationLoad->setChecked(false);
    toolStationUnload->setChecked(false);
    toolStationLoadUnload->setChecked(false);
    toolLineSingle->setChecked(false);
    toolQbSingle->setChecked(false);
    toolCbSingle->setChecked(false);
    toolLineDouble->setChecked(false);
    toolQbDouble->setChecked(false);
    toolCbDouble->setChecked(false);

    selectSelect->setChecked(true);
    selectHand->setChecked(false);
    emit sig_setTool(Scene::T_NONE);
}

void MapEditWindow::on_toolErase_triggered(bool b)
{
    //toolErase->setChecked(false);
    toolStationDraw->setChecked(false);
    toolStationReport->setChecked(false);
    toolStationHalt->setChecked(false);
    toolStationCharge->setChecked(false);
    toolStationLoad->setChecked(false);
    toolStationUnload->setChecked(false);
    toolStationLoadUnload->setChecked(false);
    toolLineSingle->setChecked(false);
    toolQbSingle->setChecked(false);
    toolCbSingle->setChecked(false);
    toolLineDouble->setChecked(false);
    toolQbDouble->setChecked(false);
    toolCbDouble->setChecked(false);


    if(b){
        emit sig_setTool(Scene::T_ERASER);
    }else{
        emit sig_setTool(Scene::T_NONE);
    }
}

void MapEditWindow::on_toolStationDraw_triggered(bool b)
{
    toolErase->setChecked(false);
    //toolStationDraw->setChecked(false);
    toolStationReport->setChecked(false);
    toolStationHalt->setChecked(false);
    toolStationCharge->setChecked(false);
    toolStationLoad->setChecked(false);
    toolStationUnload->setChecked(false);
    toolStationLoadUnload->setChecked(false);
    toolLineSingle->setChecked(false);
    toolQbSingle->setChecked(false);
    toolCbSingle->setChecked(false);
    toolLineDouble->setChecked(false);
    toolQbDouble->setChecked(false);
    toolCbDouble->setChecked(false);

    if(b){
        emit sig_setTool(Scene::T_STATION_DRAW);
    }else{
        emit sig_setTool(Scene::T_NONE);
    }
}

void MapEditWindow::on_toolStationReport_triggered(bool b)
{
    toolErase->setChecked(false);
    toolStationDraw->setChecked(false);
    //toolStationReport->setChecked(false);
    toolStationHalt->setChecked(false);
    toolStationCharge->setChecked(false);
    toolStationLoad->setChecked(false);
    toolStationUnload->setChecked(false);
    toolStationLoadUnload->setChecked(false);
    toolLineSingle->setChecked(false);
    toolQbSingle->setChecked(false);
    toolCbSingle->setChecked(false);
    toolLineDouble->setChecked(false);
    toolQbDouble->setChecked(false);
    toolCbDouble->setChecked(false);


    if(b){
        emit sig_setTool(Scene::T_STATION_REPORT);
    }else{
        emit sig_setTool(Scene::T_NONE);
    }
}

void MapEditWindow::on_toolStationHalt_triggered(bool b)
{
    toolErase->setChecked(false);
    toolStationDraw->setChecked(false);
    toolStationReport->setChecked(false);
    //toolStationHalt->setChecked(false);
    toolStationCharge->setChecked(false);
    toolStationLoad->setChecked(false);
    toolStationUnload->setChecked(false);
    toolStationLoadUnload->setChecked(false);
    toolLineSingle->setChecked(false);
    toolQbSingle->setChecked(false);
    toolCbSingle->setChecked(false);
    toolLineDouble->setChecked(false);
    toolQbDouble->setChecked(false);
    toolCbDouble->setChecked(false);


    if(b){
        emit sig_setTool(Scene::T_STATION_HALT);
    }else{
        emit sig_setTool(Scene::T_NONE);
    }
}
void MapEditWindow::on_toolStationCharge_triggered(bool b)
{
    toolErase->setChecked(false);
    toolStationDraw->setChecked(false);
    toolStationReport->setChecked(false);
    toolStationHalt->setChecked(false);
    //toolStationCharge->setChecked(false);
    toolStationLoad->setChecked(false);
    toolStationUnload->setChecked(false);
    toolStationLoadUnload->setChecked(false);
    toolLineSingle->setChecked(false);
    toolQbSingle->setChecked(false);
    toolCbSingle->setChecked(false);
    toolLineDouble->setChecked(false);
    toolQbDouble->setChecked(false);
    toolCbDouble->setChecked(false);


    if(b){
        emit sig_setTool(Scene::T_STATION_CHARGE);
    }else{
        emit sig_setTool(Scene::T_NONE);
    }
}
void MapEditWindow::on_toolStationLoad_triggered(bool b)
{
    toolErase->setChecked(false);
    toolStationDraw->setChecked(false);
    toolStationReport->setChecked(false);
    toolStationHalt->setChecked(false);
    toolStationCharge->setChecked(false);
    //toolStationLoad->setChecked(false);
    toolStationUnload->setChecked(false);
    toolStationLoadUnload->setChecked(false);
    toolLineSingle->setChecked(false);
    toolQbSingle->setChecked(false);
    toolCbSingle->setChecked(false);
    toolLineDouble->setChecked(false);
    toolQbDouble->setChecked(false);
    toolCbDouble->setChecked(false);


    if(b){
        emit sig_setTool(Scene::T_STATION_LOAD);
    }else{
        emit sig_setTool(Scene::T_NONE);
    }
}
void MapEditWindow::on_toolStationUnload_triggered(bool b)
{
    toolErase->setChecked(false);
    toolStationDraw->setChecked(false);
    toolStationReport->setChecked(false);
    toolStationHalt->setChecked(false);
    toolStationCharge->setChecked(false);
    toolStationLoad->setChecked(false);
    //toolStationUnload->setChecked(false);
    toolStationLoadUnload->setChecked(false);
    toolLineSingle->setChecked(false);
    toolQbSingle->setChecked(false);
    toolCbSingle->setChecked(false);
    toolLineDouble->setChecked(false);
    toolQbDouble->setChecked(false);
    toolCbDouble->setChecked(false);


    if(b){
        emit sig_setTool(Scene::T_STATION_UNLOAD);
    }else{
        emit sig_setTool(Scene::T_NONE);
    }
}

void MapEditWindow::on_toolStationLoadUnload_triggered(bool b)
{
    toolErase->setChecked(false);
    toolStationDraw->setChecked(false);
    toolStationReport->setChecked(false);
    toolStationHalt->setChecked(false);
    toolStationCharge->setChecked(false);
    toolStationLoad->setChecked(false);
    toolStationUnload->setChecked(false);
    //toolStationLoadUnload->setChecked(false);
    toolLineSingle->setChecked(false);
    toolQbSingle->setChecked(false);
    toolCbSingle->setChecked(false);
    toolLineDouble->setChecked(false);
    toolQbDouble->setChecked(false);
    toolCbDouble->setChecked(false);


    if(b){
        emit sig_setTool(Scene::T_STATION_LOAD_UNLOAD);
    }else{
        emit sig_setTool(Scene::T_NONE);
    }
}

void MapEditWindow::on_toolLineSingle_triggered(bool b)
{
    toolErase->setChecked(false);
    toolStationDraw->setChecked(false);
    toolStationReport->setChecked(false);
    toolStationHalt->setChecked(false);
    toolStationCharge->setChecked(false);
    toolStationLoad->setChecked(false);
    toolStationUnload->setChecked(false);
    toolStationLoadUnload->setChecked(false);

    //toolLineSingle->setChecked(false);
    toolQbSingle->setChecked(false);
    toolCbSingle->setChecked(false);
    toolLineDouble->setChecked(false);
    toolQbDouble->setChecked(false);
    toolCbDouble->setChecked(false);


    if(b){
        emit sig_setTool(Scene::T_LINE_SINGLE);
    }else{
        emit sig_setTool(Scene::T_NONE);
    }
}
void MapEditWindow::on_toolQbSingle_triggered(bool b)
{
    toolErase->setChecked(false);
    toolStationDraw->setChecked(false);
    toolStationReport->setChecked(false);
    toolStationHalt->setChecked(false);
    toolStationCharge->setChecked(false);
    toolStationLoad->setChecked(false);
    toolStationUnload->setChecked(false);
    toolStationLoadUnload->setChecked(false);
    toolLineSingle->setChecked(false);
    //toolQbSingle->setChecked(false);
    toolCbSingle->setChecked(false);
    toolLineDouble->setChecked(false);
    toolQbDouble->setChecked(false);
    toolCbDouble->setChecked(false);

    if(b){
        emit sig_setTool(Scene::T_QB_SINGLE);
    }else{
        emit sig_setTool(Scene::T_NONE);
    }
}
void MapEditWindow::on_toolCbSingle_triggered(bool b)
{
    toolErase->setChecked(false);
    toolStationDraw->setChecked(false);
    toolStationReport->setChecked(false);
    toolStationHalt->setChecked(false);
    toolStationCharge->setChecked(false);
    toolStationLoad->setChecked(false);
    toolStationUnload->setChecked(false);
    toolStationLoadUnload->setChecked(false);
    toolLineSingle->setChecked(false);
    toolQbSingle->setChecked(false);
    //toolCbSingle->setChecked(false);
    toolLineDouble->setChecked(false);
    toolQbDouble->setChecked(false);
    toolCbDouble->setChecked(false);

    if(b){
        emit sig_setTool(Scene::T_CB_SINGLE);
    }else{
        emit sig_setTool(Scene::T_NONE);
    }
}

void MapEditWindow::on_toolLineDouble_triggered(bool b)
{
    toolErase->setChecked(false);
    toolStationDraw->setChecked(false);
    toolStationReport->setChecked(false);
    toolStationHalt->setChecked(false);
    toolStationCharge->setChecked(false);
    toolStationLoad->setChecked(false);
    toolStationUnload->setChecked(false);
    toolStationLoadUnload->setChecked(false);
    toolLineSingle->setChecked(false);
    toolQbSingle->setChecked(false);
    toolCbSingle->setChecked(false);
    //toolLineDouble->setChecked(false);
    toolQbDouble->setChecked(false);
    toolCbDouble->setChecked(false);


    if(b){
        emit sig_setTool(Scene::T_LINE_DOUBLE);
    }else{
        emit sig_setTool(Scene::T_NONE);
    }
}
void MapEditWindow::on_toolQbDouble_triggered(bool b)
{
    toolErase->setChecked(false);
    toolStationDraw->setChecked(false);
    toolStationReport->setChecked(false);
    toolStationHalt->setChecked(false);
    toolStationCharge->setChecked(false);
    toolStationLoad->setChecked(false);
    toolStationUnload->setChecked(false);
    toolStationLoadUnload->setChecked(false);
    toolLineSingle->setChecked(false);
    toolQbSingle->setChecked(false);
    toolCbSingle->setChecked(false);
    toolLineDouble->setChecked(false);
    //toolQbDouble->setChecked(false);
    toolCbDouble->setChecked(false);

    if(b){
        emit sig_setTool(Scene::T_QB_DOUBLE);
    }else{
        emit sig_setTool(Scene::T_NONE);
    }
}
void MapEditWindow::on_toolCbDouble_triggered(bool b)
{
    toolErase->setChecked(false);
    toolStationDraw->setChecked(false);
    toolStationReport->setChecked(false);
    toolStationHalt->setChecked(false);
    toolStationCharge->setChecked(false);
    toolStationLoad->setChecked(false);
    toolStationUnload->setChecked(false);
    toolStationLoadUnload->setChecked(false);
    toolLineSingle->setChecked(false);
    toolQbSingle->setChecked(false);
    toolCbSingle->setChecked(false);
    toolLineDouble->setChecked(false);
    toolQbDouble->setChecked(false);
    //toolCbDouble->setChecked(false);

    if(b){
        emit sig_setTool(Scene::T_CB_DOUBLE);
    }else{
        emit sig_setTool(Scene::T_NONE);
    }
}

void MapEditWindow::on_selectSelect_triggered(bool b)
{
    toolErase->setChecked(false);
    toolStationDraw->setChecked(false);
    toolStationReport->setChecked(false);
    toolStationHalt->setChecked(false);
    toolStationCharge->setChecked(false);
    toolStationLoad->setChecked(false);
    toolStationUnload->setChecked(false);
    toolStationLoadUnload->setChecked(false);
    toolLineSingle->setChecked(false);
    toolQbSingle->setChecked(false);
    toolCbSingle->setChecked(false);
    toolLineDouble->setChecked(false);
    toolQbDouble->setChecked(false);
    toolCbDouble->setChecked(false);
    //selectSelect->setChecked(false);
    selectHand->setChecked(false);
    emit sig_setTool(Scene::T_NONE);
    emit sig_setSelectSelect();
}

void MapEditWindow::on_selectHand_triggered(bool b)
{
    toolErase->setChecked(false);
    toolStationDraw->setChecked(false);
    toolStationReport->setChecked(false);
    toolStationHalt->setChecked(false);
    toolStationCharge->setChecked(false);
    toolStationLoad->setChecked(false);
    toolStationUnload->setChecked(false);
    toolStationLoadUnload->setChecked(false);
    toolLineSingle->setChecked(false);
    toolQbSingle->setChecked(false);
    toolCbSingle->setChecked(false);
    toolLineDouble->setChecked(false);
    toolQbDouble->setChecked(false);
    toolCbDouble->setChecked(false);
    selectSelect->setChecked(false);
    //selectHand->setChecked(false);
    emit sig_setTool(Scene::T_NONE);
    if(b)
        emit sig_setSelectHand();
    else
        emit sig_setSelectSelect();
}

void MapEditWindow::on_addBkgd_triggered(bool b)
{
    if(b){
        toolErase->setChecked(false);
        toolStationDraw->setChecked(false);
        toolStationReport->setChecked(false);
        toolStationHalt->setChecked(false);
        toolStationCharge->setChecked(false);
        toolStationLoad->setChecked(false);
        toolStationUnload->setChecked(false);
        toolStationLoadUnload->setChecked(false);
        toolLineSingle->setChecked(false);
        toolQbSingle->setChecked(false);
        toolCbSingle->setChecked(false);
        toolLineDouble->setChecked(false);
        toolQbDouble->setChecked(false);
        toolCbDouble->setChecked(false);
        selectSelect->setChecked(false);
        selectHand->setChecked(false);
        emit sig_setTool(Scene::T_NONE);

        if(oneMap->getFloors().size()>0){
            QString filePath = QFileDialog::getOpenFileName(this,tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
            if(filePath.length()>0){
                QString fileName = filePath.right(filePath.length() - filePath.lastIndexOf("/")-1);
                QImage img;
                img.load(filePath);

                QByteArray ba;
                QDataStream ds(&ba,QIODevice::WriteOnly);
                ds<<img;
                qDebug()<<ba.size();

                MapBackground *_bkg = new MapBackground(oneMap->getNextId(),fileName.toStdString(),ba.data(),ba.size(),img.width(),img.height(),fileName.toStdString());
                oneMap->addSpirit(_bkg);
                emit sig_addBkg(_bkg->getId());
            }
        }
        addBkgd->setChecked(false);
    }
}
