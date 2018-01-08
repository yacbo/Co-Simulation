#include <sstream>
#include <functional>
#include <QNetworkInterface>
#include "client_proxy.h"
#include "sock_util.h"
#include "xml_util.h"
#include "log_util.h"
#include "pg_rtui_def.h"
#include "hisrecord_mgr.h"

client_proxy::client_proxy(application_layer* parent, const QString& sbs_ip, quint16 port, ESimDevType type)
{
    _proxy_status = eProxyState_idle;
    _rcv_comm_data_enabled = false;

    _quit = false;
    _appl_layer = parent;

    _sbs_port = port;
    _sbs_ip = sbs_ip.toStdString();

    _dev_type = type;
    _dst_dev_type = eSimDev_undef;

    _sock_util_ptr = nullptr;
    _sock_remote_ptr = nullptr;
    _power_handler  = nullptr;
    _power_init_success = false;
    _power_sim_started = false;
    _decision_alth = nullptr;

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
    }
    else if(type == eSimDev_power_appl){

    }

    _local_ip = SockUtil::query_local_ip();
    start_rcv_thread();

    LogUtil::Instance()->Output(MACRO_LOCAL, DevNamesSet[type], "create client proxy successfully");
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

    if(_upstm_info.size()){
        for(int i=0; i<_power_conf_param.upstm_num; ++i){
            delete _upstm_info[i];
        }
    }

    if(_dwstm_info.size()){
        for(int i=0; i<_power_conf_param.dwstm_num; ++i){
            delete _dwstm_info[i];
        }
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

//void client_proxy::rcv_upper_msg_callback(const char* data, int len)
//{
//    _union_sim_dat_rcv_vec = _union_sim_dat_snd_vec;

//    IntMap dev_type_id_tbl = _appl_layer->get_dev_id_map();
//    int ss_id = dev_type_id_tbl[eSimDev_communication];
//    int ps_id = dev_type_id_tbl[eSimDev_power_appl];
//    QDomDocument* doc = XmlUtil::generate_session_xml(ss_id, ps_id, DevNamesSet[eSimDev_power_appl], eSubProcedure_session_begin, eMessage_request);
//    emit snd_lower_signal(doc, _sbs_ip.c_str(), _sbs_port);

//    QString info = QString("client_proxy: rcv_upper_msg_callback,  items: %1").arg(_union_sim_dat_rcv_vec.size());
//    emit progress_log_signal(info);
//}

void client_proxy::rcv_upper_msg_callback(const char* data, int len)
{
    if(!data){
        LogUtil::Instance()->Output(MACRO_LOCAL, "Invalid Input Arguments");
        return;
    }

    const PG_RTUI_Base* header = (PG_RTUI_Base*)data;
    long type = header->type;

    QString info = LogUtil::Instance()->Output(MACRO_LOCAL, "rcv communication sim data", "PG_RTUI type:", type, "data len", len);
    emit progress_log_signal(info);

    if(type == eCommCmd_start_send_data || type == eCommCmd_stop_send_data){
        _rcv_comm_data_enabled = type == eCommCmd_start_send_data;
        if(type == eCommCmd_stop_send_data){
            IntMap dev_type_id_tbl = _appl_layer->get_dev_id_map();

            ESubProcedureType proc_type = eSubProcedure_undef;
            int ss_id = dev_type_id_tbl[eSimDev_communication];
            int ps_id = dev_type_id_tbl[eSimDev_power_appl];

            switch(_dst_dev_type){
            case eSimDev_power:proc_type = eSubProcedure_session_end; break;
            case eSimDev_power_appl: proc_type = eSubProcedure_session_begin; break;
            default: break;
            }

            if(proc_type != eSubProcedure_undef){
                QDomDocument* doc = XmlUtil::generate_session_xml(ss_id, ps_id, DevNamesSet[eSimDev_power_appl], proc_type, eMessage_request);
                emit snd_lower_signal(doc, _sbs_ip.c_str(), _sbs_port);
                info = QString("rcv communication sim data items:").arg(_union_sim_dat_rcv_vec.size());
            }
            else{
                info = LogUtil::Instance()->Output(MACRO_LOCAL, "not set dst device type when forward the received communication sim data");
            }

            emit progress_log_signal(info);
        }
        else{
            _union_sim_dat_rcv_vec.clear();
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
    QThread::msleep(200);

    QString info = LogUtil::Instance()->Output(MACRO_LOCAL, "send start send power data cmd", MACRO_SUCFAIL(ret));
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
            QString info = LogUtil::Instance()->Output(MACRO_LOCAL,
                                                       "send power sim data to communication failed",
                                                       "cur data index:", i, "len:", header.length,
                                                       "total items:", _union_sim_dat_snd_vec.size());
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
    QThread::msleep(200);

    info = LogUtil::Instance()->Output(MACRO_LOCAL, "send stop send power data cmd", MACRO_SUCFAIL(ret));
    emit progress_log_signal(info);

    info = LogUtil::Instance()->Output(MACRO_LOCAL, "send power sim data items:", snd_items);
    emit progress_log_signal(info);
}

bool client_proxy::fetch_power_cfg_param(const DataXmlVec& vec)
{
    if(!XmlUtil::parse_PowerSimConfParam_xml(vec, _power_conf_param)){
        LogUtil::Instance()->Output(MACRO_LOCAL, "parse power simulation config parameters failed");
        return false;
    }

    //power comm node map
    IntVec ids; string resp;
    std::istringstream parser(_power_conf_param.nodes_map);
    while(parser >> resp){
        ids.push_back(std::stoi(resp));
    }

    if(ids.size() % 3 != 0){
        QString info = LogUtil::Instance()->Output(MACRO_LOCAL, "fetch data failed", "total node map items:", ids.size());
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

    QString tips, info;
    UnionSimData data;
    bool b_map_success = true;

    QDateTime date;
    double phy_time = date.currentDateTime().toSecsSinceEpoch();

    for(int i=0; i<_power_conf_param.upstm_num; ++i){
        IntPairMap::const_iterator it = _bus_comm_id_tbl.find(_upstm_info[i]->bus_id);
        if(it == _bus_comm_id_tbl.cend()){
            info = QString::number(_upstm_info[i]->bus_id);
            tips = QString("bus id: %1 not found").arg(info);
            b_map_success = false;
            break;
        }

        memset(&data, 0, sizeof(UnionSimData));
        data.realtime = phy_time;

        const IntPair& comm = it->second;
        data.comm_dat.src_id = comm.first;
        data.comm_dat.dst_id = comm.second;

        switch(_power_conf_param.upstm_type){
        case ePowerData_businfor:{
            const PowerBusInforData* d = (const PowerBusInforData*)_upstm_info[i];
            data.sim_time = d->cur_sim_time;
            memcpy(data.power_dat, d, sizeof(PowerBusInforData));
            break;
        }
        default: break;
        }

        ud.push_back(data);
    }

    tips = b_map_success ? "successfully" : tips;
    info  = LogUtil::Instance()->Output(MACRO_LOCAL, "map total", _power_conf_param.upstm_num, "data items", tips.toStdString());
    emit progress_log_signal(info);

    return b_map_success;
}

string client_proxy::stream_power_sim_data(const UnionSimDatVec& data, int64_t& sim_time)
{
    string stream;
    if(data.empty()){
         LogUtil::Instance()->Output(MACRO_LOCAL, "data is empty");
        return stream;
    }

    QString info  = LogUtil::Instance()->Output(MACRO_LOCAL, "total data items:", data.size());
    emit progress_log_signal(info);

    sim_time = data[0].sim_time;
    stream = std::to_string(data[0].sim_time) + " ";
    for(int i=0; i<data.size(); ++i){
        switch (_power_conf_param.upstm_type) {
        case ePowerData_businfor:{
            PowerBusInforData* d = (PowerBusInforData*)data[i].power_dat;
            stream += std::to_string(d->bus_volt) + " " + std::to_string(d->bus_angle) + " ";
            break;
        }
        default: break;
        }
    }

    return stream;
}

bool client_proxy::calc_power_appl_data(UnionSimDatVec& data, DataXmlVec& vec)
{
    if(data.size() != _power_conf_param.upstm_num){
        UnionSimDatVec his_rec_vec;
        if(!HisRecordMgr::instance()->load_record(_power_conf_param.upstm_type, his_rec_vec)){
            LogUtil::Instance()->Output(MACRO_LOCAL, "load history record failed");
            return false;
        }

        if(!HisRecordMgr::instance()->fill_record(_power_conf_param.upstm_type, his_rec_vec, data)){
            LogUtil::Instance()->Output(MACRO_LOCAL, "fill record failed");
            return false;
        }

        QString info = LogUtil::Instance()->Output(MACRO_LOCAL, "rcv data items not equal to the config value, current items:",
                                                   data.size(), "config value:", _power_conf_param.upstm_num, "Apply History Data");
        emit progress_log_signal(info);
    }

    QString info  = LogUtil::Instance()->Output(MACRO_LOCAL, "total data items:", _power_conf_param.upstm_num);
    emit progress_log_signal(info);

    int64_t sim_time;
    ProcEventParam param;
    param._bus_num = _power_conf_param.upstm_num;
    param._in_out_info = stream_power_sim_data(data, sim_time);
    param._type = (EPowerDataType)_power_conf_param.upstm_type;

    switch(_power_conf_param.upstm_type){
    case ePowerData_businfor:{
        double* dvg_ret = new double[_power_conf_param.upstm_num];
        memset(dvg_ret, 0, sizeof(double) * _power_conf_param.upstm_num);
        _decision_alth->execute_event_proc_algorithm(&param, dvg_ret);

        if(HisRecordMgr::instance()->write_record(sim_time, _power_conf_param.upstm_type, data)){
            LogUtil::Instance()->Output(MACRO_LOCAL, "write record, items:", data.size());
        }

        const int offset = _power_conf_param.upstm_num - _power_conf_param.dwstm_num;

        UnionSimDatVec::iterator it = data.begin();
        data.erase(it, it + offset - 1);
        exchange_comm_node_src_dst_id(data, _power_conf_param.upstm_type);

        DblVec tmp;
        for(int i=0; i<_power_conf_param.dwstm_num; ++i){
            tmp.push_back(dvg_ret[i + offset]);
        }

        XmlUtil::generate_xml_power_appl_data(tmp, data, vec);
        delete[] dvg_ret;
    }
    default: break;
    }

    return true;
}

void client_proxy::exchange_comm_node_src_dst_id(UnionSimDatVec& data, int type)
{
    switch(type){
    case ePowerData_businfor:{
        for(int i=0; i<_power_conf_param.dwstm_num; ++i){
            UnionSimData& udi = data[i];
            std::swap(udi.comm_dat.src_id, udi.comm_dat.dst_id);
        }
        break;
    }
    default: break;
    }
}

void client_proxy::reset_power_input_data()
{
    if(_dbl_vec.size() != _union_sim_dat_rcv_vec.size()){
        QString info = LogUtil::Instance()->Output(MACRO_LOCAL, "double data items:", _dbl_vec.size(), "rcv sim data items:", _union_sim_dat_rcv_vec.size(), "not equal");
        emit progress_log_signal(info);
        return;
    }

    for(int i=0; i<_dbl_vec.size(); ++i){
        switch (_power_conf_param.dwstm_type) {
        case ePowerData_dginfor:{
            PowerDGInforData* di = (PowerDGInforData*)_dwstm_info[i];
            PowerBusInforData* bi = (PowerBusInforData*)_union_sim_dat_rcv_vec[i].power_dat;

            di->dv = _dbl_vec[i];
            di->bus_id = bi->bus_id;
            di->time_diff = _union_sim_dat_rcv_vec[i].comm_dat.trans_delay;
            break;
        }
        default: break;
        }
    }
}

void client_proxy::replace_power_sim_data(UnionSimData* data)
{
    for(int i=0; i<_union_sim_dat_snd_vec.size(); ++i){
        const UnionSimData& d = _union_sim_dat_snd_vec[i];
        if(data->comm_dat.src_id == d.comm_dat.src_id && data->comm_dat.dst_id == d.comm_dat.dst_id){
            memcpy(&data->power_dat, &d.power_dat, sizeof(d.power_dat));
            break;
        }
    }
}

void client_proxy::handle_css(EPGRTUIType type,  const char* data, int len)
{
    QString info  = LogUtil::Instance()->Output(MACRO_LOCAL, "PG_RTUI type:", type, "data len:", len);
    emit progress_log_signal(info);

    IntMap dev_type_id_tbl = _appl_layer->get_dev_id_map();

    int ss_id = dev_type_id_tbl[eSimDev_communication];
    int ps_id = dev_type_id_tbl[eSimDev_sim_controller];

    const PG_RTUI_Base* d_base = (PG_RTUI_Base*)data;
    QDomDocument* doc = XmlUtil::create_PG_RTUI_xml(ss_id, ps_id, d_base);
    if(!doc){
        info  = LogUtil::Instance()->Output(MACRO_LOCAL, "create_PG_RTUI_xml failed");
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

    bool ret_1 = _sock_util_ptr->start_trans_service(comm_ip, comm_port, type, 0);  //启用comm_port
    bool ret_2 = _sock_util_ptr->start_trans_service(comm_ip, bs_port, type, 0);        //启用bs_port
    bool ret_3 = _sock_remote_ptr->start_trans_service(host_ip, host_port, eProtocol_tcp, dev_port);

    bool ret = ret_1 && ret_2;
    QString info  = LogUtil::Instance()->Output(MACRO_LOCAL,
                                                "connect communiction software", MACRO_SUCFAIL(ret),
                                                "ip:", comm_ip.toStdString(), "port:", comm_port,"protocol:", type);
    emit progress_log_signal(info);

    info  = LogUtil::Instance()->Output(MACRO_LOCAL,
                                                "connect remote start listener", MACRO_SUCFAIL(ret_3),
                                                "ip:", host_ip.toStdString(), "port:", host_port,"protocol:", eProtocol_tcp);
    emit progress_log_signal(info);

    return ret;
}

void client_proxy::handle_power(ApplMessage* msg)
{
    long msg_type = msg->_proc_msg->_msg_type;
    long proc_type = msg->_proc_msg->_proc_type;

    if(proc_type !=  _expect_proc_type || msg_type != _expect_msg_type){
        QString info  = LogUtil::Instance()->Output(MACRO_LOCAL, "unexpected status:",
                                                    "[current state]: proc_type:",  parse_type(proc_type),
                                                    "msg_type:", parse_type(msg_type),
                                                    "[expected state]: proc_type:", parse_type(_expect_proc_type),
                                                    "msg_type:", parse_type(_expect_msg_type));
        emit progress_log_signal(info);
        return;
    }

    IntMap dev_type_id_tbl = _appl_layer->get_dev_id_map();

    int ss_id = msg->_u2i;
    int ps_id = dev_type_id_tbl[eSimDev_communication];
    const char* appl_name = ProcNamesSet[eApplProc_comm_sim];

    QString info  = LogUtil::Instance()->Output(MACRO_LOCAL,
                                                "application name:", appl_name,
                                                "ss_id", ss_id, "ps_id", ps_id,
                                                "proc_type:",  parse_type(proc_type),
                                                "msg_type:", parse_type(msg_type));
    emit progress_log_signal(info);

    int ret = 0;
    QString tips = "session: ";
    QDomDocument* doc = nullptr;

    if(proc_type == eSubProcedure_sim_cmd && msg_type == eSimCmd_start_sim){
        if(!_power_init_success || _power_sim_started){
            return;
        }

        info  = LogUtil::Instance()->Output(MACRO_LOCAL, "start execute power simulation");
        emit progress_log_signal(info);

        _power_sim_started = true;
        ret = _power_handler->Execute(_power_conf_param.dwstm_num,
                                                           (EPowerDataType)_power_conf_param.dwstm_type,
                                                          _dwstm_info,
                                                          _power_conf_param.upstm_num,
                                                          (EPowerDataType)_power_conf_param.upstm_type,
                                                          _upstm_info);
        if(ret < 0){
            _power_handler->ExitHandler();
            info  = LogUtil::Instance()->Output(MACRO_LOCAL, "power simulation execute Over, power software has closed");
            emit progress_log_signal(info);
        }
        else{
            if(ret == 0){
                doc = XmlUtil::generate_session_xml(ss_id, ps_id, DevNamesSet[eSimDev_communication], eSubProcedure_session_begin, eMessage_request);
                tips += "session begin, request";

                _expect_msg_type = eMessage_confirm;
                _expect_proc_type = eSubProcedure_session_begin;
            }

            info  = LogUtil::Instance()->Output(MACRO_LOCAL, "power simulation execute", MACRO_SUCFAIL(!doc));
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
           ret = _power_handler->Execute(_power_conf_param.dwstm_num,
                                                              (EPowerDataType)_power_conf_param.dwstm_type,
                                                             _dwstm_info,
                                                             _power_conf_param.upstm_num,
                                                             (EPowerDataType)_power_conf_param.upstm_type,
                                                             _upstm_info);

           if(ret == 0){
               doc = XmlUtil::generate_session_xml(ss_id, ps_id, DevNamesSet[eSimDev_communication], eSubProcedure_session_begin, eMessage_request);
               tips += "rcv session end msg, enter next sim stage, session begin, request";

               _expect_msg_type = eMessage_confirm;
               _expect_proc_type = eSubProcedure_session_begin;
           }
           else if(ret < 0){
               _power_handler->ExitHandler();
               info  = LogUtil::Instance()->Output(MACRO_LOCAL, "power simulation execute Over, power software has closed");
               emit progress_log_signal(info);
           }
        }
    }

    if(doc){
        emit snd_lower_signal(doc, QString::fromStdString(_sbs_ip), _sbs_port);
        info  = LogUtil::Instance()->Output(MACRO_LOCAL, "power subprogress information:", tips.toStdString());
        emit progress_log_signal(info);
    }
    else{
        LogUtil::Instance()->Output(MACRO_LOCAL, "generate QDomDocument failed",
                                    "proc_type:", parse_type(proc_type),
                                    "msg_type:", parse_type(msg_type));
    }
}

void client_proxy::handle_power_appl(ApplMessage* msg)
{
    int ss_id = msg->_u2i;
    IntMap dev_type_id_tbl = _appl_layer->get_dev_id_map();
    int ps_id = dev_type_id_tbl[eSimDev_communication];
    const char* appl_name = ProcNamesSet[eApplProc_control_calc];

    long msg_type = msg->_proc_msg->_msg_type;
    long proc_type = msg->_proc_msg->_proc_type;

    QString info  = LogUtil::Instance()->Output(MACRO_LOCAL,
                                        "application name:", appl_name,
                                        "ss_id:", ss_id, "ps_id:", ps_id,
                                        "proc_type:", parse_type(proc_type),
                                        "msg_type:", parse_type(msg_type));
    emit progress_log_signal(info);

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
        bool ret = calc_power_appl_data(_union_sim_dat_rcv_vec, vec);
        doc = XmlUtil::generate_invoke_xml(ss_id, ps_id, "", appl_name, 1, vec, eMessage_confirm);
        tips += "invoke confirm";
    }
    else if(proc_type == eSubProcedure_session_end && msg_type == eMessage_request){
       doc = XmlUtil::generate_session_xml(ss_id, ps_id, DevNamesSet[eSimDev_communication], eSubProcedure_session_end, eMessage_confirm);
       tips += "session end, confirm";
    }

    if(doc){
        emit snd_lower_signal(doc, QString::fromStdString(_sbs_ip), _sbs_port);

        info  = LogUtil::Instance()->Output(MACRO_LOCAL, "power application subprogress information:", tips.toStdString());
        emit progress_log_signal(info);
    }
    else{
        LogUtil::Instance()->Output(MACRO_LOCAL, "generate QDomDocument failed",
                                    "proc_type:", parse_type(proc_type),
                                    "msg_type:", parse_type(msg_type));
    }
}

void client_proxy::handle_communication(ApplMessage* msg)
{
    long msg_type = msg->_proc_msg->_msg_type;
    long proc_type = msg->_proc_msg->_proc_type;
    if(proc_type == eSubProcedure_sim_cmd && msg_type == eSimCmd_start_sim){
        int ret = _sock_util_ptr->send_data(_comm_tbl._dev_ip.c_str(), _comm_tbl._dev_port, (const char*)&_comm_sim_delay, _comm_sim_delay.length);
        QString info  = LogUtil::Instance()->Output(MACRO_LOCAL, "send communication sim max handl delay(",
                                                    _comm_sim_delay.comm_sim_handle_delay, "s )", MACRO_SUCFAIL(ret));
        emit progress_log_signal(info);
        return;
    }

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

    QString info  = LogUtil::Instance()->Output(MACRO_LOCAL,
                                        "application name:", appl_name,
                                        "ss_id:", ss_id, "ps_id:", ps_id,
                                        "proc_type:", parse_type(proc_type),
                                        "msg_type:", parse_type(msg_type));
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
        _dst_dev_type = eSimDev_power_appl;

        snd_upper_to_comm();
        //rcv_upper_msg_callback(nullptr, 0);
    }
    else if(proc_type == eSubProcedure_session_end && msg_type == eMessage_request){
       doc = XmlUtil::generate_session_xml(ss_id, ps_id, DevNamesSet[eSimDev_power], eSubProcedure_session_end, eMessage_confirm);
       tips += "session end, confirm";
    }

    if(doc){
        emit snd_lower_signal(doc, QString::fromStdString(_sbs_ip), _sbs_port);

        info  = LogUtil::Instance()->Output(MACRO_LOCAL, "communication to power software subprogress information:", tips.toStdString());
        emit progress_log_signal(info);
    }
    else{
        LogUtil::Instance()->Output(MACRO_LOCAL, "generate QDomDocument failed",
                                    "proc_type:", parse_type(proc_type),
                                    "msg_type:", parse_type(msg_type));
    }
}

void client_proxy::handle_comm_power_appl(ApplMessage* msg)
{
    long msg_type = msg->_proc_msg->_msg_type;
    long proc_type = msg->_proc_msg->_proc_type;

    if(proc_type != _expect_proc_type || msg_type != _expect_msg_type){
        QString info  = LogUtil::Instance()->Output(MACRO_LOCAL, "unexpected status:",
                                                    "[current state]: proc_type:",  parse_type(proc_type),
                                                    "msg_type:", parse_type(msg_type),
                                                    "[expected state]: proc_type:", parse_type(_expect_proc_type),
                                                    "msg_type:", parse_type(_expect_msg_type));
        emit progress_log_signal(info);
        return;
    }

    int ss_id = msg->_u2i;
    int ps_id = msg->_i2u;
    const char* appl_name = ProcNamesSet[eApplProc_control_calc];

    QString info  = LogUtil::Instance()->Output(MACRO_LOCAL,
                                                "application name:", appl_name,
                                                "ss_id", ss_id, "ps_id", ps_id,
                                                "proc_type:",  parse_type(proc_type),
                                                "msg_type:", parse_type(msg_type));
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
        XmlUtil::parse_xml_power_appl_data(msg->_proc_msg->_func_invoke_body->_data, _dbl_vec, _union_sim_dat_snd_vec);
        _dst_dev_type = eSimDev_power;

        snd_upper_to_comm();

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

        info  = LogUtil::Instance()->Output(MACRO_LOCAL, "communication to power application software subprogress information:", tips.toStdString());
        emit progress_log_signal(info);
    }
    else{
        LogUtil::Instance()->Output(MACRO_LOCAL, "generate QDomDocument failed",
                                    "proc_type:", parse_type(proc_type),
                                    "msg_type:", parse_type(msg_type));
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
    LogUtil::Instance()->Output(MACRO_LOCAL);

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
    case eSimCmd_stop_sim: {
        switch(_dev_type){
        case eSimDev_power:{
            if(_power_handler){
                _power_handler->ExitHandler();
                QString info  = LogUtil::Instance()->Output(MACRO_LOCAL, "rcv stop sim cmd, close power simulation software");
                emit progress_log_signal(info);
            }
            break;
        }
        case eSimDev_power_appl: break;
        case eSimDev_communication:{
            string cmd = "exit " + _comm_conf_param.comm_cmd;
            bool ret = _sock_remote_ptr->send_data(_comm_tbl._comm_host_ip.c_str(), _comm_tbl._comm_host_port, cmd.c_str(), cmd.length());
            QString info  = LogUtil::Instance()->Output(MACRO_LOCAL, "send remote operate comm software cmd(exit)", MACRO_SUCFAIL(ret));
            emit progress_log_signal(info);
            break;
        }
        }
    }
    default: break;
    }
}

int client_proxy::parse_comm_cfg_param(ApplMessage* msg, char* value)
{
    if(!msg || !value){
        LogUtil::Instance()->Output(MACRO_LOCAL, "invalid arguments");
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

    QString info  = LogUtil::Instance()->Output(MACRO_LOCAL, "current PG_RTUI type:", msg->_pg_rtui_type);
    emit progress_log_signal(info);

    return len;
}


void client_proxy::handle_comm_cfg_param(ApplMessage* msg)
{
    if(msg->_pg_rtui_type == ePG_comm_sim_cmd_data){
        XmlUtil::parse_CommSimConfParam_xml(msg->_proc_msg->_data_vector, _comm_conf_param);

        string cmd = "start " + _comm_conf_param.comm_cmd;
        bool ret = _sock_remote_ptr->send_data(_comm_tbl._comm_host_ip.c_str(), _comm_tbl._comm_host_port, cmd.c_str(), cmd.length());
        QString info  = LogUtil::Instance()->Output(MACRO_LOCAL, "send remote operate communication software cmd(start)", MACRO_SUCFAIL(ret));
        emit progress_log_signal(info);

        //配置仿真最大处理时延
        memset(&_comm_sim_delay, 0, sizeof(PG_RTUI_Msg_CommSimHandleDelay));
        _comm_sim_delay.type = eCommCmd_sim_delay_cfg;
        _comm_sim_delay.length = sizeof(PG_RTUI_Msg_CommSimHandleDelay);
        _comm_sim_delay.comm_sim_handle_delay = _comm_conf_param.comm_sim_handle_max_delay;

        return;
    }

    char data[256] = {0};
    int len = parse_comm_cfg_param(msg, data);
    if(len <= 0){
        QString info  = LogUtil::Instance()->Output(MACRO_LOCAL, "msg invalid, parse communication config parameter failed");
        emit progress_log_signal(info);
        return;
    }

    QHostAddress host(_local_ip.c_str());
    uint32_t val_ip = host.toIPv4Address();
    memcpy(data, &val_ip, sizeof(uint32_t));
    bool ret = _sock_util_ptr->send_data(_comm_tbl._dev_ip.c_str(), _comm_tbl._dev_port, data, len);
    QString info  = LogUtil::Instance()->Output(MACRO_LOCAL, "send communication config parameter", MACRO_SUCFAIL(ret), "data len:", len);
    emit progress_log_signal(info);
}

void client_proxy::handle_power_cfg_param(ApplMessage* msg)
{
    if(_power_init_success){
        LogUtil::Instance()->Output(MACRO_LOCAL, "power has been initialized");
        return;
    }

    const DataXmlVec& data = msg->_proc_msg->_data_vector;
    if(!fetch_power_cfg_param(data)){
        return;
    }

    if(_dev_type == eSimDev_power_appl){
        return;
    }

    _power_handler = new PowerHandler();
    if(!_power_handler){
        QString info  = LogUtil::Instance()->Output(MACRO_LOCAL, "alloc PowerHandler memory failed");
        emit progress_log_signal(info);
        return;
    }

    if(_power_conf_param.dwstm_type == ePowerData_dginfor){
        _dwstm_info.resize(_power_conf_param.dwstm_num);
        for(int i=0; i<_power_conf_param.dwstm_num; ++i){
            _dwstm_info[i] = new PowerDGInforData();
            _dwstm_info[i]->data_type = ePowerData_dginfor;
        }
    }

    if(_power_conf_param.upstm_type == ePowerData_businfor){
        _upstm_info.resize(_power_conf_param.upstm_num);
        for(int i=0; i<_power_conf_param.upstm_num; ++i){
            _upstm_info[i] = new PowerBusInforData();
            _upstm_info[i]->data_length = sizeof(PowerBusInforData);
            _upstm_info[i]->data_type = ePowerData_businfor;
        }
    }

    //_dev_type == eSimDev_power
    _power_init_success = _power_handler->InitHandler(_power_conf_param.prj_name.c_str(),
                                                                                        _power_conf_param.case_name.c_str(),
                                                                                        _power_conf_param.sim_time,
                                                                                        _power_conf_param.sim_period);

    QString info  = LogUtil::Instance()->Output(MACRO_LOCAL, "Init power sim software", MACRO_SUCFAIL(_power_init_success));
    emit progress_log_signal(info);
}

void client_proxy::handle_msg(ApplMessage* msg)
{
    long msg_type = msg->_proc_msg->_msg_type;
    long proc_type = msg->_proc_msg->_proc_type;

    QString info  = LogUtil::Instance()->Output(MACRO_LOCAL,
                                                "msg name:", msg->_proc_msg->_msg_name,
                                                "ss_id:", msg->_i2u, "ps_id:", msg->_u2i,
                                                "proc_type:", parse_type(proc_type),
                                                "msg_type:", parse_type(msg_type));
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
       if(!(msg =  _proxy_msg_que.pop(!_quit))){
           continue;
       }

       handle_msg(msg);
       delete msg;
    }
}
