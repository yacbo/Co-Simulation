#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H

#include <QObject>
#include <functional>
#include <WinSock.h>

typedef std::function<void(const char*, int)> RcvCallback;

class SocketHandler : public QObject
{
    Q_OBJECT
public:
    SocketHandler();
    ~SocketHandler();

signals:
    void new_net_handler(QString ip, quint16 port);

public:
    void Quit();
    bool InitSocket(const char* server_ip, int server_port, int dev_port, bool brcv_pg_rtui = false);
    long Send(const char* ip, int port, const char* data, int len);
    void RegisterRcvCallback(RcvCallback cb);

private:
    void RcvThread();

private:
    SOCKET _sock_cli;
    SOCKADDR_IN _src_addr;
    SOCKADDR_IN _local_addr;
    RcvCallback _rcv_callback;

private:
    QString _server_ip;
    quint16 _server_port;
    quint16 _dev_port;

private:
    bool _b_quit;
    bool _b_rcv_pg_rtui;
    QByteArray _rcv_buf;
    uint16_t _need_rcv_xml_len;
};

#endif // SOCKETHANDLER_H
