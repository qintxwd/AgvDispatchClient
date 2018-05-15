#include <assert.h>

#include "scene.h"
#include "stationinputdialog.h"
#include "global.h"
#include <QtWidgets>

Scene::Scene(OneMap *_onemap, MapFloor *_floor, QObject *parent) : QGraphicsScene(parent),
    cur_tool(T_NONE),
    onemap(_onemap),
    floor(_floor),
    oldSelectStation(nullptr),
    bkg(nullptr)
{
    build();
}

Scene::~Scene()
{
}

void Scene::drawBackground(QPainter *painter, const QRectF &rect)
{

    QPen oldpen = painter->pen();
    //    QBrush oldbrush = painter->brush();
    //添加原点
    QPen pen(Qt::blue);
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawEllipse(QPoint(0,0),5,5);

    //添加x轴
    QPen pen2(Qt::magenta);
    painter->setPen(pen2);

    painter->drawLine(QPoint(-100000,0),QPoint(100000,0));

    //添加y轴
    painter->drawLine(QPoint(0,-100000),QPoint(0,100000));
}


void Scene::build()
{
    //TODO:
    QList<MapPoint *> points = floor->getPoints();
    foreach (auto p, points) {
        MapItemStation *station = new MapItemStation(p);
        MapItemStationName *stationname = new MapItemStationName(station,p);
        connect(station,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
        connect(stationname,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
        addItem(station);
        addItem(stationname);
        iStations.append(station);
        iStationNames.append(stationname);
    }

    QList<MapPath *> path = floor->getPaths();
    foreach (auto p, path) {
        if(p->getPathType() == MapPath::Map_Path_Type_Line){
            MapItemStation *start = nullptr;
            MapItemStation *end = nullptr;

            foreach (auto s, iStations) {
                if(s->getPoint()->getId() == p->getStart()){start = s;}
                if(s->getPoint()->getId() == p->getEnd()){end = s;}
            }
            if(start == nullptr || end == nullptr)continue ;

            MapItemLine *l = new MapItemLine(start,end,p);
            connect(l,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
            addItem(l);
            iLines.push_back(l);
        }
        else if(p->getPathType() == MapPath::Map_Path_Type_Quadratic_Bezier){
            MapItemStation *start = nullptr;
            MapItemStation *end = nullptr;
            foreach (auto s, iStations) {
                if(s->getPoint()->getId() == p->getStart()){start = s;}
                if(s->getPoint()->getId() == p->getEnd()){end = s;}
            }
            if(start == nullptr || end == nullptr)return ;

            MapItemQuadraticBezier *l = new MapItemQuadraticBezier(start,end,p);
            connect(l,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
            addItem(l);
            iQbs.push_back(l);
        }
        else if(p->getPathType() == MapPath::Map_Path_Type_Cubic_Bezier){
            MapItemStation *start = nullptr;
            MapItemStation *end = nullptr;
            foreach (auto s, iStations) {
                if(s->getPoint()->getId() == p->getStart()){start = s;}
                if(s->getPoint()->getId() == p->getEnd()){end = s;}
            }
            if(start == nullptr || end == nullptr)return ;

            MapItemCubicBezier *l = new MapItemCubicBezier(start,end,p);
            connect(l,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
            addItem(l);
            iCbs.push_back(l);
        }
    }

    MapBackground *_bkg = floor->getBkg();
    if(_bkg != nullptr){
        bkg = new MapItemBkg(_bkg);
        connect(bkg,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
        addItem(bkg);
    }

    update();

    connect(this,SIGNAL(selectionChanged()),this,SLOT(onSelectItemChanged()));
}

void Scene::slot_setCurTool(int t)
{
    if(t>=T_NONE && t<=T_CB){
        if(cur_tool == t)return ;
        cur_tool = static_cast<Tool>(t);
        oldSelectStation = nullptr;
    }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if(cur_tool == T_STATION_DRAW
                || cur_tool ==T_STATION_REPORT
                || cur_tool ==T_STATION_HALT
                || cur_tool ==T_STATION_CHARGE
                || cur_tool ==T_STATION_LOAD
                || cur_tool ==T_STATION_UNLOAD
                || cur_tool ==T_STATION_LOAD_UNLOAD)
        {
            MapPoint::Map_Point_Type type;
            switch (cur_tool) {
            case T_STATION_DRAW:
                type = MapPoint::Map_Point_Type_Draw;
                break;
            case T_STATION_REPORT:
                type = MapPoint::Map_Point_Type_REPORT;
                break;
            case T_STATION_HALT:
                type = MapPoint::Map_Point_Type_HALT;
                break;
            case T_STATION_CHARGE:
                type = MapPoint::Map_Point_Type_CHARGE;
                break;
            case T_STATION_LOAD:
                type = MapPoint::Map_Point_Type_LOAD;
                break;
            case T_STATION_UNLOAD:
                type = MapPoint::Map_Point_Type_UNLOAD;
                break;
            case T_STATION_LOAD_UNLOAD:
                type = MapPoint::Map_Point_Type_LOAD_UNLOAD;
                break;
            default:
                type = MapPoint::Map_Point_Type_HALT;
                break;
            }
            //添加站点~
            int id = onemap->getNextId();
            QString name = QString("station %1").arg(id);
            MapPoint *p = new MapPoint(id,name,type,event->scenePos().x(),event->scenePos().y());
            floor->addPoint(p);

            //添加item
            MapItemStation *station= new MapItemStation(p);
            MapItemStationName *stationname = new MapItemStationName(station,p);
            connect(station,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
            connect(stationname,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
            addItem(station);
            addItem(stationname);
            iStations.push_back(station);
            iStationNames.push_back(stationname);
            update();

            //发射信号
            emit sig_add_remove_spirit();
        }
    }

    else if(event->button() == Qt::RightButton){
        slot_setCurTool(T_NONE);
        emit sig_cancelTool();
    }

    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit sig_currentMousePos(event->scenePos());
    QGraphicsScene::mouseMoveEvent(event);
}

//void Scene::setBackgroundImagePath(QString _path)
//{
//    QImage img(_path);
//    if(img.isNull())
//    {
//        QMessageBox::warning(nullptr,QStringLiteral("错误"),QStringLiteral("非图像文件或不存在"));
//        return ;
//    }

//    QString fileNameWithType = "image.jpg";
//    if(_path.contains("\\"))
//        fileNameWithType = _path.right(_path.length() - _path.lastIndexOf("\\"));
//    else if(_path.contains("/"))
//        fileNameWithType = _path.right(_path.length() - _path.lastIndexOf("/")-1);
//    QString fileName = fileNameWithType.left(fileNameWithType.indexOf("."));

//    MapBackground *bk = new MapBackground(onemap->getNextId(),fileName,img,fileName);

//    if(bkg!=nullptr && bkg->getBkg()!=nullptr){
//        //删除原来的元素
//        removeItem(bkg);
//        bkg = nullptr;
//    }

//    if(bkg==nullptr){
//        bkg = new MapItemBkg(bk);
//        connect(bkg,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
//        addItem(bkg);
//    }else{
//        bkg->setBkg(bk);
//        bkg->update(bkg->boundingRect());
//    }
//    floor->setBkg(bk);
//    emit sig_add_remove_spirit();
//    update();
//}

void Scene::addSpirit(MapFloor *_floor,MapSpirit *_spirit)
{
    if(_floor != floor){
        return ;
    }
    if(MapSpirit::Map_Sprite_Type_Point == _spirit->getSpiritType())
    {
        MapPoint *p = static_cast<MapPoint *>(_spirit);
        MapItemStation *sstation = new MapItemStation(p);
        MapItemStationName *sstationName = new MapItemStationName(sstation,p);
        connect(sstation,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
        connect(sstationName,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
        addItem(sstation);
        addItem(sstationName);
        iStations.push_back(sstation);
        iStationNames.append(sstationName);
        update();
    }else if(MapSpirit::Map_Sprite_Type_Path == _spirit->getSpiritType())
    {
        MapPath *p = static_cast<MapPath *>(_spirit);
        if(p->getPathType() == MapPath::Map_Path_Type_Line){
            MapItemStation *start = nullptr;
            MapItemStation *end = nullptr;

            foreach (auto s, iStations) {
                if(s->getPoint()->getId() == p->getStart()){start = s;}
                if(s->getPoint()->getId() == p->getEnd()){end = s;}
            }
            if(start == nullptr || end == nullptr)return ;

            MapItemLine *l = new MapItemLine(start,end,p);
            connect(l,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
            addItem(l);
            iLines.push_back(l);
            update();
        }
        else if(p->getPathType() == MapPath::Map_Path_Type_Quadratic_Bezier){
            MapItemStation *start = nullptr;
            MapItemStation *end = nullptr;
            foreach (auto s, iStations) {
                if(s->getPoint()->getId() == p->getStart()){start = s;}
                if(s->getPoint()->getId() == p->getEnd()){end = s;}
            }
            if(start == nullptr || end == nullptr)return ;

            MapItemQuadraticBezier *l = new MapItemQuadraticBezier(start,end,p);
            connect(l,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
            addItem(l);
            iQbs.push_back(l);
            update();
        }
        else if(p->getPathType() == MapPath::Map_Path_Type_Cubic_Bezier){
            MapItemStation *start = nullptr;
            MapItemStation *end = nullptr;
            foreach (auto s, iStations) {
                if(s->getPoint()->getId() == p->getStart()){start = s;}
                if(s->getPoint()->getId() == p->getEnd()){end = s;}
            }
            if(start == nullptr || end == nullptr)return ;

            MapItemCubicBezier *l = new MapItemCubicBezier(start,end,p);
            connect(l,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
            addItem(l);
            iCbs.push_back(l);
            update();
        }
    }
}

void Scene::propertyChanged(MapSpirit *_spirit)
{
    if(_spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Point){
        MapPoint *p = static_cast<MapPoint *>(_spirit);
        foreach (auto station, iStations) {
            if(station->getPoint() == p){
                station->my_update();
                break;
            }
        }
        foreach (auto name , iStationNames) {
            if(name->getPoint() == p){
                name->my_update();
                break;
            }
        }
    }
    else if(_spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Path){
        MapPath *path = static_cast<MapPath *>(_spirit);
        bool lineTypeChange = true;
        if(path->getPathType() == MapPath::Map_Path_Type_Line){
            foreach (auto l, iLines) {
                if(l->getPath() == path){
                    lineTypeChange = false;
                    l->my_update();
                    break;
                }
            }
        }
        else  if(path->getPathType() == MapPath::Map_Path_Type_Quadratic_Bezier){
            foreach (auto l, iQbs) {
                if(l->getPath() == path){
                    lineTypeChange = false;
                    l->my_update();
                    break;
                }
            }
        }
        else  if(path->getPathType() == MapPath::Map_Path_Type_Cubic_Bezier){
            foreach (auto l, iCbs) {
                if(l->getPath() == path){
                    lineTypeChange = false;
                    l->my_update();
                    break;
                }
            }
        }
        qDebug()<<"lineTypeChange = "<<lineTypeChange;
        if(lineTypeChange){
            //TODO:
            //刪除item
            foreach (auto l, iLines) {
                if(l->getPath() == path){
                    removeItem(l);
                    break;
                }
            }
            foreach (auto l, iQbs) {
                if(l->getPath() == path){
                    removeItem(l);
                    break;
                }
            }
            foreach (auto l, iCbs) {
                if(l->getPath() == path){
                    removeItem(l);
                    break;
                }
            }

            //添加新的item
            if(path->getPathType() == MapPath::Map_Path_Type_Line){
                MapItemStation *start = nullptr;
                MapItemStation *end = nullptr;

                foreach (auto s, iStations) {
                    if(s->getPoint()->getId() == path->getStart()){start = s;}
                    if(s->getPoint()->getId() == path->getEnd()){end = s;}
                }
                if(start == nullptr || end == nullptr)return ;

                MapItemLine *l = new MapItemLine(start,end,path);
                connect(l,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
                addItem(l);
                iLines.push_back(l);
                update();
            }else if(path->getPathType() == MapPath::Map_Path_Type_Quadratic_Bezier){
                MapItemStation *start = nullptr;
                MapItemStation *end = nullptr;
                foreach (auto s, iStations) {
                    if(s->getPoint()->getId() == path->getStart()){start = s;}
                    if(s->getPoint()->getId() == path->getEnd()){end = s;}
                }
                if(start == nullptr || end == nullptr)return ;

                MapItemQuadraticBezier *l = new MapItemQuadraticBezier(start,end,path);
                connect(l,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
                addItem(l);
                iQbs.push_back(l);
                update();
            }else if(path->getPathType() == MapPath::Map_Path_Type_Cubic_Bezier){
                MapItemStation *start = nullptr;
                MapItemStation *end = nullptr;
                foreach (auto s, iStations) {
                    if(s->getPoint()->getId() == path->getStart()){start = s;}
                    if(s->getPoint()->getId() == path->getEnd()){end = s;}
                }
                if(start == nullptr || end == nullptr)return ;

                MapItemCubicBezier *l = new MapItemCubicBezier(start,end,path);
                connect(l,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
                addItem(l);
                iCbs.push_back(l);
                update();
            }
        }
    }
    else if(_spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Background){
        MapBackground * b = static_cast<MapBackground *>(_spirit);
        if(bkg!=nullptr && bkg->getBkg() == b){
            bkg->my_update();
        }
    }
    else if(_spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Floor){
        //add bkg!
        MapFloor *_floor = static_cast<MapFloor *>(_spirit);
        if(_floor == this->floor){
            if(floor->getBkg()!=nullptr){
                if(bkg!=nullptr){
                    removeItem(bkg);
                    bkg = nullptr;
                    update();
                }
                bkg = new MapItemBkg(floor->getBkg());
                addItem(bkg);
            }
        }
    }
}

void Scene::onSelectItemChanged()
{
    QList<QGraphicsItem *> sis= this->selectedItems();

    if(sis.length() > 2){
        emit sig_chooseChanged(nullptr);
    }

    if(sis.length() == 1)
    {
        QGraphicsItem *lastSelectItem = sis.last();

        //选择了一个站点
        if(lastSelectItem->type() == MapItemStation::Type || lastSelectItem->type() == MapItemStationName::Type)
        {
            MapItemStation *newStation = nullptr;
            if(lastSelectItem->type() == MapItemStation::Type){
                newStation = qgraphicsitem_cast<MapItemStation*>(lastSelectItem);

                foreach (auto name, iStationNames) {
                    if(name->getStation() == newStation){
                        name->setSelected(true);
                        break;
                    }
                }

            }else{
                MapItemStationName *stationName = qgraphicsitem_cast<MapItemStationName *>(lastSelectItem);
                newStation = stationName->getStation();
            }

            if(cur_tool == T_LINE){
                if(oldSelectStation==nullptr){
                    oldSelectStation = newStation;
                }else if(oldSelectStation!=newStation){

                    //判断是否已经有线路了
                    foreach (auto l, iLines) {
                        if((l->getPath()->getStart() == oldSelectStation->getPoint()->getId()
                            && l->getPath()->getEnd() == newStation->getPoint()->getId())||
                                (l->getPath()->getEnd() == oldSelectStation->getPoint()->getId()
                                 && l->getPath()->getStart() == newStation->getPoint()->getId()))
                        {
                            //两个站点之间已经有线路了
                            oldSelectStation = nullptr;
                            return ;
                        }
                    }
                    foreach (auto l, iQbs) {
                        if((l->getPath()->getStart() == oldSelectStation->getPoint()->getId()
                            && l->getPath()->getEnd() == newStation->getPoint()->getId())||
                                (l->getPath()->getEnd() == oldSelectStation->getPoint()->getId()
                                 && l->getPath()->getStart() == newStation->getPoint()->getId()))
                        {
                            //两个站点之间已经有线路了
                            oldSelectStation = nullptr;
                            return ;
                        }
                    }
                    foreach (auto l, iCbs) {
                        if((l->getPath()->getStart() == oldSelectStation->getPoint()->getId()
                            && l->getPath()->getEnd() == newStation->getPoint()->getId())||
                                (l->getPath()->getEnd() == oldSelectStation->getPoint()->getId()
                                 && l->getPath()->getStart() == newStation->getPoint()->getId()))
                        {
                            //两个站点之间已经有线路了
                            oldSelectStation = nullptr;
                            return ;
                        }
                    }


                    QString lineName = QString("%1 -- %2").arg(oldSelectStation->getPoint()->getName()).arg(newStation->getPoint()->getName());

                    //对onemap添加线路数据
                    MapPath *p = new MapPath(onemap->getNextId(),lineName,oldSelectStation->getPoint()->getId(),newStation->getPoint()->getId(),MapPath::Map_Path_Type_Line,1);
                    floor->addPath(p);

                    //添加item
                    MapItemLine *line= new MapItemLine(oldSelectStation,newStation,p);
                    oldSelectStation->addLine(line);
                    newStation->addLine(line);
                    connect(line,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
                    addItem(line);
                    iLines.push_back(line);
                    update();

                    //发射信号
                    emit sig_add_remove_spirit();
                    newStation->setSelected(false);
                    oldSelectStation = nullptr;
                }
            }else if(cur_tool == T_QB){
                if(oldSelectStation==nullptr){
                    oldSelectStation = newStation;
                }else if(oldSelectStation!=newStation){
                    //判断是否已经有线路了
                    foreach (auto l, iLines) {
                        if((l->getPath()->getStart() == oldSelectStation->getPoint()->getId()
                            && l->getPath()->getEnd() == newStation->getPoint()->getId())||
                                (l->getPath()->getEnd() == oldSelectStation->getPoint()->getId()
                                 && l->getPath()->getStart() == newStation->getPoint()->getId()))
                        {
                            //两个站点之间已经有线路了
                            oldSelectStation = nullptr;
                            return ;
                        }
                    }
                    foreach (auto l, iQbs) {
                        if((l->getPath()->getStart() == oldSelectStation->getPoint()->getId()
                            && l->getPath()->getEnd() == newStation->getPoint()->getId())||
                                (l->getPath()->getEnd() == oldSelectStation->getPoint()->getId()
                                 && l->getPath()->getStart() == newStation->getPoint()->getId()))
                        {
                            //两个站点之间已经有线路了
                            oldSelectStation = nullptr;
                            return ;
                        }
                    }
                    foreach (auto l, iCbs) {
                        if((l->getPath()->getStart() == oldSelectStation->getPoint()->getId()
                            && l->getPath()->getEnd() == newStation->getPoint()->getId())||
                                (l->getPath()->getEnd() == oldSelectStation->getPoint()->getId()
                                 && l->getPath()->getStart() == newStation->getPoint()->getId()))
                        {
                            //两个站点之间已经有线路了
                            oldSelectStation = nullptr;
                            return ;
                        }
                    }
                    QString qbName = QString("%1 -- %2").arg(oldSelectStation->getPoint()->getName()).arg(newStation->getPoint()->getName());
                    int cx = (oldSelectStation->getPoint()->getX()+newStation->getPoint()->getX())/2;
                    int cy = (oldSelectStation->getPoint()->getY()+newStation->getPoint()->getY())/2;
                    //对onemap添加线路数据
                    MapPath *p = new MapPath(onemap->getNextId(),qbName,oldSelectStation->getPoint()->getId(),newStation->getPoint()->getId(),MapPath::Map_Path_Type_Quadratic_Bezier,1,cx,cy);
                    floor->addPath(p);

                    //添加item
                    MapItemQuadraticBezier *qb= new MapItemQuadraticBezier(oldSelectStation,newStation,p);
                    oldSelectStation->addQb(qb);
                    newStation->addQb(qb);
                    connect(qb,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
                    addItem(qb);
                    iQbs.push_back(qb);
                    update();

                    //发射信号
                    emit sig_add_remove_spirit();
                    newStation->setSelected(false);
                    oldSelectStation = nullptr;
                }
            }else if(cur_tool == T_CB){
                if(oldSelectStation==nullptr){
                    oldSelectStation = newStation;
                }else if(oldSelectStation!=newStation){
                    //判断是否已经有线路了
                    foreach (auto l, iLines) {
                        if((l->getPath()->getStart() == oldSelectStation->getPoint()->getId()
                            && l->getPath()->getEnd() == newStation->getPoint()->getId())||
                                (l->getPath()->getEnd() == oldSelectStation->getPoint()->getId()
                                 && l->getPath()->getStart() == newStation->getPoint()->getId()))
                        {
                            //两个站点之间已经有线路了
                            oldSelectStation = nullptr;
                            return ;
                        }
                    }
                    foreach (auto l, iQbs) {
                        if((l->getPath()->getStart() == oldSelectStation->getPoint()->getId()
                            && l->getPath()->getEnd() == newStation->getPoint()->getId())||
                                (l->getPath()->getEnd() == oldSelectStation->getPoint()->getId()
                                 && l->getPath()->getStart() == newStation->getPoint()->getId()))
                        {
                            //两个站点之间已经有线路了
                            oldSelectStation = nullptr;
                            return ;
                        }
                    }
                    foreach (auto l, iCbs) {
                        if((l->getPath()->getStart() == oldSelectStation->getPoint()->getId()
                            && l->getPath()->getEnd() == newStation->getPoint()->getId())||
                                (l->getPath()->getEnd() == oldSelectStation->getPoint()->getId()
                                 && l->getPath()->getStart() == newStation->getPoint()->getId()))
                        {
                            //两个站点之间已经有线路了
                            oldSelectStation = nullptr;
                            return ;
                        }
                    }

                    QString qbName = QString("%1 -- %2").arg(oldSelectStation->getPoint()->getName()).arg(newStation->getPoint()->getName());
                    int cx1 = oldSelectStation->getPoint()->getX()+(newStation->getPoint()->getX() - oldSelectStation->getPoint()->getX())/3;
                    int cy1 = oldSelectStation->getPoint()->getY()+(newStation->getPoint()->getY() - oldSelectStation->getPoint()->getY())/3;

                    int cx2 = oldSelectStation->getPoint()->getX()+(newStation->getPoint()->getX() - oldSelectStation->getPoint()->getX())*2/3;
                    int cy2 = oldSelectStation->getPoint()->getY()+(newStation->getPoint()->getY() - oldSelectStation->getPoint()->getY())*2/3;

                    //对onemap添加线路数据
                    MapPath *p = new MapPath(onemap->getNextId(),qbName,oldSelectStation->getPoint()->getId(),newStation->getPoint()->getId(),MapPath::Map_Path_Type_Cubic_Bezier,1,cx1,cy1,cx2,cy2);
                    floor->addPath(p);

                    //添加item
                    MapItemCubicBezier *cb= new MapItemCubicBezier(oldSelectStation,newStation,p);
                    oldSelectStation->addCb(cb);
                    newStation->addCb(cb);
                    connect(cb,SIGNAL(sig_propertyChanged(MapSpirit*)),this,SIGNAL(sig_propertyChanged(MapSpirit*)));
                    addItem(cb);
                    iCbs.push_back(cb);
                    update();

                    //发射信号
                    emit sig_add_remove_spirit();
                    newStation->setSelected(false);
                    oldSelectStation = nullptr;
                }
            }else if(cur_tool == T_ERASER ){
                //清除一个站点//TODO
                //要先清除，所有的相关线路
                QList<MapItemLine *> lines = newStation->getLines();
                foreach (auto line, lines) {
                    removeItem(line);
                    iLines.removeAll(line);
                    floor->removePath(line->getPath());
                    update();

                }

                QList<MapItemCubicBezier *> cbs = newStation->getCbs();
                foreach (auto cb, cbs) {
                    removeItem(cb);
                    iCbs.removeAll(cb);
                    floor->removePath(cb->getPath());
                    update();
                }

                QList<MapItemQuadraticBezier *> qbs = newStation->getQbs();
                foreach (auto qb, qbs) {
                    removeItem(qb);
                    iQbs.removeAll(qb);
                    floor->removePath(qb->getPath());
                    update();
                }

                //然后清除站点
                MapItemStationName *stationname = nullptr;
                foreach (auto sn, iStationNames) {
                    if(sn->getStation() == newStation){
                        stationname = sn;
                        break;
                    }
                }
                assert(stationname != nullptr);
                removeItem(stationname);
                removeItem(newStation);
                iStations.removeAll(newStation);
                iStationNames.removeAll(stationname);
                floor->removePoint(newStation->getPoint());
                update();
                emit sig_add_remove_spirit();
            }else{
                emit sig_chooseChanged(newStation->getPoint());
            }
        }

        //选择了一个线路
        else if(lastSelectItem->type() == MapItemLine::Type)
        {
            MapItemLine *selectLine = qgraphicsitem_cast<MapItemLine*>(lastSelectItem);
            if(cur_tool == T_ERASER ){
                //从它两头的站点中，将它移出
                selectLine->getStartStation()->removeLine(selectLine);
                selectLine->getEndStation()->removeLine(selectLine);
                removeItem(selectLine);
                iLines.removeAll(selectLine);
                floor->removePath(selectLine->getPath());
                update();
                //TODO
                emit sig_add_remove_spirit();
            }else{
                emit sig_chooseChanged(selectLine->getPath());
            }
        }

        //选择了一条曲线
        else if(lastSelectItem->type() == MapItemQuadraticBezier::Type)
        {
            MapItemQuadraticBezier *selectLine = qgraphicsitem_cast<MapItemQuadraticBezier*>(lastSelectItem);
            if(cur_tool == T_ERASER ){
                //从它两头的站点中，将它移出
                selectLine->getStartStation()->removeQb(selectLine);
                selectLine->getEndStation()->removeQb(selectLine);
                removeItem(selectLine);
                iQbs.removeAll(selectLine);
                floor->removePath(selectLine->getPath());
                update();
                //TODO:
                emit sig_add_remove_spirit();
            }else{
                emit sig_chooseChanged(selectLine->getPath());
            }
        }

        //选择了一条曲线
        else if(lastSelectItem->type() == MapItemCubicBezier::Type)
        {
            MapItemCubicBezier *selectLine = qgraphicsitem_cast<MapItemCubicBezier*>(lastSelectItem);
            if(cur_tool == T_ERASER ){
                //从它两头的站点中，将它移出
                selectLine->getStartStation()->removeCb(selectLine);
                selectLine->getEndStation()->removeCb(selectLine);
                removeItem(selectLine);
                iCbs.removeAll(selectLine);
                floor->removePath(selectLine->getPath());
                update();
                //TODO:
                emit sig_add_remove_spirit();
            }else{
                emit sig_chooseChanged(selectLine->getPath());
            }
        }

        //选择了背景图片
        else if(lastSelectItem->type() == MapItemBkg::Type)
        {
            MapItemBkg *sekectBkg = qgraphicsitem_cast<MapItemBkg*>(lastSelectItem);
            if(cur_tool == T_ERASER ){
                removeItem(sekectBkg);
                floor->removeBkg();
                update();
                //TODO:
                emit sig_add_remove_spirit();
            }else{
                emit sig_chooseChanged(sekectBkg->getBkg());
            }
        }
    }
}


void Scene::slot_selectItem(MapSpirit *_spirit)
{
    clearSelection();
    if(_spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Point)
    {
        foreach (auto staion, iStations) {
            if(staion->getPoint() == _spirit){
                staion->setSelected(true);;
                break;
            }
        }
    }
    else if(_spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Path)
    {
        MapPath *p = static_cast<MapPath *>(_spirit);

        if(p->getPathType() == MapPath::Map_Path_Type_Line){
            foreach (auto p, iLines) {
                if(p->getPath() == _spirit){
                    p->setSelected(true);
                    break;
                }
            }
        }else if(p->getPathType() == MapPath::Map_Path_Type_Quadratic_Bezier){
            foreach (auto p, iQbs) {
                if(p->getPath() == _spirit){
                    p->setSelected(true);
                    break;
                }
            }
        }else if(p->getPathType() == MapPath::Map_Path_Type_Cubic_Bezier){
            foreach (auto p, iCbs) {
                if(p->getPath() == _spirit){
                    p->setSelected(true);
                    break;
                }
            }
        }

    }
    else if(_spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Background){
        if(bkg!=nullptr && bkg->getBkg() == _spirit){
            bkg->setSelected(true);
        }
    }
}

