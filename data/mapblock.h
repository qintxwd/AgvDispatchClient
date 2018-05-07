#ifndef MAPBLOCK_H
#define MAPBLOCK_H

#include <QObject>
#include "mapspirit.h"

class MapBlock : public MapSpirit
{
    Q_OBJECT
public:
    explicit MapBlock(int _id, QString _name, QObject *parent = nullptr);
    MapBlock(const MapBlock& b);
    void addSpirit(int spirit){spirits.append(spirit);}
    void removeSpirit(int spirit){spirits.removeAll(spirit);}

    QList<int> getSpirits(){return spirits;}
signals:

public slots:

private:
    QList<int> spirits;
};

#endif // MAPBLOCK_H
