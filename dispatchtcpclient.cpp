#include "dispatchtcpclient.h"

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

#include "global.h"

DispatchTcpClient::DispatchTcpClient():
    need_reconnect(true),
    socketFd(0),
    quit(false)
{
}

void DispatchTcpClient::init(std::string ip, int _port, QyhClientReadCallback _readcallback, QyhClientConnectCallback _connectcallback, QyhClientDisconnectCallback _disconnectcallback, QyhClientConnecttingCallback _connectingcallback){
    serverip = (ip);
    port = (_port);
    readcallback = (_readcallback);
    connectcallback = (_connectcallback);
    disconnectcallback = (_disconnectcallback);
    connecttingcallback = (_connectingcallback);
    thread_pool.enqueue([&](){
        while(!quit)
        {
            if(!need_reconnect){
                std::chrono::milliseconds dura(1000);
                std::this_thread::sleep_for(dura);
                continue;
            }
            //创建连接
            connecttingcallback();

            if(!initConnect()){
                //如果创建失败，过会重新创建(5s)
                std::chrono::milliseconds dura(5000);
                std::this_thread::sleep_for(dura);
                continue;
            }

            connectcallback();

            while(!quit && socketFd>0){
                int rectlen = 0;
                rectlen = ::recv(socketFd, responsMsg.head, sizeof(Client_Common_Head), 0);

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
                        disconnectcallback();
                        continue;
                    }
                }


                if(responsMsg.head.head == CLIENT_COMMON_HEAD_HEAD && responsMsg.head.tail ==CLIENT_COMMON_HEAD_TAIL )
                {
                    rectlen = ::recv(socketFd,responsMsg.return_head,sizeof(CLIENT_RETURN_MSG_HEAD),0);
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
                            disconnectcallback();
                            continue;
                        }
                    }
                    if(responsMsg.head.body_length>0){
                        rectlen = ::recv(socketFd,responsMsg.body,responsMsg.head.body_length,0);
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
                                disconnectcallback();
                                continue;
                            }
                        }
                    }

                    //TODO:入队一个消息

                }
            }
        }
    });

    thread_pool.enqueue([&](){
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
            m_queue.pop();

            if(sizeof(Client_Common_Head)== ::send(socketFd,&(write_one_msg.head),sizeof(Client_Common_Head),0))
            {
                if(write_one_msg.head.body_length>0)
                    ::send(socketFd,&(write_one_msg.body), write_one_msg.head.body_length,0);
            }
            sendQueueMtx.unlock();
        }
    });
}


void DispatchTcpClient::connectToServer(QString ip,int port)
{
    //断开链接
    disconnect();
    serverip = _ip;
    port = (_port);
    need_reconnect = true;
}

bool DispatchTcpClient::send(const Client_Request_Msg &msg){
    if(socketFd<=0)return false;
    sendQueueMtx.lock();
    m_queue.push(msg);
    sendQueueMtx.unlock();
    return true;
}

void DispatchTcpClient::disconnect()
{
    need_reconnect = false;
#ifdef WIN32
    closesocket(socketFd);
#else
    close(socketFd);
#endif
    socketFd = 0;
}

bool DispatchTcpClient::initConnect()
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
