#include "sockethandler.h"
#include <QtConcurrent>

SocketHandler::SocketHandler()
{

}

bool SocketHandler::InitSocket(const char* server_ip, int server_port, int dev_port)
{
    WSADATA wsd;
    if(WSAStartup(MAKEWORD(2,2),&wsd) != 0){
        return false;
    }

    // 创建套接字
      _sock_cli = socket(AF_INET,SOCK_DGRAM,0);
      if(_sock_cli == INVALID_SOCKET){
          return false;
      }

      unsigned long block=0;                              //0 阻塞， 1非阻塞
      int ret = ioctlsocket(_sock_cli, FIONBIO, &block);
      if(ret != NO_ERROR){
          return false;
      }

      _dev_port = dev_port;
      _server_ip = server_ip;
      _server_port = server_port;

      _server_addr.sin_family=AF_INET;
      _server_addr.sin_port=htons(server_port);
      //_server_addr.sin_addr.S_un.S_addr=inet_addr();
      _server_addr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);

      ret = bind(_sock_cli, (SOCKADDR*)&_server_addr, sizeof(SOCKADDR));
      if(ret != NO_ERROR){
          return false;
      }

      QtConcurrent::run(this, &SocketHandler::RcvThread);
}

void SocketHandler::RegisterRcvCallback(RcvCallback cb)
{
    _rcv_callback = cb;
}

long SocketHandler::Send(const char* ip, int port, const char* data, int len)
{
    SOCKADDR_IN addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.S_un.S_addr=inet_addr(ip);

    return sendto(_sock_cli, data, len, 0, (SOCKADDR*)&addr, sizeof(SOCKADDR));
}

void SocketHandler::RcvThread()
{
    int ret = 0;
    int addr_len = sizeof(SOCKADDR);
    const int buf_size = 1024 * 10;
    char buf[buf_size] = {0};

    while(true){
        memset(buf, 0, buf_size);
        int rcv_len = recvfrom(_sock_cli, buf, buf_size, 0, (SOCKADDR*)&_sender_addr, &addr_len);

        if(rcv_len > 0){
            quint16 cli_port = ntohs(_sender_addr.sin_port);
            QString cli_addr = inet_ntoa(_sender_addr.sin_addr);
            emit new_net_handler(cli_addr, cli_port);

            _rcv_callback(buf, rcv_len);
        }
        else{
            ret = WSAGetLastError();
        }
    }
}
