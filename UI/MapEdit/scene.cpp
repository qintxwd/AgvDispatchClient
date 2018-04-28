#include "scene.h"
#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneMouseEvent>
#include "stationinputdialog.h"
#include "global.h"
Scene::Scene(QObject *parent) : QGraphicsScene(parent),
    cur_tool(T_NONE),_width(0), _height(0),
    backgroundImagePath(""),
    itemBackground(NULL),
    oldSelectStation(NULL)
{
    setItemIndexMethod(BspTreeIndex);
    connect(&msgCenter,SIGNAL(mapLinesSuccess()),this,SLOT(onLoadLineSuccess()));
    connect(&msgCenter,SIGNAL(mapStationsSuccess()),this,SLOT(onLoadStationSuccess()));
    build();
}

Scene::~Scene()
{
}


void Scene::build(int width, int height)
{
    clear();
    if (width == -1 || height == -1){
        _width = 128;
        _height = 72;
    }
    //setSceneRect(-40, -40, _width * GRID_SIZE + 80, _height * GRID_SIZE + 80);

    //drawAxis();

//    positoinlabel = new ScenePositionLabel;
//    addItem(positoinlabel);
//    positoinlabel->setPos(200,200);

    connect(this,SIGNAL(selectionChanged()),this,SLOT(onSelectItemChanged()));
}


void Scene::drawLimitLine()
{
    QPointF ur = QPointF(-3, -3);
    QPointF ul = QPointF(GRID_SIZE * _width+3, -3);
    QPointF dr = QPointF(-3, GRID_SIZE * _height+3);
    QPointF dl = QPointF(GRID_SIZE * _width+3, GRID_SIZE * _height+3);

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setBrush(Qt::black);
    pen.setWidth(2);
    pen.setJoinStyle(Qt::RoundJoin);

    QLineF uline = QLineF(ul, ur);
    QLineF rline = QLineF(ur, dr);
    QLineF dline = QLineF(dr, dl);
    QLineF lline = QLineF(dl, ul);
    this->addLine(uline, pen);
    this->addLine(rline, pen);
    this->addLine(dline, pen);
    this->addLine(lline, pen);

    QGraphicsSimpleTextItem *text =  this->addSimpleText("Limit Line");
    text->setPos(_width * GRID_SIZE / 2.0, -20);
}

void Scene::setCurTool(Tool t)
{
    cur_tool = t;
    oldSelectStation = NULL;
}

int Scene::width()
{
    return _width;
}

int Scene::height()
{
    return _height;
}

QPoint Scene::calGridPos(const QPointF &pos)
{
    int x = (int) pos.x();
    int y = (int) pos.y();


    x /= GRID_SIZE;
    y /= GRID_SIZE;

    return QPoint(x, y);
}

void Scene::removeAllStations()
{
    while(true){
        if(iStations.length()<=0)break;
        removeItem(iStations.at(0));
        iStations.removeAt(0);
    }
}
void Scene::removeAllLines()
{
    while(true){
        if(iLines.length()<=0)break;
        removeItem(iLines.at(0));
        iLines.removeAt(0);
    }
}
void Scene::removeAllArcs()
{
    while(true){
        if(iArcs.length()<=0)break;
        removeItem(iArcs.at(0));
        iArcs.removeAt(0);
    }
}

void Scene::load()
{
    //清空原来的
    removeAllStations();
    removeAllLines();
    removeAllArcs();
    iStations.clear();
    iLines.clear();
    iArcs.clear();
    update();
    msgCenter.loadMap();
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if(cur_tool == T_STATION)
        {
            //添加站点~
            //是否要判断站点离得太近呢 附近没有其他站点？？？
            //输入站点名称和RFID
            StationInputDialog *inputDialog = new StationInputDialog;
            int result = inputDialog->exec();
            if(result==1)
            {
                AgvStation station;
                station.name = inputDialog->getName();
                station.rfid = inputDialog->getRfid();
                station.x = event->scenePos().x();
                station.y = event->scenePos().y();
                station.id = getMaxStationId()+1;
                ItemStation *iStation = new ItemStation(station);
                addItem(iStation);
                iStation->initialize();
                iStations.push_back(iStation);
                update();
            }
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

bool Scene::outOfLimitLine(const QPointF &grid_pos)
{
    if (grid_pos.x() < 0
            || grid_pos.x() > _width - 1
            || grid_pos.y() < 0
            || grid_pos.y() > _height - 1)
        return true;
    else
        return false;
}

void Scene::setBackgroundImagePath(QString _path)
{
    backgroundImagePath = _path;
    QImage img(backgroundImagePath);
    if(img.isNull())
    {
        backgroundImagePath = "";
        return ;
    }
    if(itemBackground==NULL)
    {
        itemBackground = new ItemBackground;
        addItem(itemBackground);
    }
    itemBackground->setImg(backgroundImagePath);
    update();
}

void Scene::onLoadStationSuccess()
{
    QList<AgvStation> mapStations = msgCenter.getMapStationListModel();

    removeAllStations();

    for(int i=0;i<mapStations.length();++i){
        AgvStation s = mapStations.at(i);
        ItemStation *station = new ItemStation(s);
        addItem(station);
        station->initialize();
        iStations.push_back(station);
    }
    update();
}

ItemStation *Scene::getStation(int id)
{
    for(QList<ItemStation *>::iterator itr = iStations.begin();itr!=iStations.end();++itr)
        if((*itr)->station.id == id)
            return *itr;
    return NULL;
}

int Scene::getMaxStationId()
{
    int maxId = 0;
    for(QList<ItemStation *>::iterator itr = iStations.begin();itr!=iStations.end();++itr)
        if((*itr)->station.id > maxId)
            maxId = (*itr)->station.id;
    return maxId;
}

ItemLine *Scene::getLine(int id)
{
    for(QList<ItemLine *>::iterator itr = iLines.begin();itr!=iLines.end();++itr)
        if((*itr)->id == id)
            return *itr;
    return NULL;
}

int Scene::getMaxLineId()
{
    int maxId = 0;
    for(QList<ItemLine *>::iterator itr = iLines.begin();itr!=iLines.end();++itr)
        if((*itr)->id > maxId)
            maxId = (*itr)->id;
    return maxId;
}

ItemArc *Scene::getArc(int id)
{
    for(QList<ItemArc *>::iterator itr = iArcs.begin();itr!=iArcs.end();++itr)
        if((*itr)->id == id)
            return *itr;
    return NULL;
}

int Scene::getMaxArcId()
{
    int maxId = 0;
    for(QList<ItemArc *>::iterator itr = iArcs.begin();itr!=iArcs.end();++itr)
        if((*itr)->id > maxId)
            maxId = (*itr)->id;
    return maxId;
}

void Scene::onLoadLineSuccess()
{
    QList<AgvLine> mapLines = msgCenter.getMapLineListModel();

    //清空原来的
    removeAllArcs();
    removeAllLines();

    for(int i=0;i<mapLines.length();++i){
        AgvLine l = mapLines.at(i);
        if(!l.draw)continue;
        ItemStation * start = getStation(l.startStation);
        ItemStation * end = getStation(l.endStation);
        if(start == NULL || end == NULL)continue;

        if(l.line){
            ItemLine *line = new ItemLine(start,end,l.id,QColor(l.color_r,l.color_g,l.color_b));
            addItem(line);
            start->addLine(line);
            end->addLine(line);
            iLines.push_back(line);

        }else{
            ItemArc *arc = new ItemArc(start,end,l.id,QColor(l.color_r,l.color_g,l.color_b));
            arc->P2 = QPointF(l.p1x,l.p1y);
            arc->P3 = QPointF(l.p2x,l.p2y);
            addItem(arc);
            start->addArc(arc);
            end->addArc(arc);
            iArcs.push_back(arc);
        }
    }
    update();
}

void Scene::onSelectItemChanged()
{
    QList<QGraphicsItem *> sis= this->selectedItems();
    if(sis.length()>0){
        QGraphicsItem *lastSelectItem = sis.last();
        if(lastSelectItem->type() == ItemStation::Type)
        {
            ItemStation *newStation = qgraphicsitem_cast<ItemStation*>(lastSelectItem);
            if(cur_tool == T_LINE){
                if(oldSelectStation==NULL){
                    oldSelectStation = newStation;
                }else if(oldSelectStation!=newStation){
                    //判断是否超出了4条线路
                    if(oldSelectStation->lines.length()+oldSelectStation->arcs.length()<4
                            && newStation->lines.length()+newStation->arcs.length()<4){
                        int maxArcId = getMaxArcId();
                        int maxLineId = getMaxLineId();
                        int id = maxArcId +1;
                        if(maxLineId>maxArcId)id = maxLineId + 1;
                        ItemLine *line = new ItemLine(oldSelectStation,newStation,id);
                        addItem(line);
                        oldSelectStation->addLine(line);
                        newStation->addLine(line);
                        oldSelectStation = NULL;
                        iLines.push_back(line);
                        update();
                    }
                }
            }

            else if(cur_tool == T_ARC){
                if(oldSelectStation==NULL){
                    oldSelectStation = newStation;
                }else{
                    if(oldSelectStation->lines.length()+oldSelectStation->arcs.length()<4
                            && newStation->lines.length()+newStation->arcs.length()<4){

                        int maxArcId = getMaxArcId();
                        int maxLineId = getMaxLineId();
                        int id = maxArcId +1;
                        if(maxLineId>maxArcId)id = maxLineId + 1;

                        ItemArc *arc = new ItemArc(oldSelectStation,newStation,id);
                        addItem(arc);
                        oldSelectStation->addArc(arc);
                        newStation->addArc(arc);
                        oldSelectStation = NULL;
                        iArcs.push_back(arc);
                        update();
                    }
                }
            }


            /////删除一个站点，那么同时也要删除和这个站点的所有连线
            else if(cur_tool == T_ERASER){
                //擦除这个站点
                //首先擦除这个站点的所有连线
                for(QList<ItemLine *>::iterator itr = newStation->lines.begin();itr!=newStation->lines.end();++itr){
                    ItemLine *l = *itr;
                    removeItem(l);
                    iLines.removeAll(l);
                }
                newStation->lines.clear();
                for(QList<ItemArc *>::iterator itr = newStation->arcs.begin();itr!=newStation->arcs.end();++itr){
                    ItemArc *a = *itr;
                    removeItem(a);
                    iArcs.removeAll(a);
                }
                newStation->arcs.clear();
                //然后擦除这个站点
                removeItem(newStation);
                iStations.removeAll(newStation);
                update();
            }
        }

        //单独删除一个直线
        else if(lastSelectItem->type() == ItemLine::Type){
            ItemLine *newLine = qgraphicsitem_cast<ItemLine*>(lastSelectItem);
            if(cur_tool == T_ERASER ){
                //从它两头的站点中，将它移出
                newLine->startStation->lines.removeAll(newLine);
                newLine->endStation->lines.removeAll(newLine);
                //然后将它移出
                removeItem(newLine);
                iLines.removeAll(newLine);
                update();
            }
        }

        //单独删除一个弧线
        else if(lastSelectItem->type() == ItemArc::Type){
            //选择了一个弧线
            ItemArc *newArc = qgraphicsitem_cast<ItemArc*>(lastSelectItem);
            if(cur_tool == T_ERASER ){
                //从它两头的站点中，将它移出
                newArc->startStation->arcs.removeAll(newArc);
                newArc->endStation->arcs.removeAll(newArc);
                //然后将它移出
                removeItem(newArc);
                iArcs.removeAll(newArc);
                update();
            }
        }
    }
}
