
#include <QtNetwork>
#include "sock_util.h"
#include "udp_handler.h"
#include "tcp_handler.h"

SockUtil::SockUtil()
{
   _tcp_sock_server = nullptr;
   _protocol_type = eProtocol_undef;
   _rcv_handler = nullptr;
}

SockUtil::~SockUtil()
{
    if(_tcp_sock_server){
        _tcp_sock_server->stop_service();
        delete _tcp_sock_server;
        _tcp_sock_server = nullptr;
    }

    NetSockMap::iterator it_o = _device_sock_map.begin();
    for(; it_o != _device_sock_map.end(); ++it_o){
        ProtoHandlerMap::iterator it_i = it_o->begin();
        for(; it_i != it_o->end(); ++it_i){
            delete it_i.value();
        }
    }

    _device_sock_map.clear();
}

void SockUtil::register_rcv_callback(RcvHandler handler)
{
    _rcv_handler = handler;
}

bool SockUtil::start_trans_service(const QString& ip, uint16_t port, EProtocolType protocol, uint16_t dev_port, bool cli)
{
    if(!_rcv_handler || ip.isEmpty()){
        return false;
    }

    //判断传输服务是否已经启动，若启动直接返回
    NetHandlerBase* handler_ptr  = query_net_handler(ip, port, protocol);
    if(handler_ptr){
        return true;
    }

    //按协议创建服务
    switch (protocol) {
    case eProtocol_tcp: handler_ptr = new TcpNetHandler(); break;
    case eProtocol_udp: handler_ptr = new UdpNetHandler(); break;
    default: break;
    }

    //启动传输服务
    if(!handler_ptr->start_service(ip.toStdString().c_str(), port, dev_port, cli)){
        delete handler_ptr;
        return false;
    }

    //记录配置的协议
    _protocol_type = protocol;

    //注册接收回调函数
    handler_ptr->register_rcv_callback(_rcv_handler);

    //如果采用tcp协议并作为服务器启动，则注意存储来自客户端的连接
    if(!cli){
        if(protocol == eProtocol_tcp){
            connect((TcpNetHandler*)handler_ptr,  &TcpNetHandler::new_net_handler, this, &SockUtil::add_new_net_handler);
            _tcp_sock_server = handler_ptr;
        }
        else if(protocol == eProtocol_udp){
            connect((UdpNetHandler*)handler_ptr,  &UdpNetHandler::new_net_handler, this, &SockUtil::add_new_net_handler);
        }
    }
    else{  //存储socket服务, tcp客户端或udp通信
        QString id = QString("%1:%2").arg(ip).arg(port);
        _device_sock_map[id][protocol] = handler_ptr;
    }

    return true;
}

void SockUtil::add_new_net_handler(QString ip, quint16 port, EProtocolType type, NetHandlerBase* handler)
{
    QString id = QString("%1:%2").arg(ip).arg(port);
    _device_sock_map[id][type] = handler;
}

bool SockUtil::stop_trans_service(const QString& ip, uint16_t port, EProtocolType protocol)
{
    if(ip == nullptr || protocol == eProtocol_undef){
        return false;
    }

    QString id = QString("%1:%2").arg(ip).arg(port);
    NetSockMap::iterator it_o = _device_sock_map.find(id);
    if(it_o == _device_sock_map.end() ){
        return false;
    }

    NetHandlerBase* handler_ptr = nullptr;
    ProtoHandlerMap::iterator it_i  = it_o.value().find(protocol);
    if(it_i != it_o.value().end()){
        handler_ptr = it_i.value();
    }

    delete handler_ptr;
    it_o.value().erase(it_i);

    return true;
}

uint8_t SockUtil::stop_trans_service(const QString& ip, uint16_t port)
{
    if(ip.isEmpty() || ip.isNull()){
        return 0;
    }

    QString id = QString("%1:%2").arg(ip).arg(port);
    NetSockMap::iterator it_o = _device_sock_map.find(id);
    if(it_o == _device_sock_map.end() ){
        return 0;
    }

    uint8_t count = 0;
    ProtoHandlerMap::iterator it_i  =  it_o.value().begin();
    for (; it_i != it_o.value().end(); ++it_i){
         if( it_i.value()->stop_service()){
             ++count;
         }
    }

    _device_sock_map.erase(it_o);

    return count;
}

uint8_t SockUtil::stop_trans_service()
{
    uint8_t count = 0;
    NetSockMap::iterator it_o = _device_sock_map.begin();
    for(; it_o != _device_sock_map.end(); ++it_o){
        QStringList id = it_o.key().split(':');
        QString  ip = id.at(0);
        QString port = id.at(1);
        count += stop_trans_service(ip.toStdString().c_str(), port.toUShort());
    }

    _device_sock_map.clear();

    return count;
}

bool SockUtil::stop_sbs_service()
{
    TcpNetHandler* hndl = (TcpNetHandler*)_tcp_sock_server;
    return hndl->stop_service();
}

uint8_t SockUtil::query_trans_service_count(const QString& ip, uint16_t port)
{
    if(ip.isNull() || ip.isEmpty()){
        return 0;
    }

    QString id = QString("%1:%2").arg(ip).arg(port);
    NetSockMap::iterator it_o = _device_sock_map.find(id);
    if(it_o == _device_sock_map.end() ){
        return 0;
    }

    return it_o.value().size();
}

uint8_t SockUtil::query_trans_service_count()
{
    uint8_t count = 0;
    NetSockMap::iterator it_o = _device_sock_map.begin();
    for(; it_o != _device_sock_map.end(); ++it_o){
        count += it_o.value().size();
    }

    return count;
}

QString SockUtil::_local_ip;
string SockUtil::query_local_ip()
{
    if(!_local_ip.isEmpty()){
        return _local_ip.toStdString().c_str();
    }

    const QString virtual_netcard1 = "00:50:56:C0:00:01";
    const QString virtual_netcard2 = "00:50:56:C0:00:08";
    QList<QNetworkInterface> interfaceList = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface interfaceItem, interfaceList){
        if(interfaceItem.flags().testFlag(QNetworkInterface::IsUp) &&
            interfaceItem.flags().testFlag(QNetworkInterface::IsRunning) &&
            interfaceItem.flags().testFlag(QNetworkInterface::CanBroadcast) &&
            interfaceItem.flags().testFlag(QNetworkInterface::CanMulticast) &&
            !interfaceItem.flags().testFlag(QNetworkInterface::IsLoopBack) &&
            interfaceItem.hardwareAddress() != virtual_netcard1 &&
            interfaceItem.hardwareAddress() != virtual_netcard2){
            QList<QNetworkAddressEntry> entryList=interfaceItem.addressEntries();
            foreach(QNetworkAddressEntry entry, entryList){
                if(entry.ip().protocol()==QAbstractSocket::IPv4Protocol){
                    _local_ip = entry.ip().toString();
                    goto over;
                }
            }
        }
    }

over:
    if (_local_ip.isEmpty()){
         _local_ip = QHostAddress(QHostAddress::LocalHost).toString();
    }

    return _local_ip.toStdString().c_str();
}

bool SockUtil::insert_net_handler(const QString& ip, quint16 port, EProtocolType protocol, NetHandlerBase* handler)
{
    if(!handler){
        return false;
    }

    QString id = QString("%1:%2").arg(ip).arg(port);
    _device_sock_map[id][protocol] = handler;

    return true;
}

NetHandlerBase* SockUtil::query_net_handler(const QString& dst_ip, uint16_t port, EProtocolType protocol)
{
    if(dst_ip == nullptr || protocol == eProtocol_undef){
        return nullptr;
    }

    QString id = QString("%1:%2").arg(dst_ip).arg(port);
    NetSockMap::iterator it_o = _device_sock_map.find(id);
    if(it_o == _device_sock_map.end() ){
        return nullptr;
    }

    NetHandlerBase* handler_ptr = nullptr;
    ProtoHandlerMap::iterator it_i  = it_o.value().find(protocol);
    if(it_i != it_o.value().end()){
        handler_ptr = it_i.value();
    }

    return handler_ptr;
}

bool SockUtil::send_data(const QString& dst_ip, uint16_t port, const char* data, int len)
{
    if(_protocol_type == eProtocol_undef){
        return false;
    }

    NetHandlerBase* handler_ptr = query_net_handler(dst_ip, port, _protocol_type);
    if(!handler_ptr){
        return false;
    }

    if(len == -1){
        len = strlen(data) + 1;
    }

    bool ret = handler_ptr->send_data(data, len, dst_ip.toStdString().c_str(), port);
    return ret;
}
