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

signals:
    void new_net_handler(QString ip, quint16 port);

public:
    bool InitSocket(const char* server_ip, int server_port, int dev_port);
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
};

#endif // SOCKETHANDLER_H
