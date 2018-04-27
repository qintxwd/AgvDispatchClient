#ifndef MAPFLOOR_H
#define MAPFLOOR_H

#include <QList>

#include "mappath.h"

class MapFloor : public MapSpirit
{
public:
    MapFloor(int _id, QString _name);
    void addPoint(int p);
    void addPath(int p);

    QList<int> getPoints(){return points;}
    QList<int> getPaths(){return paths;}
private:
    QList<int> points;
    QList<int> paths;
};

#endif // MAPFLOOR_H
