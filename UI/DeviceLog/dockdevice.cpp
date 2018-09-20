#include "dockdevice.h"
#include "../../global.h"

DockDevice::DockDevice(QWidget *parent) : QDockWidget(tr("DeviceManage"),parent)
{
    content = new QWidget;
    stationsList();
    elevatorList();
    setAllowedAreas(Qt::AllDockWidgetAreas);
    listview = new QListView;
    listview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listModel = new QStringListModel;
    listview->setModel(listModel);
    stationLabel = new QLabel(QStringLiteral("位置:"));
    stationInput = new QComboBox;

    elevatorLabel = new QLabel(QStringLiteral("电梯:"));
    elevatorInput = new QComboBox;
    stopBtn = new QPushButton(QStringLiteral("停用"));
    connect(stopBtn,SIGNAL(clicked(bool)),this,SLOT(onstopBtn()));
    resumeBtn = new QPushButton(QStringLiteral("恢复"));
    connect(resumeBtn,SIGNAL(clicked(bool)),this,SLOT(onresumeBtn()));

    for(auto onestation:charge_stations){
        stationInput->addItem(QString::fromStdString(onestation->getName()));
    }

    for(auto oneelevator:elevator_stations){
        elevatorInput->addItem(QString::fromStdString(oneelevator->getName()));
    }
    queryBtn = new QPushButton(QStringLiteral("查询"));
    connect(queryBtn,SIGNAL(clicked(bool)),this,SLOT(onqueryBtn()));

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(elevatorLabel,0,0);
    gridLayout->addWidget(elevatorInput,0,1);
    gridLayout->addWidget(stopBtn,0,2);
    gridLayout->addWidget(resumeBtn,0,3);

    gridLayout->addWidget(stationLabel,1,0);
    gridLayout->addWidget(stationInput,1,1);
    gridLayout->addWidget(queryBtn,1,2);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addItem(gridLayout);
    mainLayout->addWidget(listview);

    content->setLayout(mainLayout);
    setWidget(content);
    connect(&msgCenter,SIGNAL(pubOneLog(DEVICE_LOG)),this,SLOT(appendLog(DEVICE_LOG)));
}

void DockDevice::onstopBtn()
{
    msgCenter.elevatorControl(elevator_stations[elevatorInput->currentIndex()]->getId(), 0);
}


void DockDevice::onresumeBtn()
{
    msgCenter.elevatorControl(elevator_stations[elevatorInput->currentIndex()]->getId(), 1);
}


void DockDevice::onqueryBtn()
{
    listModel->removeRows(0, listModel->rowCount());
    listModel->insertRows(0, 1);
    QModelIndex index = listModel->index(0);
    listModel->setData(index, "ID\t温度\t起始电压\t结束电压\t充电时长\t充电电量");
    msgCenter.queryDeviceLog(charge_stations[stationInput->currentIndex()]->getId());
}
void DockDevice::appendLog(DEVICE_LOG log)
{
    //    qDebug()<<"log==>"<<QString::fromUtf8(log.msg.c_str(),log.msg.length());

    int row = listModel->rowCount();
    if(row<show_max_row){
        listModel->insertRows(row, 1);
        QModelIndex index = listModel->index(row);
        listModel->setData(index, QString("%1\t%2\t%3\t%4\t%5\t%6").arg(QString::number(log.id)).arg(QString::number(log.temperature)).arg(QString::number(log.start_voltage/10.0)).arg(QString::number(log.end_voltage/10.0)).arg(QString::number(log.charge_time)).arg(QString::number(log.charge_power/10.0)));
    }else{
        for(int i=0;i<show_cut_row;++i)listModel->removeRow(0);
        row = listModel->rowCount();
        listModel->insertRows(row, 1);
        QModelIndex index = listModel->index(row);
        listModel->setData(index, QString("%1\t%2\t%3\t%4\t%5\t%6").arg(QString::number(log.id)).arg(QString::number(log.temperature)).arg(QString::number(log.start_voltage/10.0)).arg(QString::number(log.end_voltage/10.0)).arg(QString::number(log.charge_time)).arg(QString::number(log.charge_power/10.0)));
    }
    listview->scrollToBottom();
}

void DockDevice::stationsList()
{
    charge_stations.clear();
    map_charge_stations.clear();
    auto ae = g_onemap.getAllElement();
    for(auto e:ae){
        if(e->getSpiritType() == MapSpirit::Map_Sprite_Type_Point){
            MapPoint *pp = static_cast<MapPoint *>(e);
            if(MapPoint ::Map_Point_Type_CHARGE == pp->getPointType())
            {
                charge_stations.push_back(pp);
                map_charge_stations.insert(pp->getId(), pp);
            }
        }
    }
}

void DockDevice::elevatorList()
{
    auto ae = g_onemap.getGroups();
    for(auto e:ae)
    {
        if(e->getSpiritType() == MapSpirit::Map_Sprite_Type_Group){
            MapGroup *pp = static_cast<MapGroup *>(e);
            if(3 == pp->getGroupType())
            {
                elevator_stations.push_back(pp);
                map_elevator_stations.insert(pp->getId(), pp);
            }
        }
    }
}
