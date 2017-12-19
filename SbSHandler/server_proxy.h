#ifndef CLIENT_PROXY_H
#define CLIENT_PROXY_H

#include <QObject>
#include "data_queue.h"
#include "comm_table.h"
#include "application_layer.h"

class server_proxy : public QObject
{
    Q_OBJECT
public:
    server_proxy();
    virtual ~server_proxy();

signals:
    void snd_lower_signal(QDomDocument* doc, QString dst_ip, quint16 port);            //向下层发送数据
    void ready_rcv_signal(ApplMessage* msg);
    void progress_log_signal(QString log);

public slots:
    void rcv_lower_slots(ApplMessage* msg);

public:
    void quit();
    void start_rcv_thread();
    void register_lower_layer(const network_layer* net_layer_ptr);
    EProxyState query_state();

private:
    void rcv_lower_thread();
    void handle_msg(ApplMessage* msg);

private:
    bool _quit;
    std::thread _rcv_th;
    EProxyState _proxy_status;

private:
    data_queue<ApplMessage*> _proxy_msg_que;
};

#endif // CLIENT_PROXY_H
