#ifndef DIALOGADDAGV_H
#define DIALOGADDAGV_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QComboBox;
class QRadioButton;
class QButtonGroup;
class QHBoxLayout;
class QComboBox;

class DialogAddAgv : public QDialog
{
    Q_OBJECT
public:
    explicit DialogAddAgv(QWidget *parent = nullptr);
signals:

public slots:
    void onOkBtn();
private:
    QLabel *agvnameLabel;
    QLineEdit *agvnameInput;

    QLabel *ipLabel;
    QLineEdit *ipInput;

    QLabel *portLabel;
    QLineEdit *portInput;

    QLabel *tipLabel;

    QPushButton *okBtn;
    QPushButton *cancelBtn;

    QLabel *stationLabel;
    QComboBox *stationCbox;
    QList<int> stationIds;
};

#endif // DIALOGADDAGV_H
