#ifndef APPLICATION_LAYER_H
#define APPLICATION_LAYER_H

#include <QObject>
#include "data_queue.h"
#include "comm_table.h"
#include "session_layer.h"

class server_proxy;
class application_layer : public QObject
{
    Q_OBJECT
public:
    application_layer();
    virtual ~application_layer();

signals:
    void snd_lower_signal(QDomDocument* doc, QString dst_ip, quint16 port);

    void ui_login_signal(QString dev_name, QString dev_ip, int dev_port, bool login);
    void progress_log_signal(QString log);

private slots:
    void rcv_lower_slots(ApplMessage* msg);            //接收下层数据
    void rcv_lower_type_id_slots(int type, int id);

public:
    void quit();
    void start_rcv_thread();
    void set_synchronize_type(ESynchronizeType type);
    void register_lower_layer(const session_layer* session_layer_ptr, const network_layer* net_layer_ptr);

private:
    void rcv_lower_thread();
    void handle_msg(ApplMessage* msg);

    void handle_login(ApplMessage* msg);
    void handle_logout(ApplMessage* msg);
    void handle_session(ApplMessage* msg);

    void handle_sim_cmd(ApplMessage* msg);
    void handle_cfg_sim_param(ApplMessage* msg);

    server_proxy* manager_proxy(int i2u, int u2i, bool bcreate = true);
    server_proxy* init_proxy(int i2u, int u2i);

    ESimDevType query_dev_type(int dev_id);

private:
    bool _quit;
    std::thread _rcv_th;
    network_layer* _net_layer_ptr;
    ESynchronizeType _synch_type;
    UnionSimConfParam _sim_conf_param;

private:
    typedef data_queue<ApplMessage*> ApplMsgQue;
    ApplMsgQue _snd_upper_que;

private:
    data_queue<ApplMessage*> _appl_msg_que;

private:
    SbsDeviceMap _dev_tbl;                                                     //器件表
    IntMap _dev_type_id_tbl;                                                  //器件type-id表

    typedef QMap<server_proxy*, IntPair> ServerProxyIdMap;
    ServerProxyIdMap _server_proxy_tbl;                               //服务代理表
};

#endif // APPLICATION_LAYER_H
