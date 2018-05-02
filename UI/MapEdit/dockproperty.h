#ifndef DOCKPROPERTY_H
#define DOCKPROPERTY_H

#include <QDockWidget>
#include <QTableWidget>
#include "../../data/onemap.h"

class DockProperty : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockProperty(OneMap* _oneMap, QWidget *parent = nullptr);

signals:
    void sig_PointNameChanged(MapSpirit *spirit,QString name);
    void sig_PointTypeChanged(MapSpirit *spirit,int _type);
    void sig_PointXChanged(MapSpirit *spirit,QString x);
    void sig_PointYChanged(MapSpirit *spirit,QString y);
    void sig_PointRealXChanged(MapSpirit *spirit,QString realx);
    void sig_PointRealYChanged(MapSpirit *spirit,QString realy);
    void sig_PointLabelXoffsetChanged(MapSpirit *spirit,QString labelXoffset);
    void sig_PointLabelYoffsetChanged(MapSpirit *spirit,QString labelYoffset);
    void sig_PointMapChangeChanged(MapSpirit *spirit,int mapchange);

    void sig_PathNameChanged(MapSpirit *spirit,QString name);
    void sig_PathTypeChanged(MapSpirit *spirit,int _type);
    void sig_PathP1XChanged(MapSpirit *spirit,QString p1x);
    void sig_PathP1YChanged(MapSpirit *spirit,QString p1y);
    void sig_PathP2XChanged(MapSpirit *spirit,QString p2x);
    void sig_PathP2YChanged(MapSpirit *spirit,QString p2y);
    void sig_LengthChanged(MapSpirit *spirit,QString length);

    void sig_FloorNameChanged(MapSpirit *spirit,QString name);
public slots:
    void slot_showSpirit(MapSpirit *spirit);
private slots:
    void slot_PointNameChanged(QString name);
    void slot_PointTypeChanged(int _type);
    void slot_PointXChanged(QString x);
    void slot_PointYChanged(QString y);
    void slot_PointRealXChanged(QString realx);
    void slot_PointRealYChanged(QString realy);
    void slot_PointLabelXoffsetChanged(QString labelXoffset);
    void slot_PointLabelYoffsetChanged(QString labelYoffset);
    void slot_PointMapChangeChanged(int mapchange);

    void slot_PathNameChanged(QString name);
    void slot_PathTypeChanged(int _type);
    void slot_PathP1XChanged(QString p1x);
    void slot_PathP1YChanged(QString p1y);
    void slot_PathP2XChanged(QString p2x);
    void slot_PathP2YChanged(QString p2y);
    void slot_LengthChanged(QString length);

    void slot_FloorNameChanged(QString name);
private:
    void showPoint();
    void showPath();
    void showFloor();
    MapSpirit *spirit;
    QTableWidget *tableWidget;
    OneMap* oneMap;
};

#endif // DOCKPROPERTY_H
