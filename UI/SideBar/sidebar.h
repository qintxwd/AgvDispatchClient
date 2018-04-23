#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QList>
#include "commonhead.h"


class SideBar : public QTreeView
{
    Q_OBJECT
public:
    explicit SideBar(QWidget *parent = nullptr);
    void init();
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

signals:
    void clicked(QString p,QString c);
public slots:

private:
};

#endif // SIDEBAR_H
