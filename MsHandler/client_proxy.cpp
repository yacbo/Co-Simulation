#include <sstream>
#include <functional>
#include <QNetworkInterface>
#include "client_proxy.h"
#include "sock_util.h"
#include "xml_util.h"
#include "log_util.h"
#include "pg_rtui_def.h"

client_proxy::client_proxy(application_layer* parent, const QString& sbs_ip, quint16 port, ESimDevType type)
{
    _proxy_status = eProxyState_idle;
    _rcv_comm_data_enabled = false;

    _quit = false;
    _appl_layer = parent;

    _sbs_port = port;
    _sbs_ip = sbs_ip.toStdString();

    _dev_type = type;
    _sock_util_ptr = nullptr;
    _sock_remote_ptr = nullptr;
    _appl_proc_oper = nullptr;
    _power_handler  = nullptr;
    _power_init_success = false;
    _power_sim_started = false;
    _decision_alth = nullptr;
    _his_record_mgr = nullptr;

    _input_info = nullptr;
    _result_info = nullptr;

    if(type == eSimDev_communication){
        _sock_util_ptr = new SockUtil();
        _sock_util_ptr->register_rcv_callback(std::bind(&client_proxy::rcv_upper_msg_callback, this, std::placeholders::_1, std::placeholders::_2));

        _sock_remote_ptr = new SockUtil();
        _sock_remote_ptr->register_rcv_callback(std::bind(&client_proxy::rcv_upper_msg_callback, this, std::placeholders::_1, std::placeholders::_2));

        _expect_msg_type = eMessage_confirm;
        _expect_proc_type = eSubProcedure_session_begin;
    }
    else if(type == eSimDev_power){
        _expect_msg_type = eSimCmd_start_sim;
        _expect_proc_type = eSubProcedure_sim_cmd;
        _power_handler = new PowerHandler();
    }
    else if(type == eSimDev_power_appl){
        _his_record_mgr = new HisRecordMgr();
    }

    _local_ip = SockUtil::query_local_ip();
    start_rcv_thread();

    QString info = QString("client_proxy: %1 client proxy created").arg(DevNamesSet[type]);
    qInfo(info.toStdString().c_str());
    emit progress_log_signal(info);
}

client_proxy::~client_proxy()
{
    if(_sock_util_ptr){
        _sock_util_ptr->stop_trans_service(_comm_tbl._dev_ip.c_str(), _comm_tbl._dev_port);
        delete _sock_util_ptr;
    }

    if(_sock_remote_ptr){
        _sock_remote_ptr->stop_trans_service();
        delete _sock_remote_ptr;
    }

    if(_power_handler){
        delete _power_handler;
    }

    if(_his_record_mgr){
        delete _his_record_mgr;
    }

    if(_input_info){
        delete[] _input_info;
    }

    if(_result_info){
        delete[] _result_info;
    }
}

void client_proxy::quit()
{
    _quit = true;
    _proxy_msg_que.set_completed_flag();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void client_proxy::register_lower_layer(const network_layer* net_layer_ptr)
{
    //向下层发
    connect(this, &client_proxy::snd_lower_signal, net_layer_ptr, &network_layer::rcv_upper_slots, Qt::UniqueConnection);
}

void client_proxy::set_appl_proc_oper(ApplProcedureOper* oper)
{
    _appl_proc_oper = oper;
}

//void client_proxy::rcv_upper_msg_callback(const char* data, int len)
//{
//    _union_sim_dat_rcv_vec = _union_sim_dat_snd_vec;

//    IntMap dev_type_id_tbl = _appl_layer->get_dev_id_map();
//    int ss_id = dev_type_id_tbl[eSimDev_communication];
//    int ps_id = dev_type_id_tbl[eSimDev_power_appl];
//    QDomDocument* doc = XmlUtil::generate_session_xml(ss_id, ps_id, DevNamesSet[eSimDev_power_appl], eSubProcedure_session_begin, eMessage_request);
//    emit snd_lower_signal(doc, _sbs_ip.c_str(), _sbs_port);

//    QString info = QString("client_proxy: rcv_upper_msg_callback,  items: %1").arg(_union_sim_dat_rcv_vec.size());
//    qInfo(info.toStdString().c_str());
//    emit progress_log_signal(info);
//}

void client_proxy::rcv_upper_msg_callback(const char* data, int len)
{
    const PG_RTUI_Base* header = (PG_RTUI_Base*)data;
    long type = header->type;

    QString info = QString("client_proxy: rcv communication sim data, len:%1, data:%2").arg(len).arg(data);
    emit progress_log_signal(info);

    if(type == eCommCmd_start_send_data || type == eCommCmd_stop_send_data){
        _rcv_comm_data_enabled = type == eCommCmd_start_send_data;
        if(type == eCommCmd_stop_send_data){
            IntMap dev_type_id_tbl = _appl_layer->get_dev_id_map();
            int ss_id = dev_type_id_tbl[eSimDev_communication];
            int ps_id = dev_type_id_tbl[eSimDev_power_appl];
            QDomDocument* doc = XmlUtil::generate_session_xml(ss_id, ps_id, DevNamesSet[eSimDev_power_appl], eSubProcedure_session_begin, eMessage_request);
            emit snd_lower_signal(doc, _sbs_ip.c_str(), _sbs_port);

            QString info = QString("client_proxy: rcv all the %1 communication sim data items").arg(_union_sim_dat_rcv_vec.size());
            qInfo(info.toStdString().c_str());
            emit progress_log_signal(info);
        }
        return;
    }

    if(type == ePG_sim_interoper_data){
        if(_rcv_comm_data_enabled){
            const UnionSimData* dat = (UnionSimData*)(data + sizeof(PG_RTUI_Base));
            _union_sim_dat_rcv_vec.push_back(*dat);
        }
    }
    else{
        handle_css((EPGRTUIType)header->type, data, header->length);
    }

//    if(type == ePG_sim_interoper_data){
//        UnionSimData* dat = (UnionSimData*)(data + sizeof(PG_RTUI_Base));
//        replace_power_sim_data(dat);
//        _union_sim_dat_rcv_vec.push_back(*dat);

//        if(_union_sim_dat_rcv_vec.size() == _union_sim_dat_snd_vec.size()){
//            IntMap dev_type_id_tbl = _appl_layer->get_dev_id_map();
//            int ss_id = dev_type_id_tbl[eSimDev_communication];
//            int ps_id = dev_type_id_tbl[eSimDev_power_appl];
//            QDomDocument* doc = XmlUtil::generate_session_xml(ss_id, ps_id, DevNamesSet[eSimDev_power_appl], eSubProcedure_session_begin, eMessage_request);
//            emit snd_lower_signal(doc, _sbs_ip.c_str(), _sbs_port);

//            QString info = QString("client_proxy: rcv all the %1 communication sim data items").arg(_union_sim_dat_rcv_vec.size());
//            qInfo(info.toStdString().c_str());
//            emit progress_log_signal(info);
//        }
//    }
//    else{
//        handle_css((EPGRTUIType)header->type, data, header->length);
//    }
}

void client_proxy::snd_upper_to_comm()
{
    char dat[1024] = {0};
    PG_RTUI_Base snd_dat_cmd;
    QHostAddress host(_local_ip.c_str());

    //send start snd data cmd to communication software
    snd_dat_cmd.clientAddr.ip_addr = host.toIPv4Address();
    snd_dat_cmd.clientAddr.port = _comm_tbl._business_port;
    snd_dat_cmd.type = eCommCmd_start_send_data;
    snd_dat_cmd.length = sizeof(snd_dat_cmd);
    memcpy(dat, &snd_dat_cmd, snd_dat_cmd.length);
    bool ret = _sock_util_ptr->send_data(_comm_tbl._dev_ip.c_str(), _comm_tbl._business_port, dat, snd_dat_cmd.length);
    QString tips = ret ? "successfully" : "failed";
    QString info = QString("client_proxy: snd start send power data cmd %1").arg(tips);
    emit progress_log_signal(info);

    if(!ret){
        return;
    }

    //send power sim data to communication software
    PG_RTUI_Base header;
    const int header_size = sizeof(header);
    header.clientAddr.ip_addr = host.toIPv4Address();
    header.clientAddr.port = _comm_tbl._business_port;
    header.type = ePG_sim_interoper_data;
    header.length = header_size + sizeof(UnionSimData);
    memcpy(dat, &header, header_size);

    int snd_items = 0;
    for(int i=0; i<_union_sim_dat_snd_vec.size(); ++i){
        memcpy(dat + header_size, &_union_sim_dat_snd_vec[i], sizeof(UnionSimData));
        if(!_sock_util_ptr->send_data(_comm_tbl._dev_ip.c_str(), _comm_tbl._business_port, dat, header.length)){
            QString info = QString("client_proxy: snd power sim data to communication successfully, len:%1, msg:%2").arg(header.length).arg(dat);
            emit progress_log_signal(info);
            break;
        }

        //sleep 20ms for each send
        QThread::msleep(20);
        ++snd_items;
    }

    //send stop snd data cmd to communication software
    snd_dat_cmd.type = eCommCmd_stop_send_data;
    memcpy(dat, &snd_dat_cmd, snd_dat_cmd.length);
    ret = _sock_util_ptr->send_data(_comm_tbl._dev_ip.c_str(), _comm_tbl._business_port, dat, snd_dat_cmd.length);
    tips = ret ? "successfully" : "failed";
    info = QString("client_proxy: snd stop send power data cmd %1").arg(tips);
    emit progress_log_signal(info);

    info = QString("client_proxy: snd_upper_to_comm, items: %1").arg(snd_items);
    emit progress_log_signal(info);
}

bool client_proxy::fetch_power_cfg_param(const DataXmlVec& vec)
{
    if(!XmlUtil::parse_PowerSimConfParam_xml(vec, _power_conf_param)){
        return false;
    }

    //power comm node map
    IntVec ids; string resp;
    std::istringstream parser(_power_conf_param.nodes_map);
    while(parser >> resp){
        ids.push_back(std::stoi(resp));
    }

    if(ids.size() % 3 != 0){
        QString info = QString("client_proxy: fetch_power_cfg_param failed, total node map items: %1").arg(ids.size());
        qInfo(info.toStdString().c_str());
        emit progress_log_signal(info);
        return false;
    }

    for(int i=0; i<ids.size(); i+=3){
       _bus_comm_id_tbl[ids[i]] = std::make_pair(ids[i+1], ids[i+2]);
    }

    return true;
}

bool client_proxy::map_power_comm_sim_data(UnionSimDatVec& ud)
{
    ud.clear();

    QDateTime date;
    double phy_time = date.currentDateTime().toSecsSinceEpoch();

    QString tips, info;
    UnionSimData data;
    bool b_map_success = true;

    if(_power_conf_param.result_type == ePowerData_businfor){
        for(int i=0; i<_power_conf_param.result_num; ++i){
            const PowerBusInforData& d = (const PowerBusInforData&)_result_info[i];
            IntPairMap::const_iterator it = _bus_comm_id_tbl.find(d.bus_id);
            if(it == _bus_comm_id_tbl.cend()){
                info = QString::number(d.bus_id);
                tips = QString("bus no: %1 not found").arg(info);
                b_map_success = false;
                break;
            }

            memset(&data, 0, sizeof(UnionSimData));
            data.realtime = phy_time;
            data.sim_time = d.cur_sim_time;

            const IntPair& comm = it->second;
            data.comm_dat.src_id = comm.first;
            data.comm_dat.dst_id = comm.second;
            memcpy(data.power_dat, &d, sizeof(PowerBusInforData));

            ud.push_back(data);
        }
    }

    tips = b_map_success ? "successfully" : tips;
    info = QString("client_proxy: map_power_comm_sim_data %1 items %2").arg(_power_conf_param.result_num).arg(tips);
    qInfo(info.toStdString().c_str());
    emit progress_log_signal(info);

    return b_map_success;
}

string client_proxy::stream_power_sim_data(const UnionSimDatVec& data)
{
    string stream;
    if(data.empty()){
        return stream;
    }

    QString info = QString("client_proxy: stream_power_sim_data");
    qInfo(info.toStdString().c_str());

    if(_power_conf_param.result_type == ePowerData_businfor){
        stream = std::to_string(data[0].sim_time);
        for(int i=0; i<data.size(); ++i){
            PowerBusInforData* d = (PowerBusInforData*)data[i].power_dat;
            stream += std::to_string(d->bus_volt) + " ";
            stream += std::to_string(d->bus_angle) + " ";
        }
    }

    return stream;
}

bool client_proxy::calc_power_appl_data(const UnionSimDatVec& data, DataXmlVec& vec)
{
    if(data.size() != _power_conf_param.result_num){
        return false;
    }

    double* dvg_ret = new double[_power_conf_param.result_num];
    memset(dvg_ret, 0, sizeof(double) * _power_conf_param.result_num);

    QString info = QString("client_proxy: calc_power_appl_data, items: %1").arg(_power_conf_param.result_num);
    emit progress_log_signal(info);

    ProcEventParam param;
    param.bus_num = _power_conf_param.result_num;
    param._in_out_info = stream_power_sim_data(data);
    _decision_alth->ApplReq_ProcedureRequestSendBefore(&param, dvg_ret);

    DblVec tmp;
    for(int i=0; i<_power_conf_param.result_num; ++i){
        tmp.push_back(dvg_ret[i]);
    }

    XmlUtil::generate_xml_power_appl_data(tmp, data, vec);
    delete[] dvg_ret;
}

void client_proxy::reset_power_input_data()
{
    if(_dbl_vec.size() != _union_sim_dat_rcv_vec.size()){
        return;
    }

    if(_power_conf_param.input_type == ePowerData_dginfor){
        int index = _dbl_vec.size() - _power_conf_param.input_num;
        for(int i=index; i<_dbl_vec.size(); ++i){
            PowerDGInforData& di = (PowerDGInforData&)_input_info[i];
            PowerBusInforData* bi = (PowerBusInforData*)_union_sim_dat_rcv_vec[i].power_dat;

            di.dv = _dbl_vec[i];
            di.bus_id = bi->bus_id;
            di.time_diff = _union_sim_dat_rcv_vec[i].comm_dat.trans_delay;
        }
    }
}

void client_proxy::replace_power_sim_data(UnionSimData* data)
{
    for(int i=0; i<_union_sim_dat_snd_vec.size(); ++i){
        const UnionSimData& d = _union_sim_dat_snd_vec[i];
        if(data->comm_dat.src_id == d.comm_dat.src_id && data->comm_dat.dst_id == d.comm_dat.dst_id){
            memcpy(&data->power_dat, &d.power_dat, sizeof(PowerSimData));
            break;
        }
    }
}

void client_proxy::handle_css(EPGRTUIType type,  const char* data, int len)
{
    QString info = QString("client_proxy: handle_css, type:%1, data:%2, len:%3").arg(type).arg(data).arg(len);
    qInfo(info.toStdString().c_str());
    emit progress_log_signal(info);

    IntMap dev_type_id_tbl = _appl_layer->get_dev_id_map();

    int ss_id = dev_type_id_tbl[eSimDev_communication];
    int ps_id = dev_type_id_tbl[eSimDev_sim_controller];

    const PG_RTUI_Base* d_base = (PG_RTUI_Base*)data;
    QDomDocument* doc = XmlUtil::create_PG_RTUI_xml(ss_id, ps_id, d_base);
    if(!doc){
        info = QString("client_proxy: handle_css, create_PG_RTUI_xml failed.");
        qInfo(info.toStdString().c_str());
        emit progress_log_signal(info);
        return;
    }

    emit snd_lower_signal(doc, QString::fromStdString(_sbs_ip), _sbs_port);
}

void client_proxy::rcv_lower_slots(ApplMessage* msg)
{
    _proxy_msg_que.push(msg);
}

bool client_proxy::start_sock_service(const QString& comm_ip, uint16_t comm_port, EProtocolType type, uint16_t dev_port, uint16_t bs_port, const QString& host_ip, uint16_t host_port)
{
    _comm_tbl._dev_ip = comm_ip.toStdString();
    _comm_tbl._business_port = bs_port;
    _comm_tbl._dev_port = comm_port;
    _comm_tbl._proto_type = type;

    _comm_tbl._comm_host_ip = host_ip.toStdString();
    _comm_tbl._comm_host_port = host_port;

    bool ret_1 = _sock_util_ptr->start_trans_service(comm_ip, comm_port, type, dev_port);
    bool ret_2 = _sock_util_ptr->start_trans_service(comm_ip, bs_port, type, dev_port);
    bool ret_3 = _sock_remote_ptr->start_trans_service(host_ip, host_port, eProtocol_tcp, dev_port);

    bool ret = ret_1 && ret_2;
    QString tips = ret ? "successfully" : "failed";
    QString info = QString("client_proxy: start_sock_service connect to communiction %1, protocol type: 2").arg(tips).arg(type);
    emit progress_log_signal(info);

    tips = ret_3 ? "successfully" : "failed";
    info = QString("client_proxy: start_sock_service connect remote monitor %1, protocol type: 2").arg(tips).arg(eProtocol_tcp);
    emit progress_log_signal(info);

    return ret;
}

void client_proxy::handle_power(ApplMessage* msg)
{
    long msg_type = msg->_proc_msg->_msg_type;
    long proc_type = msg->_proc_msg->_proc_type;

    if(proc_type !=  _expect_proc_type || msg_type != _expect_msg_type){
        QString info = QString("client_proxy: handle_power, unexpected status: expect state, proc_type: %1, msg_type: %2; current state:  proc_type: %3, msg_type: %4").arg(_expect_proc_type).arg(_expect_msg_type).arg(_expect_msg_type).arg(proc_type).arg(msg_type);
        emit progress_log_signal(info);
        return;
    }

    IntMap dev_type_id_tbl = _appl_layer->get_dev_id_map();

    int ss_id = msg->_u2i;
    int ps_id = dev_type_id_tbl[eSimDev_communication];
    const char* appl_name = ProcNamesSet[eApplProc_comm_sim];

    QString info = QString("client_proxy: handle_power, appl_name: %1, ss_id: %2, ps_id: %3, proc_type: %4, msg_type: %5").arg(appl_name).arg(ss_id).arg(ps_id).arg(proc_type).arg(msg_type);
    emit progress_log_signal(info);

    int ret = 0;
    QString tips = "session: ";
    QDomDocument* doc = nullptr;

    if(proc_type == eSubProcedure_sim_cmd && msg_type == eSimCmd_start_sim){
        if(!_power_init_success || _power_sim_started){
            return;
        }

        QString info = QString("client_proxy: handle_power, start execute power sim");
        qInfo(info.toStdString().c_str());
        emit progress_log_signal(info);

        _power_sim_started = true;
        ret = _power_handler->Execute(_power_conf_param.input_num,
                                                           (EPowerDataType)_power_conf_param.input_type,
                                                          &_input_info[0],
                                                          _power_conf_param.result_num,
                                                          (EPowerDataType)_power_conf_param.result_type,
                                                          &_result_info[0]);
        if(ret < 0){
            info = QString("client_proxy: handle_power, power simulation over!!!!!!");
            qInfo(info.toStdString().c_str());
            emit progress_log_signal(info);
        }
        else{
            if(ret == 0){
                doc = XmlUtil::generate_session_xml(ss_id, ps_id, DevNamesSet[eSimDev_communication], eSubProcedure_session_begin, eMessage_request);
                tips += "session begin, request";

                _expect_msg_type = eMessage_confirm;
                _expect_proc_type = eSubProcedure_session_begin;
            }

            QString eret = doc ? "successfully" : "failed";
            info = QString("client_proxy: handle_power, execute power sim, %1").arg(eret);
            qInfo(info.toStdString().c_str());
            emit progress_log_signal(info);
        }
    }
    else if(proc_type == eSubProcedure_session_begin && msg_type == eMessage_confirm){
        IntVec vec  = {1, 2, 3};
        tips += "start application procedure, request";
        doc = XmlUtil::generate_appl_req_xml(ss_id, ps_id, appl_name, vec, eMessage_request);

        _expect_msg_type = eMessage_confirm;
        _expect_proc_type = eSubProcedure_appl_request;
    }
    else if(proc_type == eSubProcedure_appl_request && msg_type == eMessage_confirm){
       if(map_power_comm_sim_data(_union_sim_dat_snd_vec)){
            DataXmlVec vec;
            XmlUtil::generate_xml_power_sim_data(_union_sim_dat_snd_vec, vec);
            doc = XmlUtil::generate_snd_data_xml(ss_id, ps_id, appl_name, 1, vec, eMessage_request);
            tips += QString("start send data, request, items: %1").arg(_union_sim_dat_snd_vec.size());

            _expect_msg_type = eMessage_confirm;
            _expect_proc_type = eSubProcedure_data_send;
       }
    }
    else if(proc_type == eSubProcedure_data_send && msg_type == eMessage_confirm){
        DataXmlVec vec;
        doc = XmlUtil::generate_invoke_xml(ss_id, ps_id, "", appl_name, 1, vec, eMessage_request);
        tips += "start invoke, request";

        _expect_msg_type = eMessage_confirm;
        _expect_proc_type = eSubProcedure_invoke;
    }
    else if(proc_type == eSubProcedure_invoke && msg_type == eMessage_confirm){
        XmlUtil::parse_xml_power_appl_data(msg->_proc_msg->_func_invoke_body->_data, _dbl_vec, _union_sim_dat_rcv_vec);
        doc = XmlUtil::generate_session_xml(ss_id, ps_id, DevNamesSet[eSimDev_communication], eSubProcedure_session_end, eMessage_request);
        tips += "session end, request";

        _expect_msg_type = eMessage_confirm;
        _expect_proc_type = eSubProcedure_session_end;
    }
    else if(proc_type == eSubProcedure_session_end && msg_type == eMessage_confirm){
        if(_power_init_success){
           reset_power_input_data();
           ret = _power_handler->Execute(_power_conf_param.input_num,
                                                              (EPowerDataType)_power_conf_param.input_type,
                                                             &_input_info[0],
                                                             _power_conf_param.result_num,
                                                             (EPowerDataType)_power_conf_param.result_type,
                                                             &_result_info[0]);

           if(ret == 0){
               doc = XmlUtil::generate_session_xml(ss_id, ps_id, DevNamesSet[eSimDev_communication], eSubProcedure_session_begin, eMessage_request);
               tips += "rcv session end msg, enter next sim stage, session begin, request";

               _expect_msg_type = eMessage_confirm;
               _expect_proc_type = eSubProcedure_session_begin;
           }
           else if(ret < 0){
               info = QString("client_proxy: handle_power, power simulation over!!!!!!");
               qInfo(info.toStdString().c_str());
               emit progress_log_signal(info);
           }
        }
    }

    if(doc){
        emit snd_lower_signal(doc, QString::fromStdString(_sbs_ip), _sbs_port);
        info = QString("client_proxy:handle_power %1") .arg(tips);
        emit progress_log_signal(info);
    }
}

void client_proxy::handle_power_appl(ApplMessage* msg)
{
    int ss_id = msg->_u2i;
    IntMap dev_type_id_tbl = _appl_layer->get_dev_id_map();
    int ps_id = dev_type_id_tbl[eSimDev_communication];
    const char* appl_name = ProcNamesSet[eApplProc_control_calc];

    QString info = QString("client_proxy: handle_power_appl, appl_name: %1, ss_id: %2, ps_id: %3").arg(appl_name).arg(ss_id).arg(ps_id);
    qInfo(info.toStdString().c_str());
    emit progress_log_signal(info);

    long msg_type = msg->_proc_msg->_msg_type;
    long proc_type = msg->_proc_msg->_proc_type;

    QString tips = "session: ";
    QDomDocument* doc = nullptr;
    if(proc_type == eSubProcedure_sim_cmd && msg_type == eSimCmd_start_sim){
        _decision_alth = DecisionAlth::get_instance();
    }
    else if(proc_type == eSubProcedure_session_begin && msg_type == eMessage_request){
        doc = XmlUtil::generate_session_xml(ss_id, ps_id, DevNamesSet[eSimDev_communication], eSubProcedure_session_begin, eMessage_confirm);
        tips += "session begin, confirm";
    }
    else if(proc_type == eSubProcedure_appl_request && msg_type == eMessage_request){
        IntVec vec  = {1, 2, 3, 4};
        doc = XmlUtil::generate_appl_req_xml(ss_id, ps_id, appl_name, vec, eMessage_confirm);
        tips += "appl request, confirm";
    }
    else if(proc_type == eSubProcedure_data_send && msg_type == eMessage_request){
        DataXmlVec vec;
        XmlUtil::parse_xml_power_sim_data(msg->_proc_msg->_data_vector, _union_sim_dat_rcv_vec);
        doc = XmlUtil::generate_snd_data_xml(ss_id, ps_id, appl_name, 1, vec, eMessage_confirm);
        tips += "data send, confirm";
    }
    else if(proc_type == eSubProcedure_invoke && msg_type == eMessage_request){
        DataXmlVec vec;
        calc_power_appl_data(_union_sim_dat_rcv_vec, vec);
        doc = XmlUtil::generate_invoke_xml(ss_id, ps_id, "", appl_name, 1, vec, eMessage_confirm);
        tips += "invoke confirm";
    }
    else if(proc_type == eSubProcedure_session_end && msg_type == eMessage_request){
       doc = XmlUtil::generate_session_xml(ss_id, ps_id, DevNamesSet[eSimDev_communication], eSubProcedure_session_end, eMessage_confirm);
       tips += "session end, confirm";
    }

    if(doc){
        emit snd_lower_signal(doc, QString::fromStdString(_sbs_ip), _sbs_port);

        info = QString("client_proxy:handle_power_appl, %1").arg(tips);
        emit progress_log_signal(info);
    }
}

void client_proxy::handle_communication(ApplMessage* msg)
{
    long msg_type = msg->_proc_msg->_msg_type;
    long proc_type = msg->_proc_msg->_proc_type;

    IntMap dev_type_id_tbl = _appl_layer->get_dev_id_map();
    if(msg->_i2u == dev_type_id_tbl[eSimDev_power]){
        handle_comm_power(msg);
    }
    else if(msg->_i2u == dev_type_id_tbl[eSimDev_power_appl]){
        handle_comm_power_appl(msg);
    }
}

void client_proxy::handle_comm_power(ApplMessage* msg)
{
    int ss_id = msg->_u2i;
    int ps_id = msg->_i2u;
    long msg_type = msg->_proc_msg->_msg_type;
    long proc_type = msg->_proc_msg->_proc_type;
    const char* appl_name = ProcNamesSet[eApplProc_comm_sim];

    QString info = QString("client_proxy: handle_comm_power, appl_name: %1, ss_id: %2, ps_id: %3").arg(appl_name).arg(ss_id).arg(ps_id);
    qInfo(info.toStdString().c_str());
    emit progress_log_signal(info);

    QString tips = "session: ";
    QDomDocument* doc = nullptr;
    if(proc_type == eSubProcedure_session_begin && msg_type == eMessage_request){
        doc = XmlUtil::generate_session_xml(ss_id, ps_id, DevNamesSet[eSimDev_power], eSubProcedure_session_begin, eMessage_confirm);
        tips += "session begin, confirm";
    }
    else if(proc_type == eSubProcedure_appl_request && msg_type == eMessage_request){
        IntVec vec  = {1, 2, 3};
        doc = XmlUtil::generate_appl_req_xml(ss_id, ps_id, appl_name, vec, eMessage_confirm);
        tips += "application request, confirm";
    }
    else if(proc_type == eSubProcedure_data_send && msg_type == eMessage_request){
        XmlUtil::parse_xml_power_sim_data(msg->_proc_msg->_data_vector, _union_sim_dat_snd_vec);
        doc = XmlUtil::generate_snd_data_xml(ss_id, ps_id, appl_name, 1, msg->_proc_msg->_data_vector, eMessage_confirm);
        tips += "send data, confirm";
    }
    else if(proc_type == eSubProcedure_invoke && msg_type == eMessage_request){
        snd_upper_to_comm();
        //rcv_upper_msg_callback(nullptr, 0);
        tips += "invoke comm sim, wait a moment";
    }
    else if(proc_type == eSubProcedure_session_end && msg_type == eMessage_request){
       doc = XmlUtil::generate_session_xml(ss_id, ps_id, DevNamesSet[eSimDev_power], eSubProcedure_session_end, eMessage_confirm);
       tips += "session end, confirm";
    }

    if(doc){
        emit snd_lower_signal(doc, QString::fromStdString(_sbs_ip), _sbs_port);

        info = QString("client_proxy:handle_comm_power, %1").arg(tips);
        emit progress_log_signal(info);
    }
}

void client_proxy::handle_comm_power_appl(ApplMessage* msg)
{
    long msg_type = msg->_proc_msg->_msg_type;
    long proc_type = msg->_proc_msg->_proc_type;

    if(proc_type != _expect_proc_type || msg_type != _expect_msg_type){
      QString info = QString("client_proxy: handle_comm_power_appl, unexpected status: expect state, proc_type: %1, msg_type: %2; current state:  proc_type: %3, msg_type: %4").arg(_expect_proc_type).arg(_expect_msg_type).arg(proc_type).arg(msg_type);
        emit progress_log_signal(info);
        return;
    }

    int ss_id = msg->_u2i;
    int ps_id = msg->_i2u;
    const char* appl_name = ProcNamesSet[eApplProc_control_calc];

    QString info = QString("client_proxy: handle_comm_power_appl, appl_name: %1, ss_id: %2, ps_id: %3").arg(appl_name).arg(ss_id).arg(ps_id);
    qInfo(info.toStdString().c_str());
    emit progress_log_signal(info);

    QString tips = "session: ";
    QDomDocument* doc = nullptr;
    if(proc_type == eSubProcedure_session_begin && msg_type == eMessage_confirm){
        IntVec vec  = {1, 2, 3, 4};
        doc = XmlUtil::generate_appl_req_xml(ss_id, ps_id, appl_name, vec, eMessage_request);
        tips += "appl_req, request";

        _expect_msg_type = eMessage_confirm;
        _expect_proc_type = eSubProcedure_appl_request;
    }
    else if(proc_type == eSubProcedure_appl_request && msg_type == eMessage_confirm){
        DataXmlVec vec;
        XmlUtil::generate_xml_power_sim_data(_union_sim_dat_rcv_vec, vec);
        doc = XmlUtil::generate_snd_data_xml(ss_id, ps_id, appl_name, 1, vec, eMessage_request);
        tips += "send data, request";

        _expect_msg_type = eMessage_confirm;
        _expect_proc_type = eSubProcedure_data_send;
    }
    else if(proc_type == eSubProcedure_data_send && msg_type == eMessage_confirm){
        DataXmlVec vec;
        doc = XmlUtil::generate_invoke_xml(ss_id, ps_id, "", appl_name, 1, vec, eMessage_request);
        tips += "invoke, request";

        _expect_msg_type = eMessage_confirm;
        _expect_proc_type = eSubProcedure_invoke;
    }
    else if(proc_type == eSubProcedure_invoke && msg_type == eMessage_confirm){
        XmlUtil::parse_xml_power_appl_data(msg->_proc_msg->_func_invoke_body->_data, _dbl_vec, _union_sim_dat_rcv_vec);
        doc = XmlUtil::generate_session_xml(ss_id, ps_id, DevNamesSet[eSimDev_communication], eSubProcedure_session_end, eMessage_request);
        tips += "session end, request";

        _expect_msg_type = eMessage_confirm;
        _expect_proc_type = eSubProcedure_session_end;
    }
    else if(proc_type == eSubProcedure_session_end && msg_type == eMessage_confirm){
        DataXmlVec vec;
        XmlUtil::generate_xml_power_appl_data(_dbl_vec, _union_sim_dat_rcv_vec, vec);

        IntMap dev_type_id_tbl = _appl_layer->get_dev_id_map();
        ss_id = dev_type_id_tbl[eSimDev_communication];
        ps_id = dev_type_id_tbl[eSimDev_power];
        doc = XmlUtil::generate_invoke_xml(ss_id, ps_id, "confirm invoke", appl_name, 1, vec, eMessage_confirm);

        tips += "invoke, confirm to power";

        _expect_msg_type = eMessage_confirm;
        _expect_proc_type = eSubProcedure_session_begin;
    }

    if(doc){
        emit snd_lower_signal(doc, QString::fromStdString(_sbs_ip), _sbs_port);

        info = QString("client_proxy:handle_comm_power_appl, %1").arg(tips);
        emit progress_log_signal(info);
    }
}

void client_proxy::handle_interoper(ApplMessage* msg)
{
    switch(_dev_type){
    case eSimDev_power: handle_power(msg); break;
    case eSimDev_power_appl: handle_power_appl(msg); break;
    case eSimDev_communication: handle_communication(msg); break;
    }
}

void client_proxy::handle_sim_cmd(ApplMessage* msg)
{
    switch( msg->_proc_msg->_msg_type){
    case eSimCmd_start_sim:{
        switch(_dev_type){
        case eSimDev_power: handle_power(msg); break;
        case eSimDev_power_appl: handle_power_appl(msg); break;
        case eSimDev_communication: handle_communication(msg); break;
        }
        break;
    }
    case eSimCmd_pause_sim: break;
    case eSimCmd_stop_sim: break;
    default: break;
    }
}

int client_proxy::parse_comm_cfg_param(ApplMessage* msg, char* value)
{
    if(!msg || !value){
        return -1;
    }

    int len = -1;
    bool ret = false;
    const DataXmlVec& vec = msg->_proc_msg->_data_vector;

    switch(msg->_pg_rtui_type){
    case ePG_RTUI_msg_getinterfacenum:{
        PG_RTUI_Msg_GetInterfaceNum data;
        ret = XmlUtil::parse_PG_RTUI_Msg_GetInterfaceNum_xml(vec, data);
        len = sizeof(PG_RTUI_Msg_GetInterfaceNum);
        memcpy(value, &data, len);
        break;
    }
    case ePG_RTUI_ack_getinterfacenum: {
        PG_RTUI_Ack_GetInterfaceNum data;
        ret = XmlUtil::parse_PG_RTUI_Ack_GetInterfaceNum_xml(vec, data);
        len = sizeof(PG_RTUI_Ack_GetInterfaceNum);
        memcpy(value, &data, len);
        break;
    }
    case ePG_RTUI_msg_getinterface:{
        PG_RTUI_Msg_GetInterface data;
        ret = XmlUtil::parse_PG_RTUI_Msg_GetInterface_xml(vec, data);
        len = sizeof(PG_RTUI_Msg_GetInterface);
        memcpy(value, &data, len);
        break;
    }
    case ePG_RTUI_ack_getinterface:{
        PG_RTUI_Ack_GetInterface data;
        ret = XmlUtil::parse_PG_RTUI_Ack_GetInterface_xml(vec, data);
        len = sizeof(PG_RTUI_Ack_GetInterface);
        memcpy(value, &data, len);
        break;
    }
    case ePG_RTUI_msg_setinterface:{
        PG_RTUI_Msg_SetInterface data;
        ret = XmlUtil::parse_PG_RTUI_Msg_SetInterface_xml(vec, data);
        len = sizeof(PG_RTUI_Msg_SetInterface);
        memcpy(value, &data, len);
        break;
    }
    case ePG_RTUI_msg_getlink:{
        PG_RTUI_Msg_GetLink data;
        ret = XmlUtil::parse_PG_RTUI_Msg_GetLink_xml(vec, data);
        len = sizeof(PG_RTUI_Msg_GetLink);
        memcpy(value, &data, len);
        break;
    }
    case ePG_RTUI_ack_getlink:{
        PG_RTUI_Ack_GetLink data;
        ret = XmlUtil::parse_PG_RTUI_Ack_GetLink_xml(vec, data);
        len = sizeof(PG_RTUI_Ack_GetLink);
        memcpy(value, &data, len);
        break;
    }
    case ePG_RTUI_msg_setlink: {
        PG_RTUI_Msg_SetLink data;
        ret = XmlUtil::parse_PG_RTUI_Msg_SetLink_xml(vec, data);
        len = sizeof(PG_RTUI_Msg_SetLink);
        memcpy(value, &data, len);
        break;
    }
    case ePG_RTUI_change_node_status:{
        PG_RTUI_ChangeNodeStatus data;
        ret = XmlUtil::parse_PG_RTUI_ChangeNodeStatus_xml(vec, data);
        len = sizeof(PG_RTUI_ChangeNodeStatus);
        memcpy(value, &data, len);
        break;
    }
    case ePG_RTUI_recover_node_status:{
        PG_RTUI_ChangeNodeStatus data;
        ret = XmlUtil::parse_PG_RTUI_ChangeNodeStatus_xml(vec, data);
        len = sizeof(PG_RTUI_ChangeNodeStatus);
        memcpy(value, &data, len);
        break;
    }
    case ePG_RTUI_change_port_status: {
        PG_RTUI_ChangePortStatus data;
        ret = XmlUtil::parse_PG_RTUI_ChangePortStatus_xml(vec, data);
        len = sizeof(PG_RTUI_ChangePortStatus);
        memcpy(value, &data, len);
        break;
    }
    case ePG_RTUI_recover_port_status:{
        PG_RTUI_ChangePortStatus data;
        ret = XmlUtil::parse_PG_RTUI_ChangePortStatus_xml(vec, data);
        len = sizeof(PG_RTUI_ChangePortStatus);
        memcpy(value, &data, len);
        break;
    }
    case ePG_RTUI_static_route:{
        PG_RTUI_StaticRoute data;
        ret = XmlUtil::parse_PG_RTUI_StaticRoute_xml(vec, data);
        len = sizeof(PG_RTUI_StaticRoute);
        memcpy(value, &data, len);
        break;
    }
    case ePG_RTUI_add_cbr: {
        PG_RTUI_AddCBR data;
        ret = XmlUtil::parse_PG_RTUI_AddCBR_xml(vec, data);
        len = sizeof(PG_RTUI_AddCBR);
        memcpy(value, &data, len);
        break;
    }
    case ePG_RTUI_set_data_tamper_sim_time: {
        PG_RTUI_SetDataTamperSimTime data;
        ret = XmlUtil::parse_PG_RTUI_SetDataTamperSimTime_xml(vec, data);
        len = sizeof(PG_RTUI_SetDataTamperSimTime);
        memcpy(value, &data, len);
        break;
    }
    case ePG_RTUI_set_data_tamper_last_time: {
        PG_RTUI_SetDataTamperLastTime data;
        ret = XmlUtil::parse_PG_RTUI_SetDataTamperLastTime_xml(vec, data);
        len = sizeof(PG_RTUI_SetDataTamperLastTime);
        memcpy(value, &data, len);
        break;
    }
    case ePG_RTUI_stop_data_tamper: {
        PG_RTUI_StopDataTamper data;
        ret = XmlUtil::parse_PG_RTUI_StopDataTamper_xml(vec, data);
        len = sizeof(PG_RTUI_StopDataTamper);
        memcpy(value, &data, len);
        break;
    }
    case ePG_RTUI_add_staticroute:
    case ePG_RTUI_remove_staticroute:{
        PG_RTUI_StaticRoute data;
        ret = XmlUtil::parse_PG_RTUI_StaticRoute_xml(vec, data);
        len = sizeof(PG_RTUI_StaticRoute);
        memcpy(value, &data, len);
        break;
    }
    case ePG_sim_interoper_data: {
        break;
    }
    default: break;
    }

    QString info = QString("client_proxy: parse_comm_cfg_param, pg_rtui_type: %1").arg(msg->_pg_rtui_type);
    qInfo(info.toStdString().c_str());
    emit progress_log_signal(info);

    return len;
}


void client_proxy::handle_comm_cfg_param(ApplMessage* msg)
{
    if(msg->_pg_rtui_type == ePG_comm_sim_cmd_data){
        XmlUtil::parse_CommSimConfParam_xml(msg->_proc_msg->_data_vector, _comm_conf_param);
        bool ret = _sock_remote_ptr->send_data(_comm_tbl._comm_host_ip.c_str(), _comm_tbl._comm_host_port, _comm_conf_param.comm_cmd.c_str(), _comm_conf_param.comm_cmd.length());

        QString tips = ret ? "successfully" : "failed";
        QString info = QString("client_proxy: handle_comm_cfg_param, send remote operate comm software cmd %1").arg(tips);
        emit progress_log_signal(info.toStdString().c_str());

        //配置仿真最大处理时延
        PG_RTUI_Msg_CommSimHandleDelay comm_sim_delay;
        memset(&comm_sim_delay, 0, sizeof(PG_RTUI_Msg_CommSimHandleDelay));
        comm_sim_delay.type = eCommCmd_sim_delay_cfg;
        comm_sim_delay.length = sizeof(PG_RTUI_Msg_CommSimHandleDelay);
        comm_sim_delay.comm_sim_handle_delay = _comm_conf_param.comm_sim_handle_max_delay;

        ret = _sock_util_ptr->send_data(_comm_tbl._dev_ip.c_str(), _comm_tbl._dev_port, (const char*)&comm_sim_delay, comm_sim_delay.length);
        tips = ret ? "successfully" : "failed";
        info = QString("client_proxy: handle_comm_cfg_param, send comm sim max handl delay %1").arg(tips);
        emit progress_log_signal(info.toStdString().c_str());

        return;
    }

    char data[256] = {0};
    int len = parse_comm_cfg_param(msg, data);
    if(len <= 0){
        QString info = QString("client_proxy: handle_comm_cfg_param, msg invalid, parse_comm_cfg_param failed");
        emit progress_log_signal(info.toStdString().c_str());
        return;
    }

    QHostAddress host(_local_ip.c_str());
    uint32_t val_ip = host.toIPv4Address();
    memcpy(data, &val_ip, sizeof(uint32_t));
    bool ret = _sock_util_ptr->send_data(_comm_tbl._dev_ip.c_str(), _comm_tbl._dev_port, data, len);

    QString tips = ret ? "successfully" : "failed";
    QString info = QString("client_proxy: handle_comm_cfg_param, send data %1, data: %2, len: %3").arg(tips).arg(data).arg(len);
    qInfo(info.toStdString().c_str());
    emit progress_log_signal(info);
}

void client_proxy::handle_power_cfg_param(ApplMessage* msg)
{
    if(_power_init_success){
        return;
    }

    const DataXmlVec& data = msg->_proc_msg->_data_vector;
    if(!fetch_power_cfg_param(data)){
        return;
    }

    if(_dev_type == eSimDev_power_appl){
        return;
    }

    if(_power_conf_param.input_type == ePowerData_dginfor){
        _input_info = new PowerDGInforData[_power_conf_param.input_num];
        memset(_input_info, 0, sizeof(PowerDGInforData) * _power_conf_param.input_num);
    }

    if(_power_conf_param.result_type == ePowerData_businfor){
        _result_info = new PowerBusInforData[_power_conf_param.result_num];
        memset(_result_info, 0, sizeof(PowerBusInforData) * _power_conf_param.result_num);
    }

    //_dev_type == eSimDev_power
    _power_init_success = _power_handler->InitHandler(_power_conf_param.prj_name.c_str(),
                                                                                        _power_conf_param.case_name.c_str(),
                                                                                        _power_conf_param.sim_time,
                                                                                        _power_conf_param.sim_period);

    QString tips = _power_init_success ? "successfully" : "failed";
    QString info = QString("client_proxy: handle_power_cfg_param, init power sim software %1").arg(tips);
    qInfo(info.toStdString().c_str());
    emit progress_log_signal(info);
}

void client_proxy::handle_msg(ApplMessage* msg)
{
    long proc_type = msg->_proc_msg->_proc_type;
    QString info = QString("client_proxy: handle_msg, proc_type: %1").arg(proc_type);
    qInfo(info.toStdString().c_str());
    emit progress_log_signal(info);

    switch (proc_type){
    case eSubProcedure_sim_cmd: handle_sim_cmd(msg); break;
    case eSubProcedure_cfg_power_data: handle_power_cfg_param(msg); break;
    case eSubProcedure_cfg_communication_data: handle_comm_cfg_param(msg); break;
    default: handle_interoper(msg); break;
    }
}

void client_proxy::start_rcv_thread()
{
    _rcv_th = std::thread(std::bind(&client_proxy::rcv_lower_thread, this));
    _rcv_th.detach();
}

void client_proxy::rcv_lower_thread()
{
    ApplMessage* msg = nullptr;
    while(!_quit){
       if(msg =  _proxy_msg_que.pop(!_quit)){
           handle_msg(msg);

           QString info = QString("client_proxy:rcv_lower_thread,msg_name: %1, ss_id: %2, ps_id: %3, proc_type: %4, msg_type: %5").arg(msg->_proc_msg->_msg_name.c_str()).arg(msg->_i2u).arg(msg->_u2i).arg(msg->_proc_msg->_proc_type).arg(msg->_proc_msg->_msg_type);
           LogUtil::Instance()->Output(QtInfoMsg, info);

           delete msg;
       }
    }
}
