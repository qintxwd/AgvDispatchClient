#ifndef CONFIGURE_H
#define CONFIGURE_H
#include <QString>
#include <QVariant>
#include <QMap>

class Configure
{
public:
    Configure();

    //设置配置文件路径
    void setConfigFile(const QString &file);

    //载入配置文件
    void load();

    //保存配置文件
    void save();

    //读取配置
    QVariant getValue(const QString &what);

    //设置配置，注意未保存到文件。
    void setValue(const QString &what, const QVariant &value);

private:
    QString config_file;

    QMap<QString, QVariant> config_map;
};

#endif // CONFIGURE_H
