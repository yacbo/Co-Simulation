#ifndef MS_HANDLER_H
#define MS_HANDLER_H

#include <QObject>
#include "pg_rtui_def.h"
#include "comm_type_def.h"
#include "mshandler_global.h"

class network_layer;
class session_layer;
class application_layer;
class MSHANDLERSHARED_EXPORT MsHandler : public QObject
{
    Q_OBJECT

public:
    MsHandler();
    virtual ~MsHandler();

signals:
    void ack_comm_cfg_param_signal(PG_RTUI_Base* data);
    void login_signal(QString dev_name, QString dev_ip, uint16_t dev_port, bool login);
    void progress_log_signal(QString log);

public:
    bool config_communication(const char* comm_ip, uint16_t comm_port, uint16_t bss_port, int proto_type, const char* host_ip, uint16_t host_port);

    bool config_device(const char* dev_name, uint32_t ss_id,  const char* dll_path);
    //dev_ip: 仿真软件所在主机ip
    bool config_device(const char* dev_name, uint32_t ss_id,  const char* dev_ip, uint16_t dev_port, int type);
    bool register_device(const char* dev_name,  uint32_t ss_id, const char* sbs_ip, uint16_t sbs_port, int type);
    bool unregister(const char* dev_name, uint32_t ss_id, const char* sbs_ip);

    bool test_send_xml(const QString& dst_ip, int port, const char* packet, int len);

public:
    //comm parameter config, PG_RTUI
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
    QString get_local_ip();
    const IntMap& get_dev_id_map();

private:
    network_layer* _net_layer_ptr;
    session_layer* _sess_layer_ptr;
    application_layer* _appl_layer_ptr;
};

#endif // MS_HANDLER_H
