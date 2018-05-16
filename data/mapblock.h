#ifndef MAPBLOCK_H
#define MAPBLOCK_H

#include <QObject>
#include "mapspirit.h"

//一个block内同时只允许一个AGV
class MapBlock : public MapSpirit
{
    Q_OBJECT
public:
    explicit MapBlock(int _id, std::string _name, QObject *parent = nullptr);
    MapBlock(const MapBlock& b);
    void addSpirit(int spirit){spirits.append(spirit);}
    void removeSpirit(int spirit){spirits.removeAll(spirit);}
    void clear(){spirits.clear();}
    QList<int> getSpirits(){return spirits;}
signals:

public slots:

private:
    QList<int> spirits;
};

#endif // MAPBLOCK_H
