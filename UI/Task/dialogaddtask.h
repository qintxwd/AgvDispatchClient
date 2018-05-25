#ifndef DIALOGADDTASK_H
#define DIALOGADDTASK_H

#include <QDialog>
#include <QTableWidget>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include "../../msgcenter.h"

class DialogAddTask : public QDialog
{
    Q_OBJECT
public:
    explicit DialogAddTask(QWidget *parent = nullptr);

signals:

public slots:
    void onOkBtn();
    void addNode();
    void removeNode();
    void addParam();
    void removeParam();

    void agvsUpdate();
    void stationsUpdate();
private:
    QLabel *agvLabel;
    QComboBox *agvCombobox;

    QLabel *priorityLabel;
    QComboBox *priorityCombobox;

    QTableWidget *extraTable;
    QTableWidget *nodeTable;

    QGroupBox *baseGroupbox;
    QGroupBox *extraGroupbox;
    QGroupBox *nodeGroupbox;

    QPushButton *okBtn;
    QPushButton *cancelBtn;

    QLabel *tipLabel;

    QList<AGV_BASE_INFO> agvinfos;

    QList<MapPoint *> stations;
};

#endif // DIALOGADDTASK_H
