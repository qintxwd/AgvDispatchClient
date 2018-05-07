#ifndef DIALOGROOTPATH_H
#define DIALOGROOTPATH_H

#include <QDialog>
#include "../../data/onemap.h"


class QComboBox;

class DialogRootPath : public QDialog
{
    Q_OBJECT
public:
    explicit DialogRootPath(OneMap *_onemap,QWidget *parent = nullptr);
    void init();
signals:
    void sig_add_root_path(MapPoint *from,MapPoint *to);
public slots:
    void on_okbtn_clicked(bool b);
    void on_cancelbtn_clicked(bool b);
    void on_fromComboboxA_currentIndexChanged(int index);
    void on_toComboboxA_currentIndexChanged(int index);
private:
    OneMap *onemap;
    QPushButton *okbtn;
    QPushButton *cancelbtn;

    QComboBox *fromComboboxA;

    QComboBox *fromComboboxB;

    QComboBox *toComboboxA;

    QComboBox *toComboboxB;
};

#endif // DIALOGROOTPATH_H
