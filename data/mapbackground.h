#ifndef MAPBACKGROUND_H
#define MAPBACKGROUND_H

#include "mapspirit.h"

class MapBackground : public MapSpirit
{
public:
    MapBackground(int _id, QString _name, int _x, int _y,int _width,int _height);

    void setX(int _x){x=_x;}
    void setY(int _y){y=_y;}
    void setWidth(int _width){width=_width;}
    void setHeight(int _height){height=_height;}

    void getX(){return x;}
    void getY(){return y;}
    void getWidth(){return width;}
    void getHeight(){return height;}

private:
    QByteArray background;
    QString imgFileName;
    int x;
    int y;
    int width;
    int height;
};

#endif // MAPBACKGROUND_H
