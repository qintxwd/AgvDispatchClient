#include "mapbackground.h"

MapBackground::MapBackground(int _id, QString _name, QImage _img, QString fileName, QObject *parent):
    MapSpirit(_id,_name,Map_Sprite_Type_Background,parent),
    x(0),
    y(0),
    width(_img.width()),
    height(_img.height()),
    img(_img),
    imgFileName(fileName)
{
}

MapBackground::MapBackground(const MapBackground &b):
    MapSpirit(b),
    x(b.x),
    y(b.y),
    img(b.img),
    imgFileName(b.imgFileName),
    width(b.width),
    height(b.height)
{
}
