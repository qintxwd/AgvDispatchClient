#ifndef DIALOGMODIFYAGV_H
#define DIALOGMODIFYAGV_H

#include <QDialog>
#include "../../msgcenter.h"

class QLabel;
class QLineEdit;
class QPushButton;
class QComboBox;
class QRadioButton;
class QButtonGroup;
class QHBoxLayout;
class DialogModifyAgv : public QDialog
{
    Q_OBJECT
public:
    explicit DialogModifyAgv(int id, QString name, QString ip,int port, int station, QWidget *parent = nullptr);

signals:

public slots:
    void onOkBtn();
    void stationsUpdate();
private:
    QLabel *nameLabel;
    QLineEdit *nameInput;

    QLabel *ipLabel;
    QLineEdit *ipInput;

    QLabel *portLabel;
    QLineEdit *portInput;

    QLabel *stationLabel;
    QComboBox *stationInput;

    QLabel *tipLabel;

    QPushButton *okBtn;
    QPushButton *cancelBtn;

    int m_id;
    QList<MapPoint *> stations;
    QMap<int, MapPoint*> map_stations;
};

#endif // DIALOGMODIFYAGV_H
