#ifndef STATIONINPUTDIALOG_H
#define STATIONINPUTDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;

class StationInputDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StationInputDialog(QString _name = "",int _rfid = 0,QWidget *parent = nullptr);
    QString getName(){return name;}
    int getRfid(){return rfid;}
signals:

public slots:
    void onOk();
private:
    QLabel *nameLabel;
    QLabel *rfidLabel;
    QLineEdit *nameinput;
    QLineEdit *rfidinput;

    QPushButton *okBtn;
    QPushButton *cancelBtn;
    QString name;
    int rfid;
};

#endif // STATIONINPUTDIALOG_H
