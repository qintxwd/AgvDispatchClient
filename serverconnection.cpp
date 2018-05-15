#include "serverconnection.h"
#include "global.h"


#ifdef WIN32
#define _WIN32_WINNT _WIN32_WINNT_WIN8
#include <WinSock2.h>
#include <windows.h>
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

ServerConnection::~ServerConnection()
{
    quit = true;
    condition.wakeAll();
    thread_read.join();
    thread_send.join();
}

void ServerConnection::init(QString ip, int _port)
{
    serverip = (ip.toStdString());
    port = (_port);

    thread_read = std::thread([&](){
        while(!quit)
        {
            if(!need_reconnect){
                std::chrono::milliseconds dura(50);
                std::this_thread::sleep_for(dura);
                continue;
            }
            //创建连接
            emit onConnecting();

            if(!initConnect()){
                //如果创建失败，过会重新创建(50ms)
                std::chrono::milliseconds dura(50);
                std::this_thread::sleep_for(dura);
                continue;
            }

            emit onConnect();

            while(!quit && socketFd>0){
                int recvLen = 0;
                char head_flag;
                int32_t json_length;
                recvLen = ::recv(socketFd,&head_flag, 1, 0);

                if (recvLen <= 0)
                {
                    if (errno == 0 || errno == EINTR || errno == EAGAIN)
                    {
                        continue;
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


                if(head_flag == MSG_MSG_HEAD)
                {
                    recvLen = ::recv(socketFd,(char*)&(json_length),sizeof(json_length),0);
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
                    if(json_length>0){
                        char *buffer = new char[json_length+1];
                        int read_pos = 0;
                        recvLen = 0;
                        while(true){
                            recvLen += ::recv(socketFd,buffer+read_pos,json_length-recvLen,0);
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
                                    break;
                                }
                            }else if(recvLen==json_length){
                                break;
                            }else{
                                read_pos += recvLen;
                            }
                        }

                        if(recvLen==json_length){
                            Json::Reader reader;
                            Json::Value root;
                            if (reader.parse(std::string(buffer,json_length), root))
                            {
                                emit onRead(root);
                            }
                        }
                        delete []buffer;
                    }else{
                        continue;
                    }
                }
            }
        }
    });

    thread_send = std::thread([&](){
        while(!quit){
            if(socketFd<0){
                std::chrono::milliseconds dura(50);
                std::this_thread::sleep_for(dura);
                continue;
            }

            sendQueueMtx.lock();
            if(m_queue.size()<=0){
                condition.wait(&sendQueueMtx);
            }
            if(quit){
                sendQueueMtx.unlock();
                break;
            }
            Json::Value write_one_msg = m_queue.front();
            m_queue.pop_front();
            sendQueueMtx.unlock();

            char headLeng[MSG_JSON_PREFIX_LENGTH] = {0xAA};
            int length = write_one_msg.toStyledString().length();
            //send head and length
            snprintf(headLeng+1,4, (char *)&length,sizeof(length));
            ::send(socketFd,headLeng,MSG_JSON_PREFIX_LENGTH,0);
            if (::send(socketFd,headLeng,MSG_JSON_PREFIX_LENGTH,0)!=MSG_JSON_PREFIX_LENGTH)
            {
                continue ;
            }
            ::send(socketFd,write_one_msg.toStyledString().c_str(),write_one_msg.toStyledString().length(),0);
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

bool ServerConnection::send(const Json::Value &json){
    if(socketFd<=0)return false;

    sendQueueMtx.lock();
    m_queue.push_back(json);
    condition.wakeAll();
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

    //设置接收超时为1000ms
    int nTimeout=1000;
#ifdef WIN32
    if( SOCKET_ERROR == setsockopt(socketFd, SOL_SOCKET, SO_RCVTIMEO,(char *)&nTimeout, sizeof( int ) ) )
#else
    if( 0 == setsockopt(socketFd, SOL_SOCKET, SO_RCVTIMEO,(char *)&nTimeout, sizeof( int ) ) )
#endif
    {
        printf("Set SO_RCVTIMEO error !\n" );
    }


    struct sockaddr_in stServer;
    stServer.sin_family = AF_INET;
    stServer.sin_port = htons(port);
    stServer.sin_addr.s_addr=inet_addr(serverip.c_str());
#ifdef WIN32
    ret = ::connect(socketFd, (LPSOCKADDR)&stServer, sizeof(stServer));
#else
    ret = ::connect(socketFd, (struct sockaddr *)&stServer, sizeof(stServer));
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
