#ifndef MAPGROUP_H
#define MAPGROUP_H

#include <QObject>
#include "mapspirit.h"

class MapGroup : public MapSpirit
{
    Q_OBJECT
public:
    explicit MapGroup(QObject *parent = nullptr);

signals:

public slots:

private:
    QList<int> spirits;
    QList<int> agvs;
};

#endif // MAPGROUP_H
