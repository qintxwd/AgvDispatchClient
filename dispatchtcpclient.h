#ifndef DISPATCHTCPCLIENT_H
#define DISPATCHTCPCLIENT_H
#include <QString>
#include <mutex>
#include <atomic>
#include <queue>
#include "protocol.h"

class DispatchTcpClient
{
public:

    enum{
        QYH_TCP_CLIENT_RECV_BUF_LEN = 1500,
    };

    typedef std::function<void (const Client_Response_Msg msg)> QyhClientReadCallback;
    typedef std::function<void ()> QyhClientConnectCallback;
    typedef std::function<void ()> QyhClientDisconnectCallback;
    typedef std::function<void ()> QyhClientConnecttingCallback;

    DispatchTcpClient();

    void init(std::string ip, int _port, QyhClientReadCallback _readcallback, QyhClientConnectCallback _connectcallback, QyhClientDisconnectCallback _disconnectcallback,QyhClientConnecttingCallback _connectingcallback);
    void connectToServer(QString ip,int port);


    bool send(const Client_Request_Msg &msg);

    void disconnect();
private:
    bool initConnect();

    volatile bool need_reconnect;//重连

    volatile bool quit;

    int socketFd;

    std::string serverip;

    int port;

    QyhClientReadCallback readcallback;

    QyhClientConnectCallback connectcallback;

    QyhClientDisconnectCallback disconnectcallback;

    QyhClientConnecttingCallback connecttingcallback;
    //获取的一个响应消息
    Client_Response_Msg responsMsg;

    //////发送请求
    Client_Request_Msg write_one_msg;
    std::queue<Client_Request_Msg> m_queue;//发送队列
    std::mutex sendQueueMtx;
};

#endif // DISPATCHTCPCLIENT_H
