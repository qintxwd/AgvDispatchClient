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

#include <QDebug>

#ifdef WIN32
#pragma comment(lib,"ws2_32.lib")
#endif

#include "common.h"

ServerConnection::ServerConnection(QObject *parent) : QObject(parent),
    need_reconnect(true),
    socketFd(0),
    quit(false)
{

}

ServerConnection::~ServerConnection()
{
    quit = true;
#ifdef WIN32
    closesocket(socketFd);
#else
    close(socketFd);
#endif
    condition.wakeAll();
    thread_read.join();
    thread_send.join();
}

void ServerConnection::packageProcess(const char *data,int len)
{
    buffer.append(data,len);
    if(buffer.size()<=json_len){
        return ;
    }

    while(true)
    {
        if(buffer.length()<=0)break;
        //寻找包头
        int pack_head = buffer.find(MSG_MSG_HEAD);
        if(pack_head>=0){
            //丢弃前面的无用数据
            buffer.removeFront(pack_head);
            //json长度
            json_len = buffer.getInt32(1);
            if(json_len==0){
                //空包,清除包头和长度信息。
                buffer.removeFront(1+sizeof(int32_t));
            }else if(json_len>0){
                if(json_len <= buffer.length()-1-sizeof(int32_t))
                {
                    //包完整
                    //处理包数据
                    Json::Reader reader;
                    Json::Value root;
                    std::string sss = std::string(buffer.data(1+sizeof(int32_t)),json_len);
                    if (reader.parse(sss, root))
                    {
                        qDebug() << "RECV! len=" << json_len << " json=\n" << sss.c_str();
                        emit onRead(root);
                    }
                    //清除报数据
                    buffer.removeFront(1+sizeof(int32_t)+json_len);
                    json_len = 0;
                }else{
                    //包不完整，继续接收
                    break;
                }
            }
        }else{
            //未找到包头
            buffer.clear();
            break;
        }
    }
}

void ServerConnection::init(QString ip, int _port)
{
    serverip = (ip.toStdString());
    port = (_port);

    thread_read = std::thread([&](){
        char perBuffer[MSG_READ_BUFFER_LENGTH];//每次接收的缓存区
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

            json_len = 0;

            emit onConnect();
            while(!quit && socketFd>0)
            {
                int recvLen = ::recv(socketFd,perBuffer, MSG_READ_BUFFER_LENGTH, 0);
                if (recvLen <= 0)
                {
                    if (errno == 0 || errno == EINTR || errno == EAGAIN)
                    {
                        continue;
                    }
                    else
                    {
                        qDebug()<<"recvLen="<<recvLen<<" errno="<<errno<<" msg="<<strerror(errno) <<" socket="<<socket;
#ifdef WIN32
                        closesocket(socketFd);
#else
                        close(socketFd);
#endif
                        socketFd = 0;
                        buffer.clear();
                        emit onDisconnect();
                        continue;
                    }
                }else{
                    packageProcess(perBuffer,recvLen);
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
            //std::string json_str = write_one_msg.toStyledString();
            std::string msg = write_one_msg.toStyledString();
            int length = msg.length();

            char *send_buffer = new char[length + 6];
            memset(send_buffer, 0, length + 6);
            send_buffer[0] = MSG_MSG_HEAD;
            memcpy_s(send_buffer + 1, length+5, (char *)&length, sizeof(length));
            memcpy_s(send_buffer +5, length + 1, msg.c_str(),msg.length());

            int per_send_length = MSG_READ_BUFFER_LENGTH;

            if(length+5<per_send_length){
                if(length+5 != ::send(socketFd,send_buffer,length+5,0))
                {
                    qDebug()<<("send error ") ;
                }
            }else{
                int packindex = 0;
                char *per_send_buffer = new char[per_send_length];
                int resendTime = 3;
                while(true){
                    int this_time_send_length = (length +5) - packindex * per_send_length;
                    if(this_time_send_length<=0)break;
                    if(this_time_send_length > per_send_length)this_time_send_length = per_send_length;

                    memcpy(per_send_buffer,send_buffer+packindex*per_send_length,this_time_send_length);
                    bool sendRet = false;
                    for(int i=0;i<resendTime;++i){
                        if(this_time_send_length == ::send(socketFd,per_send_buffer,this_time_send_length,0)){
                            sendRet = true;
                            break;
                        }
                    }

                    if(!sendRet){
                        qDebug() << ("send error ") ;
                        break;
                    }

                    ++packindex;
                }

            }
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

//    //设置接收超时为1000ms
//    int nTimeout=10000;
//#ifdef WIN32
//    if( SOCKET_ERROR == setsockopt(socketFd, SOL_SOCKET, SO_RCVTIMEO,(char *)&nTimeout, sizeof( int ) ) )
//#else
//    if( 0 == setsockopt(socketFd, SOL_SOCKET, SO_RCVTIMEO,(char *)&nTimeout, sizeof( int ) ) )
//#endif
//    {
//        printf("Set SO_RCVTIMEO error !\n" );
//    }


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
