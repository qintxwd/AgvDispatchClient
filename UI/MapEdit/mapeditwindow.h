#ifndef MAPEDITWINDOW_H
#define MAPEDITWINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include "../../data/onemap.h"

class DockView;
class DockMapTree;
class DockProperty;
class DockBlock;

class MapEditWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MapEditWindow(OneMap *_oneMap,QWidget *parent = nullptr);
    ~MapEditWindow();
    void init();
signals:
    void sig_setTool(int);

    void sig_setSelectSelect();
    void sig_setSelectHand();

    void sig_addBkg(int _bkg);
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

    void slot_load_map_success();
    void slot_set_map_success();

    void slot_currentMousePos(QPointF pos);

    void slot_cancelTool();

    //几个action
    void on_toolErase_triggered(bool b);
    void on_toolStationDraw_triggered(bool b);
    void on_toolStationReport_triggered(bool b);
    void on_toolStationHalt_triggered(bool b);
    void on_toolStationCharge_triggered(bool b);
    void on_toolStationLoad_triggered(bool b);
    void on_toolStationUnload_triggered(bool b);
    void on_toolStationLoadUnload_triggered(bool b);
    void on_toolStationOrigin_triggered(bool b);
    void on_toolLine_triggered(bool b);
    void on_toolQb_triggered(bool b);
    void on_toolCb_triggered(bool b);
    void on_addBkgd_triggered(bool b);
    void on_selectSelect_triggered(bool b);
    void on_selectHand_triggered(bool b);
    void on_toolSave_triggered(bool b);
    void on_toolLoad_triggered(bool b);
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

    OneMap *oneMap;//地图编辑器的一个map

    DockMapTree *dockMapTree;
    DockProperty *dockProperty;
    DockView *dockView;
    DockBlock *blockView;

    QAction *selectSelect;
    QAction *selectHand;

    QAction *toolErase;
    QAction *toolStationDraw;
    QAction *toolStationReport;
    QAction *toolStationHalt;
    QAction *toolStationCharge;
    QAction *toolStationLoad;
    QAction *toolStationUnload;
    QAction *toolStationLoadUnload;
    QAction *toolStationOrigin;
    QAction *toolLine;
    QAction *toolQb;
    QAction *toolCb;

    QAction *toolSave;
    QAction *toolLoad;


    QAction *addBkgd;
};

#endif // MAPEDITWINDOW_H
