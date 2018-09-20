#ifndef MONITORSCENE_H
#define MONITORSCENE_H

#include <QGraphicsScene>
#include "../../data/onemap.h"
#include "monitormapitemline.h"
#include "monitormapitemstation.h"
#include "monitormapitemcubicbezier.h"
#include "monitormapitemstationname.h"
#include "monitormapitemquadraticbezier.h"
#include "monitormapitembkg.h"
#include "monitormapitemagv.h"

class MonitorScene: public QGraphicsScene
{
    Q_OBJECT
public:
    MonitorScene(OneMap *_onemap,MapFloor *_floor,QObject *parent = nullptr);
    ~MonitorScene();

    enum Tool
    {
        T_NONE = -1,//无
        T_ERASER,//橡皮擦
        T_STATION_DRAW,//站点
        T_STATION_REPORT,//站点
        T_STATION_HALT,//站点
        T_STATION_CHARGE,//站点
        T_STATION_LOAD,//站点
        T_STATION_UNLOAD,//站点
        T_STATION_LOAD_UNLOAD,//站点
        T_STATION_ORIGIN,//站点
        T_LINE,//直线
        T_QB,//弧线1
        T_CB,//弧线2
    };

    void build();

    MapFloor *getFloor(){return floor;}

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent  *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);
signals:
    void sig_cancelTool();

    void sig_add_remove_spirit();

    //属性修改
    void sig_propertyChanged(MapSpirit *spirit);

    //选择修改
    void sig_chooseChanged(MapSpirit *spirit);

    void sig_currentMousePos(QPointF pos);

    void sig_pub_agv_postion(int id,QString name,double x,double y,double theta);
public slots:

//    //菜单触动,修改背景
//    void setBackgroundImagePath(QString _path);

    //tree触动，添加删除节点
    void addSpirit(MapFloor *_floor, MapSpirit *_spirit);
    //void removeSpirit(MapFloor *_floor, MapSpirit *_spirit);

    //property触动，修改属性
    void propertyChanged(MapSpirit *_spirit);

    ////
    void onSelectItemChanged();

    //选择条目 被动变换了
    void slot_selectItem(MapSpirit *_spirit);

    void slot_setCurTool(int t);

    void slot_pub_agv_postion(int id, QString name, double x, double y, double theta,QStringList qsl, int floor);

    void slot_agv_occus_set_color();
private:

    Tool cur_tool;

    MonitorMapItemStation *oldSelectStation;//用于绘制线路，这个是选择的一个起点

    QMap<int,QStringList> agvOccus;
private:

    QList<MonitorMapItemStation *> iStations;//记录所有的站点

    QList<MonitorMapItemStationName *> iStationNames;//记录所有的站点的名字

    QList<MonitorMapItemLine *> iLines;//记录所有的直线

    QList<MonitorMapItemQuadraticBezier *> iQbs;

    QList<MonitorMapItemCubicBezier *> iCbs;

    QList<MonitorMapItemAgv *> agvs;

    MonitorMapItemBkg *bkg;
private:
    OneMap *onemap;
    MapFloor *floor;

    QList<int> agvIds;
};

#endif // MONITORSCENE_H
