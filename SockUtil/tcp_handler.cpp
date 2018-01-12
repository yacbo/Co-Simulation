
#include "log_util.h"
#include "tcp_handler.h"
#include <WinSock.h>
#include <QNetworkProxyFactory>

TcpNetHandler::TcpNetHandler() : NetHandlerBase()
{
    _b_cli = false;
    _sock_cli = nullptr;
    _sock_svr = nullptr;
    _b_connected = false;

    _rcv_buf.clear();
    _need_rcv_xml_len = 0;

    //fix listen error: the proxy type is invalid for this operation
    QNetworkProxyFactory::setUseSystemConfiguration(false);
}

TcpNetHandler::TcpNetHandler(QTcpSocket* sock, RcvHandler handler, bool cli) : NetHandlerBase()
{
    _b_cli = cli;
    _sock_cli = sock;
    _sock_svr = nullptr;
    _b_connected = true;
    _rcv_handler = handler;

    _rcv_buf.clear();
    _need_rcv_xml_len = 0;
}

TcpNetHandler::~TcpNetHandler()
{
    stop_service();
}

void TcpNetHandler::init_signal_slots(QTcpSocket* sign_obj, TcpNetHandler* slot_obj)
 {
    connect(sign_obj, &QTcpSocket::readyRead, slot_obj, &TcpNetHandler::receive_data);
    connect(sign_obj, &QTcpSocket::disconnected, slot_obj, &TcpNetHandler::disconnected);
 }

bool TcpNetHandler::start_service(const char* ip, uint16_t port, uint16_t dev_port, bool cli, bool pg_rtui)
{
    _b_cli = cli;
    bool ret = true;

    if(_b_cli){       //客户端
        _sock_cli = new QTcpSocket();

        int r_buf_size = 1024 * 16;   //16k
        int s_buf_size = 1024 * 16;   //16k
        int r_val = setsockopt(_sock_cli->socketDescriptor(), SOL_SOCKET, SO_RCVBUF, (char*)&r_buf_size, sizeof(r_buf_size));
        int s_val = setsockopt(_sock_cli->socketDescriptor(), SOL_SOCKET, SO_SNDBUF, (char*)&s_buf_size, sizeof(s_buf_size));

        int try_num = 3;
        bool conn_state = false;
        while(!conn_state && try_num--){
            _sock_cli->bind(dev_port);
            _sock_cli->connectToHost(ip, port);
            conn_state = _sock_cli->waitForConnected(2000);
        }

        if(!conn_state){
            LogUtil::Instance()->Output(MACRO_LOCAL, "connect to server failed", "server ip:", ip, "server port:", port, "device port:", dev_port, "socket id:", _sock_cli->socketDescriptor());
            return false;
        }

        _server_ip = ip;
        _server_port = port;
        _b_connected = true;

        init_signal_slots(_sock_cli, this);

        LogUtil::Instance()->Output(MACRO_LOCAL, "connect to server successfully", "server ip:", ip, "server port:", port, "device port:", dev_port, "socket id:", _sock_cli->socketDescriptor());
    }
    else{           //服务端
        _sock_svr = new QTcpServer();

        int r_buf_size = 1024 * 16;   //16k
        int s_buf_size = 1024 * 16;   //16k
        int r_val = setsockopt(_sock_svr->socketDescriptor(), SOL_SOCKET, SO_RCVBUF, (char*)&r_buf_size, sizeof(r_buf_size));
        int s_val = setsockopt(_sock_svr->socketDescriptor(), SOL_SOCKET, SO_SNDBUF, (char*)&s_buf_size, sizeof(s_buf_size));

        if(ret = _sock_svr->listen(QHostAddress::AnyIPv4, port)){
              connect(_sock_svr, &QTcpServer::newConnection, this, &TcpNetHandler::accept_connection);
        }
        else{
            LogUtil::Instance()->Output(MACRO_LOCAL, "server listen failed", "listen port:", port, "socket id:", _sock_svr->socketDescriptor());
            QString error = _sock_svr->errorString();
        }

    }

     return ret;
}

bool TcpNetHandler::stop_service()
{
    if(_sock_cli){
        if(_b_cli){
            _sock_cli->disconnectFromHost();
            delete _sock_cli;
        }
        _sock_cli = nullptr;
    }

    if(!_b_cli && _sock_svr){
        _sock_svr->close();
        delete _sock_svr;
        _sock_svr = nullptr;
    }

    return true;
}

bool TcpNetHandler::send_data(const char* data, int len, const char* ip, int port)
{
    if(!_sock_cli || !data){
        return false;
    }

    int try_num = 5;
    bool snd_success = false;
    while(!snd_success && try_num--){
        snd_success = _sock_cli->write(data, len) == len;
        if(!_sock_cli->waitForBytesWritten(2000)){
            snd_success = false;
        }
    }

    LogUtil::Instance()->Output(MACRO_LOCAL, "send data length:", len, "send status:", MACRO_SUCFAIL(snd_success));

     return snd_success;
}

bool TcpNetHandler::register_rcv_callback(RcvHandler handler)
{
    _rcv_handler = handler;
    return true;
}

void TcpNetHandler::receive_data()
{
    while(_sock_cli->bytesAvailable()){
        QByteArray bytes = _sock_cli->readAll();
        _rcv_buf.append(bytes);
    }

    LogUtil::Instance()->Output(MACRO_LOCAL, "receive data", "data length::", _rcv_buf.length());

    if(_need_rcv_xml_len == 0){
        memcpy(&_need_rcv_xml_len, _rcv_buf.data() + 1, sizeof(_need_rcv_xml_len));
    }

    int buf_len = _rcv_buf.length();
    const char* buf = _rcv_buf.data();
    int handle_len = _need_rcv_xml_len + 1 + 4 + 4;

    while(buf_len >= handle_len){
        _rcv_handler(buf, handle_len);

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

void TcpNetHandler::accept_connection()
{
    QTcpSocket* tmp_sock = _sock_svr->nextPendingConnection();
    NetHandlerBase* cli_handler = new TcpNetHandler(tmp_sock, _rcv_handler, false);
    init_signal_slots(tmp_sock, (TcpNetHandler*)cli_handler);

    quint16 dst_port = tmp_sock->peerPort();
    QString dst_ip = tmp_sock->peerAddress().toString();

    LogUtil::Instance()->Output(MACRO_LOCAL, "accept connection", "socket id:", tmp_sock->socketDescriptor(), "dst ip::", dst_ip.toStdString(), "dst port:", dst_port);

    emit new_net_handler(dst_ip,  dst_port, eProtocol_tcp, cli_handler);
}

void TcpNetHandler::disconnected()
{
    int sock_id = -1;
    if(_sock_cli){
        sock_id = _sock_cli->socketDescriptor();
        _sock_cli->close();
    }

    LogUtil::Instance()->Output(MACRO_LOCAL, "socket disconnect", "socket id:", sock_id);

    _b_connected = false;
}
