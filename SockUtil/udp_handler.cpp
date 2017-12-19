
#include <QtConcurrent>
#include "udp_handler.h"

UdpNetHandler::UdpNetHandler() : NetHandlerBase()
{
    _sock_cli = nullptr;
}

UdpNetHandler:: ~UdpNetHandler()
{
    stop_service();
}

bool UdpNetHandler::start_service(const char* ip, uint16_t port, uint16_t dev_port, bool cli)
{
    if(_sock_cli){
       return true;
    }

    _b_cli = cli;
    _server_ip = ip;
    _server_port = port;

    _sock_cli = new QUdpSocket(this);
    if(!_sock_cli){
        return false;
    }

    bool ret = true;

    if(cli){                                                          //作客户端
//        _sock_cli->connectToHost(QHostAddress(_server_ip), _server_port);
//        if(!_sock_cli->waitForConnected()){
//            return false;
//        }
//       ret =  _sock_cli->bind(QHostAddress::AnyIPv4, _server_port);
        _hanlder.InitSocket(_server_ip.toStdString().c_str(), _server_port);
    }
    else{                                                              //作服务端
        ret = _sock_cli->bind(port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    }

    ret = connect(_sock_cli, &QUdpSocket::readyRead, this, &UdpNetHandler::receive_data);

    return true;
}

bool UdpNetHandler::stop_service()
{
    bool ret = true;
    if(_sock_cli){
        if(_b_cli){
            _sock_cli->disconnectFromHost();
            ret =  _sock_cli->waitForDisconnected();
        }

        _sock_cli->close();
        _sock_cli->deleteLater();
        _sock_cli = nullptr;
    }

    return ret;
}

bool UdpNetHandler::send_data(const char* data, int len, const char* ip, int port)
{
    //客户端发送
    if(_b_cli){
        //int snd_len = _sock_cli->writeDatagram(data,  len,  QHostAddress(_server_ip), _server_port);
        int snd_len = _hanlder.Send(ip, port, data, len);
        return snd_len == len;
    }

    //服务端发送
    if(!ip){
        return false;
    }

    IpPortMap::const_iterator it = _cli_conn_tbl.find(ip);
    if(it == _cli_conn_tbl.cend()){
        return false;
    }

    uint16_t snd_port = it.value();
    return _sock_cli->writeDatagram(data,  len,  QHostAddress(ip), snd_port) == len;
}

bool UdpNetHandler::register_rcv_callback(RcvHandler handler)
{
    _rcv_handler = handler;
    _hanlder.RegisterRcvCallback(handler);
    return true;
}

void UdpNetHandler::receive_data()
{
    quint16 sender_port;
    QHostAddress sender;
    QByteArray datagram;

    while (_sock_cli->hasPendingDatagrams()) {
         datagram.resize(_sock_cli->pendingDatagramSize());
         _sock_cli->readDatagram(datagram.data(), datagram.size(), &sender, &sender_port);

         //记录新的客户端ip及端口
         if(!_b_cli){
             QString cli_ip = sender.toString();
             IpPortMap::const_iterator it = _cli_conn_tbl.find(cli_ip);
             if(it != _cli_conn_tbl.cend()){
                  _cli_conn_tbl[cli_ip] = sender_port;
                  emit new_net_handler(cli_ip,  sender_port, eProtocol_udp, this);
             }
         }

         //处理接收的数据
         _rcv_handler(datagram.toStdString().c_str(), datagram.length());
     }
}




