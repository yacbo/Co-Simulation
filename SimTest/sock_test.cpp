
#include <string>
#include <iostream>
#include <QDebug>
#include "sock_util.h"
#include "header_test.h"

SockUtil util;
void rcv_handler(const char* data, int len)
{
     qDebug() << "data:" << data <<" " <<"len:" << len;
     util.send_data("192.168.2.227", 10000, data, len);
}

void start_server(const char* ip, uint16_t port, EProtocolType protol)
{
    util.register_rcv_callback(rcv_handler);
    bool ret = util.start_trans_service(QString(ip), port, protol, false);
}

void start_client(const char* ip, uint16_t port, EProtocolType protol)
{
    bool ret = true;
     util.register_rcv_callback(rcv_handler);
     ret = util.start_trans_service(QString(ip), port, protol, 0);

     //char data[] = {"hello server"};
     unsigned char data[14];
     data[0] = 0xea;
     data[1] = 0x02;
     data[2] = 0xa8;
     data[3] = 0xc0;
     data[4] = 0xdc;
     data[5] = 0xff;
     data[6] = 0x07;
     data[7] = 0x00;
     data[8] = 0x0e;
     data[9] = 0x00;
     data[10] = 0x0e;
     data[11] = 0x00;
     data[12] = 0x1a;
     data[13] = 0x00;

     util.send_data(ip, 10000, (char*)data, 14);
}

void sock_tcp_test(bool cli)
{
    const char* ip = "127.0.0.1";
    EProtocolType proto = eProtocol_tcp;

    if(cli){
        start_client(ip, 19099, proto);
        qDebug() << "client started";
    }
    else{
        start_server(ip, 19099, proto);
        qDebug() << "server started";
    }
}

void sock_udp_test(bool cli)
{
    const char* ip = "192.168.2.101";
    EProtocolType proto = eProtocol_udp;

    if(cli){
        start_client(ip, 5132, proto);
        qDebug() << "client started";
    }
    else{
        start_server(ip, 19099, proto);
        qDebug() << "server started";
    }
}
