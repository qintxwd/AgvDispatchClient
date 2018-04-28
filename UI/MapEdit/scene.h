#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include "scenepositionlabel.h"
#include "itembackground.h"
#include "itemarc.h"
#include "itemline.h"
#include "itemstation.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(QObject *parent = nullptr);
    ~Scene();

    enum{
        GRID_SIZE = 10,
    };

    enum Tool
    {
        T_NONE = -1,//无
        T_ERASER,//橡皮擦
        T_STATION,//站点
        T_LINE,//直线
        T_ARC,//弧线
    };

    void build(int width = -1, int height = -1);

    void setCurTool(Tool);

    int width();

    int height();

    QPoint calGridPos(const QPointF &);

    void load();

    bool outOfLimitLine(const QPointF &grid_pos);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent  *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
signals:
    void cancelTool();
public slots:
    void setBackgroundImagePath(QString _path);

    void onSelectItemChanged();
    void onLoadStationSuccess();
    void onLoadLineSuccess();
private:

    Tool cur_tool;

    int _width;

    int _height;

    void drawLimitLine();

    ScenePositionLabel *positoinlabel;

    ItemStation *oldSelectStation;//用于绘制线路，这个是选择的一个起点

    ItemStation *getStation(int id);

    int getMaxStationId();

    ItemLine *getLine(int id);

    int getMaxLineId();

    ItemArc *getArc(int id);

    int getMaxArcId();

    void removeAllStations();
    void removeAllLines();
    void removeAllArcs();
public:
    ItemBackground *itemBackground;

    QList<ItemStation *> iStations;//记录所有的站点

    QList<ItemLine *> iLines;//记录所有的直线

    QList<ItemArc *> iArcs;//记录所有的弧线

    QString backgroundImagePath;//记录背景图片的路径
};

#endif // SCENE_H
