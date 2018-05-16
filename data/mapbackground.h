#ifndef MAPBACKGROUND_H
#define MAPBACKGROUND_H
#include <QImage>
#include "mapspirit.h"

class MapBackground : public MapSpirit
{
    Q_OBJECT
public:
    MapBackground(int _id, std::string _name, QImage _img, std::string fileName,QObject *parent = nullptr);
    MapBackground(const MapBackground &b);//深拷贝

    void setX(int _x){x=_x;}
    void setY(int _y){y=_y;}
    void setWidth(int _width){width=_width;}
    void setHeight(int _height){height=_height;}

    int getX(){return x;}
    int getY(){return y;}
    int getWidth(){return width;}
    int getHeight(){return height;}

    std::string getFileName(){return imgFileName;}
    QImage getImg(){return img;}

    void setFileName(std::string _filename){imgFileName = _filename;}
    void setImg(QImage _img){img=_img;}
private:
    QImage img;
    std::string imgFileName;
    int x;
    int y;
    int width;
    int height;
};

#endif // MAPBACKGROUND_H
