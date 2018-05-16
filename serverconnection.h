#ifndef SERVERCONNNECTION_H
#define SERVERCONNNECTION_H
#include <thread>
#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QQueue>
#include <QWaitCondition>
#include "protocol.h"
class ServerConnection : public QObject
{
    Q_OBJECT
public:
    explicit ServerConnection(QObject *parent = nullptr);
    ~ServerConnection();
    void init(QString ip, int _port);
    void reset(QString ip, int _port);
    bool send(const Json::Value &json);
    void disconnect();
    QString getIp(){return QString::fromStdString(serverip);}
    int getPort(){return port;}
signals:
    void onConnecting();
    void onConnect();
    void onDisconnect();
    void onRead(Json::Value responsMsg);
public slots:

private:
    enum{
        QYH_TCP_CLIENT_RECV_BUF_LEN = 1500,
    };
    bool initConnect();

    volatile bool need_reconnect;//重连

    volatile bool quit;

    int socketFd;

    std::string serverip;

    int port;

    //////发送请求
    QQueue<Json::Value> m_queue;//发送队列
    QWaitCondition condition;
    QMutex sendQueueMtx;

    std::thread thread_read;
    std::thread thread_send;
};

#endif // SERVERCONNNECTION_H
