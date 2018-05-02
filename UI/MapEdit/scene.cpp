#include <assert.h>

#include "scene.h"
#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneMouseEvent>
#include "stationinputdialog.h"
#include "global.h"

Scene::Scene(OneMap *_onemap, MapFloor *_floor, QObject *parent) : QGraphicsScene(parent),
    cur_tool(T_NONE),
    onemap(_onemap),
    floor(_floor),
    oldSelectStation(nullptr)
{
    setItemIndexMethod(BspTreeIndex);
    build();
}

Scene::~Scene()
{
}


void Scene::build()
{
    clear();

    //TODO:
    QList<MapPoint *> points = floor->getPoints();
    foreach (auto p, points) {
        MapItemStation *station = new MapItemStation(p);
        MapItemStationName *stationname = new MapItemStationName(station,p);
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
            addItem(l);
            iCbs.push_back(l);
        }
    }

    MapBackground *_bkg = floor->getBkg();
    if(_bkg != nullptr){
        bkg = new MapItemBkg(_bkg);
        addItem(bkg);
    }

    update();

    connect(this,SIGNAL(selectionChanged()),this,SLOT(onSelectItemChanged()));
}


void Scene::setCurTool(Tool t)
{
    if(cur_tool == t)return ;
    cur_tool = t;
    oldSelectStation = nullptr;
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
            addItem(station);
            addItem(stationname);
            iStations.push_back(station);
            iStationNames.push_back(stationname);
            update();

            //发射信号
            emit sig_addStation(p);
        }
    }

    else if(event->button() == Qt::RightButton){
        setCurTool(T_NONE);
        emit cancelTool();
    }

    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    positoinlabel->setPos(event->scenePos().x()+10,event->scenePos().y()-10);
//    positoinlabel->positionChanged(event->scenePos());
    QGraphicsScene::mouseMoveEvent(event);
}

void Scene::setBackgroundImagePath(QString _path)
{
    QImage img(_path);
    if(img.isNull())
    {
        QMessageBox::warning(nullptr,QStringLiteral("错误"),QStringLiteral("非图像文件或不存在"));
        return ;
    }

    QString fileNameWithType = "image.jpg";
    if(_path.contains("\\"))
        fileNameWithType = _path.right(_path.length() - _path.lastIndexOf("\\"));
    else if(_path.contains("/"))
        fileNameWithType = _path.right(_path.length() - _path.lastIndexOf("/")-1);
    QString fileName = fileNameWithType.left(fileNameWithType.indexOf("."));

    MapBackground *bk = new MapBackground(onemap->getNextId(),fileName,img,fileName);

    if(bkg!=nullptr && bkg->getBkg()!=nullptr){
        //删除原来的元素
        MapBackground *bk = bkg->getBkg();
        emit sig_removeBkg(bk);
    }

    if(bkg==nullptr){
        bkg = new MapItemBkg(bk);
        addItem(bkg);
    }else{
        bkg->setBkg(bk);
        bkg->update(bkg->boundingRect());
    }
    update();
}

void Scene::toolChanged()
{
    //setCurTool(tool);
}

void Scene::addSpirit(MapFloor *_floor,MapSpirit *_spirit)
{
    if(_floor != floor){
        return ;
    }
    if(MapSpirit::Map_Sprite_Type_Point == _spirit->getSpiritType())
    {
        MapPoint *p = dynamic_cast<MapPoint *>(_spirit);
        MapItemStation *sstation = new MapItemStation(p);
        MapItemStationName *sstationName = new MapItemStationName(sstation,p);
        addItem(sstation);
        addItem(sstationName);
        iStations.push_back(sstation);
        iStationNames.append(sstationName);
        update();
    }else if(MapSpirit::Map_Sprite_Type_Path == _spirit->getSpiritType())
    {
        MapPath *p = dynamic_cast<MapPath *>(_spirit);
        if(p->getPathType() == MapPath::Map_Path_Type_Line){
            MapItemStation *start = nullptr;
            MapItemStation *end = nullptr;

            foreach (auto s, iStations) {
                if(s->getPoint()->getId() == p->getStart()){start = s;}
                if(s->getPoint()->getId() == p->getEnd()){end = s;}
            }
            if(start == nullptr || end == nullptr)return ;

            MapItemLine *l = new MapItemLine(start,end,p);
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
            addItem(l);
            iCbs.push_back(l);
            update();
        }
    }
}

void Scene::removeSpirit(MapFloor *_floor, MapSpirit *_spirit)
{
    if(_floor != floor){
        return ;
    }

    if(MapSpirit::Map_Sprite_Type_Point == _spirit->getSpiritType())
    {
        MapPoint *p = dynamic_cast<MapPoint *>(_spirit);

        MapItemStation *newStation = nullptr;
        foreach (auto station, iStations) {
            if(station->getPoint() == p){
                newStation = station;
                break;
            }
        }
        if(newStation == nullptr)return ;

        //清除一个站点//TODO
        //要先清除，所有的相关线路
        QList<MapItemLine *> lines = newStation->getLines();
        foreach (auto line, lines) {
            removeItem(line);
            iLines.removeAll(line);
            update();
            emit sig_removePath(line->getPath());
        }

        QList<MapItemCubicBezier *> cbs = newStation->getCbs();
        foreach (auto cb, cbs) {
            removeItem(cb);
            iCbs.removeAll(cb);
            update();
            emit sig_removePath(cb->getPath());
        }

        QList<MapItemQuadraticBezier *> qbs = newStation->getQbs();
        foreach (auto qb, qbs) {
            removeItem(qb);
            iQbs.removeAll(qb);
            update();
            emit sig_removePath(qb->getPath());
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
        update();
        emit sig_removeStation(newStation->getPoint());

    }else if(MapSpirit::Map_Sprite_Type_Path == _spirit->getSpiritType())
    {
        MapPath *p = dynamic_cast<MapPath *>(_spirit);
        if(p->getPathType() == MapPath::Map_Path_Type_Line){
            MapItemStation *start = nullptr;
            MapItemStation *end = nullptr;

            foreach (auto s, iStations) {
                if(s->getPoint()->getId() == p->getStart()){start = s;}
                if(s->getPoint()->getId() == p->getEnd()){end = s;}
            }
            if(start == nullptr || end == nullptr)return ;

            MapItemLine *l = new MapItemLine(start,end,p);
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
            addItem(l);
            iCbs.push_back(l);
            update();
        }
    }
}

void Scene::propertyChanged(MapFloor *_floor, MapSpirit *_spirit)
{
    if(_floor != floor)return ;
    if(_spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Point){
        MapPoint *p = dynamic_cast<MapPoint *>(_spirit);
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
        MapPath *path = dynamic_cast<MapPath *>(_spirit);
        if(path->getPathType() == MapPath::Map_Path_Type_Line){
            foreach (auto l, iLines) {
                if(l->getPath() == path){
                    l->my_update();
                    break;
                }
            }
        }
        else  if(path->getPathType() == MapPath::Map_Path_Type_Quadratic_Bezier){
            foreach (auto l, iQbs) {
                if(l->getPath() == path){
                    l->my_update();
                    break;
                }
            }
        }
        else  if(path->getPathType() == MapPath::Map_Path_Type_Cubic_Bezier){
            foreach (auto l, iCbs) {
                if(l->getPath() == path){
                    l->my_update();
                    break;
                }
            }
        }
    }
    else if(_spirit->getSpiritType() == MapSpirit::Map_Sprite_Type_Background){
        MapBackground * b = dynamic_cast<MapBackground *>(_spirit);
        if(bkg!=nullptr && bkg->getBkg() == b){
            bkg->my_update();
        }
    }
}

void Scene::onSelectItemChanged()
{
    QList<QGraphicsItem *> sis= this->selectedItems();

    if(sis.length() != 1){
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
            }else{
                MapItemStationName *stationName = qgraphicsitem_cast<MapItemStationName *>(lastSelectItem);
                newStation = stationName->getStation();
            }

            if(cur_tool == T_LINE){
                if(oldSelectStation==nullptr){
                    oldSelectStation = newStation;
                }else if(oldSelectStation!=newStation){
                    QString lineName = QString("%1 -- %2").arg(oldSelectStation->getPoint()->getName()).arg(newStation->getPoint()->getName());

                    //对onemap添加线路数据
                    MapPath *p = new MapPath(onemap->getNextId(),lineName,oldSelectStation->getPoint()->getId(),newStation->getPoint()->getId(),MapPath::Map_Path_Type_Line,1);
                    floor->addPath(p);

                    //添加item
                    MapItemLine *line= new MapItemLine(oldSelectStation,newStation,p);
                    oldSelectStation->addLine(line);
                    newStation->addLine(line);
                    addItem(line);
                    iLines.push_back(line);
                    update();

                    //发射信号
                    emit sig_addPath(line->getPath());
                }
            }else if(cur_tool == T_QB){
                if(oldSelectStation==nullptr){
                    oldSelectStation = newStation;
                }else if(oldSelectStation!=newStation){
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
                    addItem(qb);
                    iQbs.push_back(qb);
                    update();

                    //发射信号
                    emit sig_addPath(qb->getPath());
                }
            }else if(cur_tool == T_CB){
                if(oldSelectStation==nullptr){
                    oldSelectStation = newStation;
                }else if(oldSelectStation!=newStation){
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
                    addItem(cb);
                    iCbs.push_back(cb);
                    update();

                    //发射信号
                    emit sig_addPath(cb->getPath());
                }
            }else if(cur_tool == T_ERASER ){
                //清除一个站点//TODO
                //要先清除，所有的相关线路
                QList<MapItemLine *> lines = newStation->getLines();
                foreach (auto line, lines) {
                    removeItem(line);
                    iLines.removeAll(line);
                    update();
                    emit sig_removePath(line->getPath());
                }

                QList<MapItemCubicBezier *> cbs = newStation->getCbs();
                foreach (auto cb, cbs) {
                    removeItem(cb);
                    iCbs.removeAll(cb);
                    update();
                    emit sig_removePath(cb->getPath());
                }

                QList<MapItemQuadraticBezier *> qbs = newStation->getQbs();
                foreach (auto qb, qbs) {
                    removeItem(qb);
                    iQbs.removeAll(qb);
                    update();
                    emit sig_removePath(qb->getPath());
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
                update();
                emit sig_removeStation(newStation->getPoint());
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
                update();
                //TODO:
                emit sig_removePath(selectLine->getPath());
            }
        }
        else if(lastSelectItem->type() == MapItemQuadraticBezier::Type)
        {
            MapItemQuadraticBezier *selectLine = qgraphicsitem_cast<MapItemQuadraticBezier*>(lastSelectItem);
            if(cur_tool == T_ERASER ){
                //从它两头的站点中，将它移出
                selectLine->getStartStation()->removeQb(selectLine);
                selectLine->getEndStation()->removeQb(selectLine);
                removeItem(selectLine);
                iQbs.removeAll(selectLine);
                update();
                //TODO:
                emit sig_removePath(selectLine->getPath());
            }
        }
        else if(lastSelectItem->type() == MapItemCubicBezier::Type)
        {
            MapItemCubicBezier *selectLine = qgraphicsitem_cast<MapItemCubicBezier*>(lastSelectItem);
            if(cur_tool == T_ERASER ){
                //从它两头的站点中，将它移出
                selectLine->getStartStation()->removeCb(selectLine);
                selectLine->getEndStation()->removeCb(selectLine);
                removeItem(selectLine);
                iCbs.removeAll(selectLine);
                update();
                //TODO:
                emit sig_removePath(selectLine->getPath());
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
        MapPath *p = dynamic_cast<MapPath *>(_spirit);

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
        if(bkg->getBkg() == _spirit){
            bkg->setSelected(true);
        }
    }
}
