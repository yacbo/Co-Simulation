#ifndef APPLICATION_LAYER_H
#define APPLICATION_LAYER_H

#include <QObject>
#include "data_queue.h"
#include "comm_table.h"
#include "session_layer.h"
#include "pg_rtui_def.h"

class client_proxy;
class application_layer : public QObject
{
    Q_OBJECT
public:
    application_layer();
    virtual ~application_layer();

signals:
    void snd_upper_signal(ApplMessage* msg);     //向上层发送数据
    void snd_lower_signal(QDomDocument* doc, QString dst_ip, quint16 port);
    void snd_lower_register_signal(int ss_id, QString sbs_ip, int sbs_port,  int proto_type, int dev_port);
    void snd_lower_unregister_signal(int ss_id, QString sbs_ip, quint16 port);

signals:
    void ack_comm_cfg_param_signal(PG_RTUI_Base* data);
    void login_signal(QString dev_name, QString dev_ip, int dev_port, bool login = true);
    void progress_log_signal(QString log);

private slots:
    void rcv_lower_slots(ApplMessage* msg);            //接收下层数据
    void rcv_lower_type_id_slots(IntMap* type_ids);

public:
    bool config_communication(const char* comm_ip, uint16_t comm_port, uint16_t bss_port, int proto_type, const char* host_ip, uint16_t host_port);
    bool config_sim_device(const char* dev_name, int ss_id,  const char* dll_path);
    bool config_sim_device(const char* dev_name, int ss_id,  const char* dev_ip, uint16_t dev_port, EProtocolType type);
    bool register_device(const char* dev_name, int ss_id,  const char* sbs_ip, uint16_t sbs_port, EProtocolType type);
    bool unregister(const char* dev_name, int ss_id, const char* sbs_ip);

public:
    //comm sim event config
    bool config_comm_sim_event();

    //comm parameter config PG_RTUI
    bool config_comm_param(const PG_RTUI_Base* param);

    //union sim parameter config
    bool config_union_sim_param(const UnionSimConfParam* param);

    //power sim parameter config
    bool config_power_sim_param(const PowerConfParam* cfg_param);

    //comm sim parameter config
    bool config_comm_sim_param(const CommConfParam* cfg_param);

    //sim cmd
    bool exec_sim_oper_cmd(int cmd);

public:
    void quit();
    void start_rcv_thread();
    void register_lower_layer(const session_layer* session_layer_ptr, const network_layer* net_layer_ptr);
    const IntMap& get_dev_id_map();

private:
    void rcv_lower_thread();
    void handle_msg(ApplMessage* msg);
    void handle_login(ApplMessage* msg);
    void handle_logout(ApplMessage* msg);
    void handle_interoper(ApplMessage* msg);
    void handle_union_sim_param(ApplMessage* msg);
    void handle_ack_comm_cfg_param(ApplMessage* msg);

    void notify_ui_msg(const QString& info);

private:
    client_proxy* manager_proxy(int i2u, int u2i, bool bcreate = true);
    client_proxy* init_proxy(int i2u, int u2i);

    bool parse_event_dll(const char* dev_name, const char* dll_path);

private:
    bool _quit;
    std::thread _rcv_th;
    ESimDevType _dev_type;
    UnionSimConfParam _union_sim_conf_param;

private:
    string _sbs_ip;
    uint16_t _sbs_port;
    MsDeviceTable _comm_tbl;
    network_layer* _net_layer_ptr;

private:
    typedef data_queue<ApplMessage*> ApplMsgQue;
    ApplMsgQue _snd_upper_que;

private:
    data_queue<ApplMessage*> _appl_msg_que;

private:
    typedef QMap<client_proxy*, IntPair> ClientProxyIdMap;
    ClientProxyIdMap _cli_proxy_tbl;                                     //客户代理表

    MsDeviceMap _dev_tbl;                                                   //器件表

private:
    IntMap _dev_type_id_tbl;
};

#endif // APPLICATION_LAYER_H
