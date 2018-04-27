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

public slots:
    void showSpirit(int id);
private slots:
    void slot_PointNameChanged(QString name);
    void slot_PointTypeChanged(int type);
    void slot_PointXChanged(QString x);
    void slot_PointYChanged(QString y);
    void slot_PointRealXChanged(QString realx);
    void slot_PointRealYChanged(QString realy);
    void slot_PointLabelXoffsetChanged(QString labelXoffset);
    void slot_PointLabelYoffsetChanged(QString labelYoffset);
    void slot_PointMapChangeChanged(int mapchange);

    void slot_PathNameChanged(QString name);
    void slot_PathTypeChanged(int type);
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
