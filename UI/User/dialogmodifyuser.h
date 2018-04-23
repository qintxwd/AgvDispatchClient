#ifndef DIALOGMODIFYUSER_H
#define DIALOGMODIFYUSER_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QComboBox;
class QRadioButton;
class QButtonGroup;
class QHBoxLayout;

class DialogModifyUser : public QDialog
{
    Q_OBJECT
public:
    explicit DialogModifyUser(int id, QString username, QString password, int role, QWidget *parent = nullptr);

signals:

public slots:
    void onOkBtn();
private:
    QLabel *usernameLabel;
    QLineEdit *usernameInput;

    QLabel *passwordLabel;
    QLineEdit *passwordInput;

    QLabel *passwordConfirmLabel;
    QLineEdit *passwordConfirmInput;

    QLabel *roleLabel;
    QHBoxLayout *roleGroup;
    QRadioButton *visitorRadio;
    QRadioButton *operatorRadio;
    QRadioButton *managerRadio;
    QRadioButton *sysManagerRadio;
    QRadioButton *developerRadio;

    QLabel *tipLabel;

    QPushButton *okBtn;
    QPushButton *cancelBtn;

    int m_id;
};

#endif // DIALOGMODIFYUSER_H
