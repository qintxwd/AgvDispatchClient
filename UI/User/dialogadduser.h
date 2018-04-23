#ifndef DIALOGADDUSER_H
#define DIALOGADDUSER_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QComboBox;
class QRadioButton;
class QButtonGroup;
class QHBoxLayout;

class DialogAddUser : public QDialog
{
    Q_OBJECT
public:
    explicit DialogAddUser(QWidget *parent = nullptr);

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
};

#endif // DIALOGADDUSER_H
