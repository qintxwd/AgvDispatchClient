#ifndef MAPGROUP_H
#define MAPGROUP_H

#include <QObject>
#include "mapspirit.h"

//一个区块，只允许部分车辆通行
class MapGroup : public MapSpirit
{
    Q_OBJECT
public:
    explicit MapGroup(int _id, QString _name, QObject *parent = nullptr);
    MapGroup(const MapGroup& b);

    void init(QList<int> _spirits,QList<int> _agvs);

    void addSpirit(int spirit){spirits.append(spirit);}
    void removeSpirit(int spirit){spirits.removeAll(spirit);}
    QList<int> getSpirits(){return spirits;}

    void addAgv(int agv){agvs.append(agv);}
    void removeAgv(int agv){agvs.removeAll(agv);}
    QList<int> getAgvs(){return agvs;}

signals:

public slots:

private:
    QList<int> spirits;//区块
    QList<int> agvs;//通行车辆
};

#endif // MAPGROUP_H
