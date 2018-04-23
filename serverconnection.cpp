#include "serverconnection.h"
#include "global.h"


#ifdef WIN32
#define _WIN32_WINNT _WIN32_WINNT_WIN8
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#include <Ws2tcpip.h>
#else
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdarg.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <errno.h>
#endif

#ifdef WIN32
#pragma comment(lib,"ws2_32.lib")
#endif

ServerConnection::ServerConnection(QObject *parent) : QObject(parent),
    need_reconnect(true),
    socketFd(0),
    quit(false)
{

}

void ServerConnection::init(QString ip, int _port)
{
    serverip = (ip.toStdString());
    port = (_port);

    g_threadPool.enqueue([&](){
        while(!quit)
        {
            if(!need_reconnect){
                std::chrono::milliseconds dura(1000);
                std::this_thread::sleep_for(dura);
                continue;
            }
            //创建连接
            emit onConnecting();

            if(!initConnect()){
                //如果创建失败，过会重新创建(5s)
                std::chrono::milliseconds dura(5000);
                std::this_thread::sleep_for(dura);
                continue;
            }

            emit onConnect();

            while(!quit && socketFd>0){
                int recvLen = 0;
                recvLen = ::recv(socketFd, (char *)&(responsMsg.head), sizeof(MSG_Head), 0);

                if (recvLen <= 0)
                {
                    if (errno == EINTR || errno == EAGAIN)
                    {
                    }
                    else
                    {
#ifdef WIN32
                        closesocket(socketFd);
#else
                        close(socketFd);
#endif
                        socketFd = 0;
                        emit onDisconnect();
                        continue;
                    }
                }


                if(responsMsg.head.head == MSG_MSG_Head_HEAD && responsMsg.head.tail == MSG_MSG_Head_TAIL )
                {
                    recvLen = ::recv(socketFd,(char*)&(responsMsg.return_head),sizeof(MSG_RESPONSE_HEAD),0);
                    if (recvLen <= 0)
                    {
                        if (errno == EINTR || errno == EAGAIN)
                        {
                        }
                        else
                        {
#ifdef WIN32
                            closesocket(socketFd);
#else
                            close(socketFd);
#endif
                            socketFd = 0;
                            emit onDisconnect();
                            continue;
                        }
                    }
                    if(responsMsg.head.body_length>0){
                        recvLen = ::recv(socketFd,responsMsg.body,responsMsg.head.body_length,0);
                        if (recvLen <= 0)
                        {
                            if (errno == EINTR || errno == EAGAIN)
                            {
                            }
                            else
                            {
#ifdef WIN32
                                closesocket(socketFd);
#else
                                close(socketFd);
#endif
                                socketFd = 0;
                                emit onDisconnect();
                                continue;
                            }
                        }
                    }

                    //TODO:入队一个消息
                    emit onRead(responsMsg);
                }
            }
        }
    });

    g_threadPool.enqueue([&](){
        while(!quit){
            if(socketFd<0){
                std::chrono::milliseconds dura(50);
                std::this_thread::sleep_for(dura);
                continue;
            }
            sendQueueMtx.lock();
            if(m_queue.size()<=0){
                std::chrono::milliseconds dura(10);
                std::this_thread::sleep_for(dura);
                sendQueueMtx.unlock();
                continue;
            }
            write_one_msg = m_queue.front();
            m_queue.pop_front();
            sendQueueMtx.unlock();

            ::send(socketFd,(char *)&write_one_msg,sizeof(MSG_Head)+write_one_msg.head.body_length,0);
        }
    });
}


void ServerConnection::reset(QString ip,int _port)
{
    //断开链接
    disconnect();
    serverip = ip.toStdString();
    port = (_port);
    need_reconnect = true;
}

bool ServerConnection::send(const MSG_Request &msg){
    if(socketFd<=0)return false;
    sendQueueMtx.lock();
    m_queue.push_back(msg);
    sendQueueMtx.unlock();
    return true;
}

void ServerConnection::disconnect()
{
    need_reconnect = false;
#ifdef WIN32
    closesocket(socketFd);
#else
    close(socketFd);
#endif
    socketFd = 0;
}

bool ServerConnection::initConnect()
{
#ifdef WIN32
    //加载winsocket的库
    int 		Error;
    WORD 	VersionRequested;
    WSADATA 	WsaData;

    VersionRequested = MAKEWORD(2,2);

    Error = WSAStartup(VersionRequested,&WsaData); //start WinSock2
    if(Error!=0)
    {
        printf("WSAStartup fail!\n");
        return false;
    }
    else
    {
        if(LOBYTE(WsaData.wVersion)!=2||HIBYTE(WsaData.wHighVersion)!=2)
        {
            printf("WSAStartup version fail\n");
            WSACleanup();
            return false;
        }
    }
#endif

    int ret;

    if(socketFd>0){
#ifdef WIN32
        closesocket(socketFd);
#else
        close(socketFd);
#endif
        socketFd = 0;
    }

    socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

#ifdef WIN32
    if (INVALID_SOCKET == socketFd)
#else
    if(socketFd < 0)
#endif
    {
        printf("Create TCP socket Err. [Line: %d] [errno: %d]\n", __LINE__, errno);
        return false;
    }

    struct sockaddr_in stServer;
    stServer.sin_family = AF_INET;
    stServer.sin_port = htons(port);
    stServer.sin_addr.s_addr=inet_addr(serverip.c_str());
#ifdef WIN32
    ret = ::connect(socketFd, (LPSOCKADDR)&stServer, sizeof(stServer));
#else
    ret = connect(socketFd, (struct sockaddr *)&stServer, sizeof(stServer));
#endif

#ifdef WIN32
    if (SOCKET_ERROR == ret)
#else
    if(ret < 0)
#endif
    {
#ifdef WIN32
        closesocket(socketFd);
#else
        close(socketFd);
#endif
        socketFd = 0;
        return false;
    }
    return true;
}
