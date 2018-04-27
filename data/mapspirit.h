#ifndef MAPSPIRIT_H
#define MAPSPIRIT_H

#include <QString>

class MapSpirit
{
public:
    enum Map_Spirit_Type
    {
        Map_Sprite_Type_Point = 0,
        Map_Sprite_Type_Path,
        Map_Sprite_Type_Floor,
    };

    MapSpirit();

    Map_Spirit_Type getSpiritType() const
    {
        return spirit_type;
    }
    void setSpiritType(Map_Spirit_Type __type){spirit_type = __type;}

    int getId(){return id;}
    void setId(int _id){id=_id;}

    QString getName(){return name;}
    void setName(QString _name){name=_name;}
protected:
    int id;
    QString name;
    Map_Spirit_Type spirit_type;
};

#endif // MAPSPIRIT_H
