#ifndef SERVERCONNNECTION_H
#define SERVERCONNNECTION_H

#include <QObject>
#include <QMutex>
#include <QQueue>
#include "protocol.h"
class ServerConnection : public QObject
{
    Q_OBJECT
public:
    explicit ServerConnection(QObject *parent = nullptr);
    void init(QString ip, int _port);
    void reset(QString ip, int _port);
    bool send(const MSG_Request &msg);
    void disconnect();
    QString getIp(){return QString::fromStdString(serverip);}
    int getPort(){return port;}
signals:
    void onConnecting();
    void onConnect();
    void onDisconnect();
    void onRead(MSG_Response responsMsg);
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

    //获取的一个响应消息
    MSG_Response responsMsg;

    //////发送请求
    MSG_Request write_one_msg;
    QQueue<MSG_Request> m_queue;//发送队列
    QMutex sendQueueMtx;

};

#endif // SERVERCONNNECTION_H
