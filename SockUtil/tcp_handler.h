#ifndef TCP_HANDLER_H
#define TCP_HANDLER_H

#include "net_handler.h"
#include <QTcpSocket>
#include <QTcpServer>

class TcpNetHandler : public NetHandlerBase
{
    Q_OBJECT
public:
    TcpNetHandler();
    TcpNetHandler(QTcpSocket* sock, RcvHandler handler, bool cli = true);
    virtual ~TcpNetHandler();

public:
    bool start_service(const char* ip, uint16_t port, uint16_t dev_port, bool cli = true);
    bool stop_service();

    bool send_data(const char* data, int len, const char* ip = nullptr, int port = 0);
    bool register_rcv_callback(RcvHandler handler);

signals:
    bool new_net_handler(QString ip, quint16 port, EProtocolType type, NetHandlerBase* handler);

private slots:
    void receive_data();
    void accept_connection();
    void disconnected();

private:
    void init_signal_slots(QTcpSocket* sign_obj, TcpNetHandler* slot_obj);

private:
    bool _b_cli;                           //是否客户端标识
    bool _b_connected;             //是否处于连接状态标识

private:
    uint16_t _server_port;
    const char* _server_ip;

    QTcpServer* _sock_svr;
    QTcpSocket* _sock_cli;

private:
    QByteArray _rcv_buf;
    uint16_t _need_rcv_xml_len;
};

#endif // TCP_HANDLER_H
