#ifndef UDP_HANDLER_H
#define UDP_HANDLER_H

#include <QMap>
#include <QThread>
#include <QUdpSocket>
#include "net_handler.h"
#include "sockethandler.h"

class UdpNetHandler : public NetHandlerBase
{
    Q_OBJECT
public:
    UdpNetHandler();
    virtual ~UdpNetHandler();

public:
    bool start_service(const char* ip, uint16_t port, uint16_t dev_port, bool cli = true);
    bool stop_service();

    bool send_data(const char* data, int len, const char* ip = nullptr, int port = 0);
    bool register_rcv_callback(RcvHandler handler);

signals:
    bool new_net_handler(QString ip, quint16 port, EProtocolType type, NetHandlerBase* handler);

public slots:
    void receive_data();
    void new_net_handler_slot(QString ip, quint16 port);

private:
    typedef QMap<QString, uint16_t> IpPortMap;
    IpPortMap _cli_conn_tbl;

private:
    bool  _b_cli;                          //是否客户端标识
    QString _server_ip;
    uint16_t _server_port;

    QUdpSocket* _sock_cli;
    SocketHandler _hanlder;


};

#endif // UDP_HANDLER_H
