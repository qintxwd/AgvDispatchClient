#ifndef DockDevice_H
#define DockDevice_H

#include <QDockWidget>
#include <QListView>
#include <QComboBox>
#include <QStringListModel>
#include "../../protocol.h"
#include "../../msgcenter.h"
class QPushButton;
class QComboBox;
class QLabel;

class DockDevice : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockDevice(QWidget *parent = nullptr);
    void stationsList();
    void elevatorList();
signals:

public slots:
    void appendLog(DEVICE_LOG log);
    void onqueryBtn();
    void onstopBtn();
    void onresumeBtn();
private:
    QListView *listview;
    QStringListModel *listModel;

    QWidget *content;
    QLabel *stationLabel;
    QComboBox *stationInput;
    QPushButton *queryBtn;


    QLabel *elevatorLabel;
    QComboBox *elevatorInput;
    QPushButton *stopBtn;
    QPushButton *resumeBtn;

    //最多显示多少行日志
    const int show_max_row = 1000;

    //超出的部分，删除前面多少行的
    const int show_cut_row = 300;
    QList<MapPoint *> charge_stations;
    QMap<int, MapPoint*> map_charge_stations;

    QList<MapGroup *> elevator_stations;
    QMap<int, MapGroup*> map_elevator_stations;
};

#endif // DockDevice_H
