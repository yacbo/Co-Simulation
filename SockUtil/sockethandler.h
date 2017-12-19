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

public:
    bool InitSocket(const char* server_ip, int port);
    long Send(const char* ip, int port, const char* data, int len);
    void RegisterRcvCallback(RcvCallback cb);

private:
    void RcvThread();

private:
    SOCKET _sock_cli;
    SOCKADDR_IN _server_addr;
    SOCKADDR_IN _sender_addr;
    RcvCallback _rcv_callback;
};

#endif // SOCKETHANDLER_H
