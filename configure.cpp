#include "configure.h"
#include <QSettings>
#include <QDebug>
#include "global.h"

Configure::Configure(): config_file("./config.conf")
{

}

void Configure::setConfigFile(const QString &file)
{
    config_file = file;
}

void Configure::load()
{
    config_map.clear();
    QSettings settings(config_file, QSettings::IniFormat);
    QStringList keys = settings.allKeys();
    for(QString key : keys)
    {
        config_map.insert(key,settings.value(key));
    }

    //检查是否配置，未配置添加默认值
    if(!config_map.contains("server/ip")){
        config_map.insert("server/ip","127.0.0.1");
    }
    if(!config_map.contains("server/port")){
        config_map.insert("server/port",GLOBAL_SERVER_PORT);
    }
}

void Configure::save()
{
    QSettings settings(config_file, QSettings::IniFormat);
    for (QMap<QString, QVariant>::iterator it = config_map.begin(); it != config_map.end(); ++it)
    {
        settings.setValue(it.key(), it.value());
    }
}

QVariant Configure::getValue(const QString &what)
{
    return config_map.value(what, QVariant());
}

void Configure::setValue(const QString &what, const QVariant &value)
{
    config_map.insert(what, value);
}
