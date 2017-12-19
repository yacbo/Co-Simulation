#ifndef CLIENT_PROXY_H
#define CLIENT_PROXY_H

#include <QObject>
#include "pg_rtui_def.h"
#include "data_queue.h"
#include "comm_table.h"
#include "powerhandler.h"
#include "application_layer.h"
#include "decisionalth.h"

class client_proxy : public QObject
{
    Q_OBJECT
public:
    client_proxy(application_layer* parent, const QString& sbs_ip, quint16 port,  ESimDevType type = eSimDev_undef);
    virtual ~client_proxy();

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
    void set_appl_proc_oper(ApplProcedureOper* oper);

    //当仿真软件与客户代理通过网络通信时，客户代理作服务器端
    //dev_ip: 仿真软件所在主机ip
    //lis_port: 监听端口
    bool start_sock_service(const QString& comm_ip, uint16_t comm_port, EProtocolType type, uint16_t dev_port, uint16_t bs_port, const QString& host_ip, uint16_t host_port);

private:
    void rcv_lower_thread();
    const char* get_local_ip_addr();
    void rcv_upper_msg_callback(const char* data, int len);
    void snd_upper_to_comm();

public:
    bool fetch_power_cfg_param(const DataXmlVec& vec);
    bool map_power_comm_sim_data(const BusInfor* bus, int num, UnionSimDatVec& ud);
    string stream_power_sim_data(const UnionSimDatVec& data);

    void calc_power_appl_data(const UnionSimDatVec& data, DataXmlVec& vec);
    void reset_power_dginfo(DGInfor* infor, int num);
    void replace_power_sim_data(UnionSimData* data);

private:
    //从上层发下来.
    void handle_css(EPGRTUIType type,  const char* data, int len);

    void handle_power(ApplMessage* msg);
    void handle_power_appl(ApplMessage* msg);
    void handle_communication(ApplMessage* msg);
    void handle_comm_power(ApplMessage* msg);
    void handle_comm_power_appl(ApplMessage* msg);

    void handle_interoper(ApplMessage* msg);

    void handle_sim_cmd(ApplMessage* msg);
    void handle_comm_cfg_param(ApplMessage* msg);
    int parse_comm_cfg_param(ApplMessage* msg, char* data);
    void handle_power_cfg_param(ApplMessage* msg);

    void handle_msg(ApplMessage* msg);

private:
    bool _quit;
    std::thread _rcv_th;
    MsDeviceTable _comm_tbl;
    bool _rcv_comm_data_enabled;

    string _sbs_ip;
    uint16_t _sbs_port;
    string _local_ip;
    ESimDevType _dev_type;

private:
    long _expect_msg_type;
    long _expect_proc_type;

    bool _power_sim_started;
    bool _power_init_success;
    PowerConfParam _power_conf_param;
    IntPairMap _bus_comm_id_tbl;
    application_layer* _appl_layer;

private:
    EProxyState _proxy_status;                                        //客户代理状态
    ApplProcedureOper* _appl_proc_oper;                       //应用过程实例

private:
    SockUtil* _sock_util_ptr;
    PowerHandler* _power_handler;                               //电力仿真处理器
    DecisionAlth* _decision_alth;

    UnionSimDatVec _union_sim_dat_snd_vec;
    UnionSimDatVec _union_sim_dat_rcv_vec;
    DblVec _dbl_vec;

private:
    SockUtil* _sock_remote_ptr;
    CommConfParam _comm_conf_param;

private:
    data_queue<ApplMessage*> _proxy_msg_que;
};

#endif // CLIENT_PROXY_H
