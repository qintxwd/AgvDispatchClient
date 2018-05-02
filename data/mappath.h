#ifndef MAPPATH_H
#define MAPPATH_H

#include "mappoint.h"


class MapPath : public MapSpirit
{
public:
    enum Map_Path_Type{
        Map_Path_Type_Line = 0,
        Map_Path_Type_Quadratic_Bezier,
        Map_Path_Type_Cubic_Bezier,
    };

    MapPath(int _id, QString _name, int _start, int _end, Map_Path_Type _type,int _length, int _p1x = 0, int _p1y = 0, int _p2x=0, int _p2y = 0);
    MapPath(const MapPath &p);
    void setPathType(Map_Path_Type _path_type){path_type=_path_type;}
    Map_Path_Type getPathType(){return path_type;}

    void setStart(int _start){start = _start;}
    int getStart(){return start;}
    void setEnd(int _end){end = _end;}
    int getEnd(){return end;}
    void setP1x(int _p1x){p1x = _p1x;}
    void setP1y(int _p1y){p1y = _p1y;}
    void setP2x(int _p2x){p2x = _p2x;}
    void setP2y(int _p2y){p2y = _p2y;}
    void setLength(int _length){length=_length;}

    int getP1x(){return p1x;}
    int getP1y(){return p1y;}
    int getP2x(){return p2x;}
    int getP2y(){return p2y;}
    int getLength(){return length;}

private:
    Map_Path_Type path_type;
    int start;
    int end;
    int p1x;
    int p2x;
    int p1y;
    int p2y;
    int length;
};

#endif // MAPPATH_H
