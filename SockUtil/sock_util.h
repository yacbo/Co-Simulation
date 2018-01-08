#ifndef SOCK_UTIL_H
#define SOCK_UTIL_H

#include <QMap>
#include <QObject>
#include "net_handler.h"
#include "sockutil_global.h"

class SOCKUTILSHARED_EXPORT SockUtil : public QObject
{
    Q_OBJECT
public:
    SockUtil();
    virtual ~SockUtil();

public slots:
    void add_new_net_handler(QString ip,  quint16 port, EProtocolType type, NetHandlerBase* handler);

public:
    /**
  * @brief  注册接收回调函数，此操作必须先于start_trans_service操作，网络层共用此回调函数
  * @param handler: 接收回调函数指针
  * @return void
  */
    void register_rcv_callback(RcvHandler handler);

    /**
  * @brief 启动网络传输服务
  * @param ip: 对tcp，此参数指服务器ip，对udp，此参数指目的器件ip
  * @param port: 对tcp，此参数指服务器端口，对udp，此参数指本机通信端口
  * @param protocol: tcp/udp
  * @param cli: true指创建客户端，false指创建服务器端
  * @return true/false: 启动成功或失败
  */
    bool start_trans_service(const QString& ip, uint16_t port, EProtocolType protocol, uint16_t dev_port, bool cli = true);
    bool stop_trans_service(const QString& ip, quint16 port, EProtocolType protocol);
    uint8_t stop_trans_service(const QString& ip, quint16 port);
    uint8_t stop_trans_service();
    bool stop_sbs_service();

    uint8_t query_trans_service_count(const QString& ip, quint16 port);
    uint8_t query_trans_service_count();

    /* 查询本地IP */
    static string query_local_ip();

    /* 发送数据 */
    bool send_data(const QString& dst_ip, quint16 port, const char* data, int len = -1);

public:
    bool insert_net_handler(const QString& dst_ip, quint16 port, EProtocolType protocol, NetHandlerBase* hndl);
    NetHandlerBase* query_net_handler(const QString& dst_ip, quint16 port, EProtocolType protocol);

private:
    typedef QMap<EProtocolType, NetHandlerBase*>ProtoHandlerMap;
    typedef QMap<QString, ProtoHandlerMap> NetSockMap;

    //仿真器件与客户端套接字映射集合
    NetSockMap _device_sock_map;

private:
    static QString _local_ip;
    RcvHandler _rcv_handler;
    EProtocolType _protocol_type;

    NetHandlerBase* _sock_server;
};

#endif // SOCK_UTIL_H
