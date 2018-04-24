#ifndef DIALOGMODIFYAGV_H
#define DIALOGMODIFYAGV_H

#include <QDialog>
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
    explicit DialogModifyAgv(int id, QString name, QString ip,int port,QWidget *parent = nullptr);

signals:

public slots:
    void onOkBtn();
private:
    QLabel *nameLabel;
    QLineEdit *nameInput;

    QLabel *ipLabel;
    QLineEdit *ipInput;

    QLabel *portLabel;
    QLineEdit *portInput;

    QLabel *tipLabel;

    QPushButton *okBtn;
    QPushButton *cancelBtn;

    int m_id;
};

#endif // DIALOGMODIFYAGV_H
