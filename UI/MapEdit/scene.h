#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include "scenepositionlabel.h"
#include "../../data/onemap.h"
#include "mapitemline.h"
#include "mapitemstation.h"
#include "mapitemcubicbezier.h"
#include "mapitemstationname.h"
#include "mapitemquadraticbezier.h"
#include "mapitembkg.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(OneMap *_onemap,MapFloor *_floor,QObject *parent = nullptr);
    ~Scene();

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
        T_LINE,//直线
        T_QB,//弧线1
        T_CB,//弧线2
    };

    void build();

    MapFloor *getFloor(){return floor;}

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent  *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
signals:
    void sig_cancelTool();

    //添加站点
    void sig_addStation(MapPoint *spirit);

    //删除站点
    void sig_removeStation(MapPoint *spirit);

    //添加路径
    void sig_addPath(MapPath *spirit);

    //删除路径
    void sig_removePath(MapPath *spirit);

    //删除背景
    void sig_removeBkg(MapBackground *spirit);

    //站点属性修改
    void sig_stationPropertyChanged(MapPoint *spirit);

    //路径属性修改
    void sig_pathPropertyChanged(MapPath *spirit);

    //背景图片属性修改
    void sig_bgkPropertyChanged(MapBackground *spirit);

    //选择修改
    void sig_chooseChanged(MapSpirit *spirit);

    void sig_currentMousePos(QPointF pos);

public slots:

    //菜单触动,修改背景
    void setBackgroundImagePath(QString _path);

    //TODO 修改工具
    void toolChanged();

    //tree触动，添加删除节点
    void addSpirit(MapFloor *_floor, MapSpirit *_spirit);
    void removeSpirit(MapFloor *_floor, MapSpirit *_spirit);

    //property触动，修改属性
    void propertyChanged(MapFloor *_floor,MapSpirit *_spirit);

    ////
    void onSelectItemChanged();

    //选择条目 被动变换了
    void slot_selectItem(MapSpirit *_spirit);

    void slot_setCurTool(int t);
private:

    Tool cur_tool;

//    ScenePositionLabel *positoinlabel;

    MapItemStation *oldSelectStation;//用于绘制线路，这个是选择的一个起点

private:

    QList<MapItemStation *> iStations;//记录所有的站点

    QList<MapItemStationName *> iStationNames;//记录所有的站点的名字

    QList<MapItemLine *> iLines;//记录所有的直线

    QList<MapItemQuadraticBezier *> iQbs;

    QList<MapItemCubicBezier *> iCbs;

    MapItemBkg *bkg;
private:
    OneMap *onemap;
    MapFloor *floor;
};

#endif // SCENE_H
