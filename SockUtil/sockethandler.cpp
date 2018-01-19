
#include "log_util.h"
#include "sockethandler.h"
#include <QtConcurrent>

SocketHandler::SocketHandler()
{
    _b_quit = false;
    _rcv_buf.clear();
    _need_rcv_xml_len = 0;
}

SocketHandler::~SocketHandler()
{

}

bool SocketHandler::InitSocket(const char* server_ip, int server_port, int dev_port, bool brcv_pg_rtui)
{
    WSADATA wsd;
    if(WSAStartup(MAKEWORD(2,2),&wsd) != 0){
        return false;
    }

    // 创建套接字
      _sock_cli = socket(AF_INET,SOCK_DGRAM,0);
      if(_sock_cli == INVALID_SOCKET){
          LogUtil::Instance()->Output(MACRO_LOCAL, "create socket failed");
          return false;
      }

      unsigned long block=0;                              //0 阻塞， 1非阻塞
      int ret = ioctlsocket(_sock_cli, FIONBIO, &block);
      if(ret != NO_ERROR){
          LogUtil::Instance()->Output(MACRO_LOCAL, "set socket block mode failed");
          return false;
      }

      _dev_port = dev_port;
      _server_ip = server_ip;
      _server_port = server_port;

      _local_addr.sin_family=AF_INET;
      _local_addr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
      _local_addr.sin_port=htons(dev_port == 0 ? server_port : dev_port);

      ret = bind(_sock_cli, (SOCKADDR*)&_local_addr, sizeof(SOCKADDR));
      if(ret != NO_ERROR){
          LogUtil::Instance()->Output(MACRO_LOCAL, "bind port: ", dev_port == 0 ? server_port : dev_port, "failed");
          return false;
      }

      int src_addr_len = sizeof(SOCKADDR);
      ret = getsockname(_sock_cli, (sockaddr*)&_src_addr, &src_addr_len);
      if(ret == NO_ERROR){
          quint16 cli_port = ntohs(_src_addr.sin_port);
          QString cli_addr = inet_ntoa(_src_addr.sin_addr);
      }

      _b_rcv_pg_rtui = brcv_pg_rtui;

      QtConcurrent::run(this, &SocketHandler::RcvThread);

      return true;
}

void SocketHandler::RegisterRcvCallback(RcvCallback cb)
{
    _rcv_callback = cb;
}

void SocketHandler::Quit()
{
    _b_quit = true;
    QThread::msleep(200);
}

long SocketHandler::Send(const char* ip, int port, const char* data, int len)
{
    SOCKADDR_IN dst_addr;
    dst_addr.sin_family=AF_INET;
    dst_addr.sin_port=htons(port);
    dst_addr.sin_addr.S_un.S_addr=inet_addr(ip);

    long snd_len = sendto(_sock_cli, data, len, 0, (SOCKADDR*)&dst_addr, sizeof(SOCKADDR));
    bool snd_status = snd_len == len;

    LogUtil::Instance()->Output(MACRO_LOCAL, "send data length:", snd_len, "send status:", MACRO_SUCFAIL(snd_status));

    return snd_len;
}

void SocketHandler::RcvThread()
{
    int ret = 0;
    uint16_t pg_rtui_len = 0;
    const int pg_len_offset = 8;
    const int buf_size = 1024 * 100;
    char buf[buf_size] = {0};
    int src_addr_len = sizeof(SOCKADDR);

    while(!_b_quit){
        memset(buf, 0, buf_size);
        int rcv_len = recvfrom(_sock_cli, buf, buf_size, 0,  (sockaddr*)&_src_addr, &src_addr_len);
        if(rcv_len <= 0){
            LogUtil::Instance()->Output(MACRO_LOCAL, "receive data failed");
            ret = WSAGetLastError();
            continue;
        }

        if(_b_rcv_pg_rtui){
            LogUtil::Instance()->Output(MACRO_LOCAL, "received pg_rtui data, data length:", rcv_len);
            _rcv_callback(buf, rcv_len);
            continue;
        }

        LogUtil::Instance()->Output(MACRO_LOCAL, "receive xml data", "data length::", _rcv_buf.length());

        _rcv_buf.append(buf, rcv_len);
        if(_need_rcv_xml_len == 0){
            memcpy(&_need_rcv_xml_len, _rcv_buf.data() + 1, sizeof(_need_rcv_xml_len));
        }

        int buf_len = _rcv_buf.length();
        const char* buf = _rcv_buf.data();
        int handle_len = _need_rcv_xml_len + 1 + 4 + 4;

        while(buf_len >= handle_len){
            quint16 cli_port = ntohs(_src_addr.sin_port);
            QString cli_addr = inet_ntoa(_src_addr.sin_addr);
            emit new_net_handler(cli_addr, cli_port);

            _rcv_callback(buf, handle_len);

            buf += handle_len;
            buf_len -= handle_len;

            if(buf_len > 0){               //剩余一部分数据
                if(buf_len >= 5){         //可以定位消息长度.
                    memcpy(&_need_rcv_xml_len, buf + 1, sizeof(_need_rcv_xml_len));
                    handle_len = _need_rcv_xml_len + 1 + 4 + 4;
                }

                if(buf_len < 5 || buf_len < handle_len){   //不能定位消息长度或者不是一个完整的数据包.
                    QByteArray tmp = _rcv_buf.right(buf_len);
                    _need_rcv_xml_len = 0;
                    _rcv_buf = tmp;
                    break;
                }
            }
            else{                                //数据是一个完整的数据包.
                _need_rcv_xml_len = 0;
                _rcv_buf.clear();
                break;
            }
        }
    }
}
