
#include <sstream>
#include <fstream>
#include <QNetworkInterface>
#include "xml_util.h"

XmlUtil::XmlUtil()
{

}

XmlUtil::~XmlUtil()
{

}

QDomDocument* XmlUtil::generate_conn_xml(const char* dev_name, uint32_t dev_ip, uint16_t dev_port, int ss_id, int ps_id, long reg_type, long msg_type, const char* err_msg)
{
    NetAddrMsgDataBody* net_addr = nullptr;
    if(dev_name){
        net_addr = new NetAddrMsgDataBody();
        net_addr->_device_ip = dev_ip;
        net_addr->_device_port = dev_port;
        net_addr->_device_name = dev_name;
    }

    VariableMsgDataBody* var_error = nullptr;
    if(msg_type == eMessage_error && err_msg){
        var_error = new VariableMsgDataBody();
        var_error->_var_name = "error cause";
        var_error->_var_type = eData_string;
        var_error->_var_value = err_msg;
    }

    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = reg_type == eSubProcedure_register ? "login" : "logout";
    proc_body->_proc_type = reg_type;
    proc_body->_msg_type = msg_type;
    if(net_addr) proc_body->_data_vector.push_back(net_addr);
    if(var_error) proc_body->_data_vector.push_back(var_error);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    if(net_addr)  delete net_addr;
    if(var_error) delete var_error;
    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

QDomDocument* XmlUtil::generate_reg_xml(int ss_id, int ps_id, const char* dev_name, long reg_type)
{
    if(!dev_name){
        return nullptr;
    }

    VariableMsgDataBody* var_1 = new VariableMsgDataBody();
    var_1->_var_name = "device name";
    var_1->_var_type = eData_string;
    var_1->_var_value = dev_name;

    VariableMsgDataBody* var_2 = new VariableMsgDataBody();
    var_2->_var_name = "oper type";                 //登录或登出
    var_2->_var_type = eData_int32;
    var_2->_var_value = QString::number(reg_type, 10).toStdString();

    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = reg_type == eSubProcedure_register ? "tips: login" : "tips: logout";
    proc_body->_proc_type = reg_type;
    proc_body->_msg_type = eMessage_notify;
    if(var_1) proc_body->_data_vector.push_back(var_1);
    if(var_2) proc_body->_data_vector.push_back(var_2);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    if(var_1)  delete var_1;
    if(var_2)  delete var_2;
    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

QDomDocument* XmlUtil::generate_session_xml(int ss_id, int ps_id, const char* dst_dev, long sess_type, long msg_type, const char* err_msg)
{
    if(!dst_dev){
        return nullptr;
    }

    VariableMsgDataBody* var = nullptr;
    if(msg_type != eMessage_error && msg_type != eMessage_notify){
        var = new VariableMsgDataBody();
        var->_var_name = "session destination device";
        var->_var_type = eData_string;
        var->_var_value = dst_dev;
    }
    else if(msg_type == eMessage_notify){
        var = new VariableMsgDataBody();
        var->_var_name = "session notify";
        var->_var_type = eData_string;
        var->_var_value = "note";
    }
    else if(err_msg && sess_type == eSubProcedure_session_begin){
        var = new VariableMsgDataBody();
        var->_var_name = "start session error";
        var->_var_type = eData_string;
        var->_var_value = err_msg;
    }

    const char* msg_name = "";
    switch(sess_type){
    case eSubProcedure_session_begin: msg_name = "start session"; break;
    case eSubProcedure_session_end: msg_name = "end session"; break;
    case eSubProcedure_test: msg_name = "heartbeat test"; break;
    default: break;
    }

    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = msg_name;
    proc_body->_proc_type = sess_type;
    proc_body->_msg_type = msg_type;
    if(var)  proc_body->_data_vector.push_back(var);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    if(var)  delete var;
    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

QDomDocument* XmlUtil::generate_appl_req_xml(int ss_id, int ps_id, const char* appl_name, const IntVec& steps,  long msg_type, const char* err_msg)
{
    VariableMsgDataBody* var = nullptr;
    ApplMessageBody* appl_body = nullptr;
    ApplStepMsgBody* steps_body = nullptr;
    if(msg_type != eMessage_error){
        if(!steps.empty()){
            steps_body = new ApplStepMsgBody();
            steps_body->_step_num = steps.size();

            for(int i=0; i<steps.size(); ++i){
                steps_body->_step_vec.push_back(i + 1);
                const char* proc_name = steps[i] == eSubProcedure_invoke ? "invoke" : "send data";
                steps_body->_proc_name_vec.push_back(proc_name);
            }
        }

        appl_body = new ApplMessageBody();
        appl_body->_appl_name = appl_name;
        appl_body->_appl_step = steps.size();
        appl_body->_msg_step = steps_body;
    }
    else if(err_msg || msg_type == eMessage_notify){
        var = new VariableMsgDataBody();
        var->_var_name = msg_type == eMessage_error ? "appl req error" : "application request notify";
        var->_var_type = eData_string;
        var->_var_value = msg_type == eMessage_error ? err_msg : "note";
    }

    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = appl_body;
    proc_body->_msg_name = "application request";
    proc_body->_proc_type = eSubProcedure_appl_request;
    proc_body->_msg_type = msg_type;
    if(var)  proc_body->_data_vector.push_back(var);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    if(var)  delete var;
    if(steps_body) delete steps_body;
    if(appl_body) delete appl_body;
    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

QDomDocument* XmlUtil::generate_snd_data_xml(int ss_id, int ps_id, const char* appl_name, long steps, const DataXmlVec& var, long msg_type)
{
    VariableMsgDataBody* var_note = nullptr;
    ApplMessageBody* appl_body = nullptr;
    if(msg_type != eMessage_notify){
        appl_body = new ApplMessageBody();
        appl_body->_appl_name = appl_name;
        appl_body->_appl_step = steps;
        appl_body->_msg_step = nullptr;
    }
    else{
        var_note = new VariableMsgDataBody();
        var_note->_var_name = "send data notify";
        var_note->_var_type = eData_string;
        var_note->_var_value = "note";
    }

    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = appl_body;
    proc_body->_msg_name = "send data";
    proc_body->_proc_type = eSubProcedure_data_send;
    proc_body->_msg_type = msg_type;
    if(!var_note) proc_body->_data_vector = var;
    else proc_body->_data_vector.push_back(var_note);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    if(var_note)  delete var_note;
    if(appl_body) delete appl_body;
    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

QDomDocument* XmlUtil::generate_invoke_xml(int ss_id, int ps_id, const char* invoke_name, const char* appl_name, long steps, const DataXmlVec& var, long msg_type, const char* err_msg)
{
    VariableMsgDataBody* var_ne = nullptr;
    ApplMessageBody* appl_body = nullptr;
    FuncInvokeBody* invoke_body = nullptr;
    if(msg_type != eMessage_error && msg_type != eMessage_notify){
        if(!var.empty()){
            invoke_body = new FuncInvokeBody();
            invoke_body->_name = invoke_name;
            invoke_body->_msg_type = eSubProcedure_invoke;
            invoke_body->_data = var;
        }

        appl_body = new ApplMessageBody();
        appl_body->_appl_name = appl_name;
        appl_body->_appl_step = steps;
        appl_body->_msg_step = nullptr;
    }
    else if(err_msg || msg_type == eMessage_notify){
        var_ne = new VariableMsgDataBody();
        var_ne->_var_name = msg_type == eMessage_error ? "invoke error" : "notify";
        var_ne->_var_type = eData_string;
        var_ne->_var_value = msg_type == eMessage_error ? err_msg  : "note";
    }

    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_func_invoke_body = invoke_body;
    proc_body->_appl_msg_body = appl_body;
    proc_body->_msg_name = "invoke";
    proc_body->_proc_type = eSubProcedure_invoke;
    proc_body->_msg_type = msg_type;
    if(var_ne) proc_body->_data_vector.push_back(var_ne);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    if(var_ne)  delete var_ne;
    if(invoke_body) delete invoke_body;
    if(appl_body) delete appl_body;
    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QDomDocument* XmlUtil::generate_notify_dev_id_xml(int ss_id, int ps_id, const IntMap& dev_ids)
{
    string str_dev_ids;
    IntMap::const_iterator it = dev_ids.cbegin();
    for(; it != dev_ids.cend(); ++it){
        str_dev_ids += std::to_string(it->first) + " " + std::to_string(it->second) + " ";
    }

    VariableMsgDataBody* var = new VariableMsgDataBody();
    var->_var_name = "dev-id list";
    var->_var_type = eData_string;
    var->_var_value = str_dev_ids;

    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = "dev_type_id_data";
    proc_body->_proc_type = eSubProcedure_dev_type_id_data;
    proc_body->_msg_type = eMessage_notify;
    proc_body->_data_vector.push_back(var);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    if(var)  delete var;
    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

bool XmlUtil::parse_device_id_type_data(const DataXmlVec& data, IntMap& type_id)
{
    if(data.empty()){
        return false;
    }

    type_id.clear();
    VariableMsgDataBody* var= (VariableMsgDataBody*)data[0];
    string dev_ids = var->_var_value;

    IntVec vec;  string val;
    std::istringstream istr(dev_ids);
    while(istr >> val){
        vec.push_back(std::stoi(val));
    }

    for(int i=0; i<vec.size(); i += 2){
        type_id[vec[i]] = vec[i + 1];
    }

    return true;
}

QDomDocument* XmlUtil::generate_sim_cmd_xml(int ss_id, int ps_id, int cmd)
{
    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = "";
    proc_body->_proc_type = eSubProcedure_sim_cmd;
    proc_body->_msg_type = cmd;

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

ESimDevType XmlUtil::query_sim_dev_type(const char* name)
{
    ESimDevType dev_type = eSimDev_undef;
    for(int i=eSimDev_power; i <= eSimDev_half_real_entity; ++i){
        if(strcmp(DevNamesSet[i], name) == 0){
            dev_type = (ESimDevType )i;
            break;
        }
    }

    return dev_type;
}

/****************动态控制界面各模块消息结构体******************/
PG_RTUI_Base* XmlUtil::parse_PG_RTUI_xml(EPGRTUIType type, const DataXmlVec& vec)
{
    bool ret = false;
    PG_RTUI_Base* data = nullptr;

    switch(type){
    case ePG_RTUI_msg_getinterfacenum:{
        data = new PG_RTUI_Msg_GetInterfaceNum;
        ret = XmlUtil::parse_PG_RTUI_Msg_GetInterfaceNum_xml(vec, *(PG_RTUI_Msg_GetInterfaceNum*)(data));
        break;
    }
    case ePG_RTUI_ack_getinterfacenum: {
        data = new PG_RTUI_Ack_GetInterfaceNum;
        ret = XmlUtil::parse_PG_RTUI_Ack_GetInterfaceNum_xml(vec, *(PG_RTUI_Ack_GetInterfaceNum*)(data));
        break;
    }
    case ePG_RTUI_msg_getinterface:{
        data = new PG_RTUI_Msg_GetInterface;
        ret = XmlUtil::parse_PG_RTUI_Msg_GetInterface_xml(vec, *(PG_RTUI_Msg_GetInterface*)(data));
        break;
    }
    case ePG_RTUI_ack_getinterface:{
        data = new PG_RTUI_Ack_GetInterface;
        ret = XmlUtil::parse_PG_RTUI_Ack_GetInterface_xml(vec, *(PG_RTUI_Ack_GetInterface*)(data));
        break;
    }
    case ePG_RTUI_msg_setinterface:{
        data = new PG_RTUI_Msg_SetInterface;
        ret = XmlUtil::parse_PG_RTUI_Msg_SetInterface_xml(vec, *(PG_RTUI_Msg_SetInterface*)(data));
        break;
    }
    case ePG_RTUI_msg_getlink:{
        data = new PG_RTUI_Msg_GetLink;
        ret = XmlUtil::parse_PG_RTUI_Msg_GetLink_xml(vec, *(PG_RTUI_Msg_GetLink*)(data));
        break;
    }
    case ePG_RTUI_ack_getlink:{
        data = new PG_RTUI_Ack_GetLink;
        ret = XmlUtil::parse_PG_RTUI_Ack_GetLink_xml(vec, *(PG_RTUI_Ack_GetLink*)(data));
        break;
    }
    case ePG_RTUI_msg_setlink: {
        data = new PG_RTUI_Msg_SetLink;
        ret = XmlUtil::parse_PG_RTUI_Msg_SetLink_xml(vec, *(PG_RTUI_Msg_SetLink*)(data));
        break;
    }
    case ePG_RTUI_change_node_status:{
        data = new PG_RTUI_ChangeNodeStatus;
        ret = XmlUtil::parse_PG_RTUI_ChangeNodeStatus_xml(vec, *(PG_RTUI_ChangeNodeStatus*)(data));
        break;
    }
    case ePG_RTUI_recover_node_status:{
        data = new PG_RTUI_ChangeNodeStatus;
        ret = XmlUtil::parse_PG_RTUI_ChangeNodeStatus_xml(vec, *(PG_RTUI_ChangeNodeStatus*)(data));
        break;
    }
    case ePG_RTUI_change_port_status: {
        data = new PG_RTUI_ChangePortStatus;
        ret = XmlUtil::parse_PG_RTUI_ChangePortStatus_xml(vec, *(PG_RTUI_ChangePortStatus*)(data));
        break;
    }
    case ePG_RTUI_recover_port_status:{
        data = new PG_RTUI_ChangePortStatus;
        ret = XmlUtil::parse_PG_RTUI_ChangePortStatus_xml(vec, *(PG_RTUI_ChangePortStatus*)(data));
        break;
    }
    case ePG_RTUI_static_route:{
        data = new PG_RTUI_StaticRoute;
        ret = XmlUtil::parse_PG_RTUI_StaticRoute_xml(vec, *(PG_RTUI_StaticRoute*)(data));
        break;
    }
    case ePG_RTUI_add_cbr: {
        data = new PG_RTUI_AddCBR;
        ret = XmlUtil::parse_PG_RTUI_AddCBR_xml(vec, *(PG_RTUI_AddCBR*)(data));
        break;
    }
    case ePG_RTUI_set_data_tamper_sim_time: {
        data = new PG_RTUI_SetDataTamperSimTime;
        ret = XmlUtil::parse_PG_RTUI_SetDataTamperSimTime_xml(vec, *(PG_RTUI_SetDataTamperSimTime*)(data));
        break;
    }
    case ePG_RTUI_set_data_tamper_last_time: {
        data = new PG_RTUI_SetDataTamperLastTime;
        ret = XmlUtil::parse_PG_RTUI_SetDataTamperLastTime_xml(vec, *(PG_RTUI_SetDataTamperLastTime*)(data));
        break;
    }
    case ePG_RTUI_stop_data_tamper: {
        data = new PG_RTUI_StopDataTamper;
        ret = XmlUtil::parse_PG_RTUI_StopDataTamper_xml(vec, *(PG_RTUI_StopDataTamper*)(data));
        break;
    }
    case ePG_RTUI_add_staticroute:
    case ePG_RTUI_remove_staticroute:{
        data = new PG_RTUI_StaticRoute;
        ret = XmlUtil::parse_PG_RTUI_StaticRoute_xml(vec, *(PG_RTUI_StaticRoute*)(data));
        break;
    }
    }

    return data;
}

QDomDocument* XmlUtil::create_PG_RTUI_xml(int ss_id, int ps_id, const PG_RTUI_Base* param)
{
    QDomDocument* doc = nullptr;
    switch(param->type){
    case ePG_RTUI_msg_getinterfacenum:{
        const PG_RTUI_Msg_GetInterfaceNum* pg = (PG_RTUI_Msg_GetInterfaceNum*)param;
        doc = XmlUtil::generate_PG_RTUI_Msg_GetInterfaceNum_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_ack_getinterfacenum:{
        const PG_RTUI_Ack_GetInterfaceNum* pg = (PG_RTUI_Ack_GetInterfaceNum*)param;
        doc = XmlUtil::generate_PG_RTUI_Ack_GetInterfaceNum_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_msg_getinterface:{
        const PG_RTUI_Msg_GetInterface* pg = (PG_RTUI_Msg_GetInterface*)param;
        doc = XmlUtil::generate_PG_RTUI_Msg_GetInterface_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_ack_getinterface:{
        const PG_RTUI_Ack_GetInterface* pg = (PG_RTUI_Ack_GetInterface*)param;
        doc = XmlUtil::generate_PG_RTUI_Ack_GetInterface_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_msg_setinterface:{
        const PG_RTUI_Msg_SetInterface* pg = (PG_RTUI_Msg_SetInterface*)param;
        doc = XmlUtil::generate_PG_RTUI_Msg_SetInterface_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_msg_getlink:{
        const PG_RTUI_Msg_GetLink* pg = (PG_RTUI_Msg_GetLink*)param;
        doc = XmlUtil::generate_PG_RTUI_GetLink_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_ack_getlink:{
        const PG_RTUI_Ack_GetLink* pg = (PG_RTUI_Ack_GetLink*)param;
        doc = XmlUtil::generate_PG_RTUI_Ack_GetLink_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_msg_setlink:{
        const PG_RTUI_Msg_SetLink* pg = (PG_RTUI_Msg_SetLink*)param;
        doc = XmlUtil::generate_PG_RTUI_Msg_SetLink_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_recover_node_status:
    case ePG_RTUI_change_node_status:{
        const PG_RTUI_ChangeNodeStatus* pg = (PG_RTUI_ChangeNodeStatus*)param;
        doc = XmlUtil::generate_PG_RTUI_ChangeNodeStatus_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_recover_port_status:
    case ePG_RTUI_change_port_status:{
        const PG_RTUI_ChangePortStatus* pg = (PG_RTUI_ChangePortStatus*)param;
        doc = XmlUtil::generate_PG_RTUI_ChangePortStatus_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_static_route:{
        const PG_RTUI_StaticRoute* pg = (PG_RTUI_StaticRoute*)param;
        doc = XmlUtil::generate_PG_RTUI_StaticRoute_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_add_cbr:{
        const PG_RTUI_AddCBR* pg = (PG_RTUI_AddCBR*)param;
        doc = XmlUtil::generate_PG_RTUI_AddCBR_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_set_data_tamper_sim_time:{
        const PG_RTUI_SetDataTamperSimTime* pg = (PG_RTUI_SetDataTamperSimTime*)param;
        doc = XmlUtil::generate_PG_RTUI_SetDataTamperSimulationTime_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_set_data_tamper_last_time:{
        const PG_RTUI_SetDataTamperLastTime* pg = (PG_RTUI_SetDataTamperLastTime*)param;
        doc = XmlUtil::generate_PG_RTUI_SetDataTamperLastTime_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_stop_data_tamper:{
        const PG_RTUI_StopDataTamper* pg = (PG_RTUI_StopDataTamper*)param;
        doc = XmlUtil::generate_PG_RTUI_StopDataTamper_xml(ss_id, ps_id, pg);
        break;
    }
    case ePG_RTUI_add_staticroute:
    case ePG_RTUI_remove_staticroute:{
            const PG_RTUI_StaticRoute* pg = (PG_RTUI_StaticRoute*)param;
            doc = XmlUtil::generate_PG_RTUI_StaticRoute_xml(ss_id, ps_id, pg);
            break;
        }
    default: break;
    }

    return doc;
}


QDomDocument*  XmlUtil::generate_PG_RTUI_Msg_GetInterfaceNum_xml(int ss_id, int ps_id,  const PG_RTUI_Msg_GetInterfaceNum* data)
{
     ProcedureMessageBody* proc_body = new ProcedureMessageBody();
     proc_body->_appl_msg_body = nullptr;
     proc_body->_msg_name = "PG_RTUI_Msg_GetInterfaceNum";
     proc_body->_proc_type = eSubProcedure_cfg_communication_data;
     proc_body->_msg_type = eMessage_notify;

     const int len = sizeof(PG_RTUI_Msg_GetInterfaceNum);
     QByteArray  d = QByteArray::fromRawData((const char*)data, len);

     VariableMsgDataBody* var = new VariableMsgDataBody();
     var->_var_name = "PG_RTUI_Msg_GetInterfaceNum";
     var->_var_type = eData_string;
     var->_var_value = d.toBase64().toStdString();
     proc_body->_data_vector.push_back(var);

     SessionMessageBody* sess_body = new SessionMessageBody();
     sess_body->_msg_type = ePG_RTUI_msg_getinterfacenum;
     sess_body->_id_i2u = ss_id;
     sess_body->_id_u2i = ps_id;
     sess_body->_procedure_msg_body = proc_body;

     RootMessageBody root;
     root._session_msg_body = sess_body;
     QDomDocument* doc = root.Attr2Document();

     if(var) delete var;
     if(proc_body) delete proc_body;
     if(sess_body) delete sess_body;

     return doc;
}

QDomDocument*  XmlUtil::generate_PG_RTUI_Ack_GetInterfaceNum_xml(int ss_id, int ps_id,
                                                                 const PG_RTUI_Ack_GetInterfaceNum* data)
{
     ProcedureMessageBody* proc_body = new ProcedureMessageBody();
     proc_body->_appl_msg_body = nullptr;
     proc_body->_msg_name = "PG_RTUI_Ack_GetInterfaceNum";
     proc_body->_proc_type = eSubProcedure_cfg_communication_data;
     proc_body->_msg_type = eMessage_notify;

     const int len = sizeof(PG_RTUI_Ack_GetInterfaceNum);
     QByteArray  d = QByteArray::fromRawData((const char*)data, len);

     VariableMsgDataBody* var = new VariableMsgDataBody();
     var->_var_name = "PG_RTUI_Ack_GetInterfaceNum";
     var->_var_type = eData_string;
     var->_var_value = d.toBase64().toStdString();
     proc_body->_data_vector.push_back(var);

     SessionMessageBody* sess_body = new SessionMessageBody();
     sess_body->_msg_type = ePG_RTUI_ack_getinterfacenum;
     sess_body->_id_i2u = ss_id;
     sess_body->_id_u2i = ps_id;
     sess_body->_procedure_msg_body = proc_body;

     RootMessageBody root;
     root._session_msg_body = sess_body;
     QDomDocument* doc = root.Attr2Document();

     if(var) delete var;
     if(proc_body) delete proc_body;
     if(sess_body) delete sess_body;

     return doc;
}

QDomDocument*  XmlUtil::generate_PG_RTUI_Msg_GetInterface_xml(int ss_id, int ps_id,
                                                              const PG_RTUI_Msg_GetInterface* data)
{
    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = "PG_RTUI_Msg_GetInterface";
    proc_body->_proc_type = eSubProcedure_cfg_communication_data;
    proc_body->_msg_type = eMessage_notify;

    const int len = sizeof(PG_RTUI_Msg_GetInterface);
     QByteArray  d = QByteArray::fromRawData((const char*)data, len);

    VariableMsgDataBody* var = new VariableMsgDataBody();
    var->_var_name = "PG_RTUI_Msg_GetInterface";
    var->_var_type = eData_string;
    var->_var_value = d.toBase64().toStdString();
    proc_body->_data_vector.push_back(var);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_msg_type = ePG_RTUI_msg_getinterface;
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    if(var) delete var;
    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

QDomDocument*  XmlUtil::generate_PG_RTUI_Ack_GetInterface_xml(int ss_id, int ps_id,  const PG_RTUI_Ack_GetInterface* data)
{
    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = "PG_RTUI_Ack_GetInterface";
    proc_body->_proc_type = eSubProcedure_cfg_communication_data;
    proc_body->_msg_type = eMessage_notify;

    const int len = sizeof(PG_RTUI_Ack_GetInterface);
     QByteArray  d = QByteArray::fromRawData((const char*)data, len);

    VariableMsgDataBody* var = new VariableMsgDataBody();
    var->_var_name = "PG_RTUI_Ack_GetInterface";
    var->_var_type = eData_string;
    var->_var_value = d.toBase64().toStdString();
    proc_body->_data_vector.push_back(var);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_msg_type = ePG_RTUI_ack_getinterface;
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    if(var) delete var;
    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

QDomDocument*  XmlUtil::generate_PG_RTUI_Msg_SetInterface_xml(int ss_id, int ps_id, const PG_RTUI_Msg_SetInterface* data)
{
    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = "PG_RTUI_Msg_SetInterface";
    proc_body->_proc_type = eSubProcedure_cfg_communication_data;
    proc_body->_msg_type = eMessage_notify;

    const int len = sizeof(PG_RTUI_Msg_SetInterface);
     QByteArray  d = QByteArray::fromRawData((const char*)data, len);

    VariableMsgDataBody* var = new VariableMsgDataBody();
    var->_var_name = "PG_RTUI_Msg_SetInterface";
    var->_var_type = eData_string;
    var->_var_value = d.toBase64().toStdString();
    proc_body->_data_vector.push_back(var);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_msg_type = ePG_RTUI_msg_setinterface;
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    if(var) delete var;
    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}



QDomDocument*  XmlUtil::generate_PG_RTUI_GetLink_xml(int ss_id, int ps_id, const PG_RTUI_Msg_GetLink* data)
{
    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = "PG_RTUI_GetLink";
    proc_body->_proc_type = eSubProcedure_cfg_communication_data;
    proc_body->_msg_type= eMessage_notify;

    const int len = sizeof(PG_RTUI_Msg_GetLink);
     QByteArray  d = QByteArray::fromRawData((const char*)data, len);

    VariableMsgDataBody* var = new VariableMsgDataBody();
    var->_var_name = "PG_RTUI_Msg_GetLink";
    var->_var_type = eData_string;
    var->_var_value = d.toBase64().toStdString();
    proc_body->_data_vector.push_back(var);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_msg_type = ePG_RTUI_msg_getlink;
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    if(var) delete var;
    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

QDomDocument*  XmlUtil::generate_PG_RTUI_Ack_GetLink_xml(int ss_id, int ps_id, const PG_RTUI_Ack_GetLink* data)
{
    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = "PG_RTUI_Ack_GetLink";
    proc_body->_proc_type = eSubProcedure_cfg_communication_data;
    proc_body->_msg_type = eMessage_notify;

    const int len = sizeof(PG_RTUI_Ack_GetLink);
     QByteArray  d = QByteArray::fromRawData((const char*)data, len);

    VariableMsgDataBody* var = new VariableMsgDataBody();
    var->_var_name = "PG_RTUI_Ack_GetLink";
    var->_var_type = eData_string;
    var->_var_value = d.toBase64().toStdString();
    proc_body->_data_vector.push_back(var);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_msg_type = ePG_RTUI_ack_getlink;
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    if(var) delete var;
    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

QDomDocument*  XmlUtil::generate_PG_RTUI_Msg_SetLink_xml(int ss_id, int ps_id, const PG_RTUI_Msg_SetLink* data)
{
     ProcedureMessageBody* proc_body = new ProcedureMessageBody();
     proc_body->_appl_msg_body = nullptr;
     proc_body->_msg_name = "PG_RTUI_Ack_SetLink";
     proc_body->_proc_type = eSubProcedure_cfg_communication_data;
     proc_body->_msg_type = eMessage_notify;

     const int len = sizeof(PG_RTUI_Msg_SetLink);
     QByteArray  d = QByteArray::fromRawData((const char*)data, len);

     VariableMsgDataBody* var = new VariableMsgDataBody();
     var->_var_name = "PG_RTUI_Ack_SetLink";
     var->_var_type = eData_string;
     var->_var_value = d.toBase64().toStdString();
     proc_body->_data_vector.push_back(var);

     SessionMessageBody* sess_body = new SessionMessageBody();
     sess_body->_msg_type = ePG_RTUI_msg_setlink;
     sess_body->_id_i2u = ss_id;
     sess_body->_id_u2i = ps_id;
     sess_body->_procedure_msg_body = proc_body;

     RootMessageBody root;
     root._session_msg_body = sess_body;
     QDomDocument* doc = root.Attr2Document();

     if(var) delete var;
     if(proc_body) delete proc_body;
     if(sess_body) delete sess_body;

     return doc;
}

QDomDocument*  XmlUtil::generate_PG_RTUI_ChangeNodeStatus_xml(int ss_id, int ps_id, const PG_RTUI_ChangeNodeStatus* data)
{
    string messName = data->type==ePG_RTUI_change_node_status?"PG_RTUI_changeNodeStatus":"PG_RTUI_recoverNodeStatus";
    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = messName;
    proc_body->_proc_type = eSubProcedure_cfg_communication_data;
    proc_body->_msg_type = eMessage_notify;

    const int len = sizeof(PG_RTUI_ChangeNodeStatus);
     QByteArray  d = QByteArray::fromRawData((const char*)data, len);

    VariableMsgDataBody* var = new VariableMsgDataBody();
    var->_var_name = messName;
    var->_var_type = eData_string;
    var->_var_value = d.toBase64().toStdString();
    proc_body->_data_vector.push_back(var);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_msg_type = data->type;
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    if(var) delete var;
    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

QDomDocument*  XmlUtil::generate_PG_RTUI_ChangePortStatus_xml(int ss_id, int ps_id, const PG_RTUI_ChangePortStatus* data)
{
    string messName = data->type==ePG_RTUI_change_port_status?"PG_RTUI_changePortStatus":"PG_RTUI_recoverPortStatus";
    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = messName;
    proc_body->_proc_type = eSubProcedure_cfg_communication_data;
    proc_body->_msg_type = eMessage_notify;

    const int len = sizeof(PG_RTUI_ChangePortStatus);
     QByteArray  d = QByteArray::fromRawData((const char*)data, len);

    VariableMsgDataBody* var = new VariableMsgDataBody();
    var->_var_name = messName;
    var->_var_type = eData_string;
    var->_var_value = d.toBase64().toStdString();
    proc_body->_data_vector.push_back(var);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_msg_type = data->type;
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();
    XmlUtil::generate_xml_file("aaaa.xml",doc);

    if(var) delete var;
    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

QDomDocument*  XmlUtil::generate_PG_RTUI_StaticRoute_xml(int ss_id, int ps_id, const PG_RTUI_StaticRoute* data)
{
    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = "PG_RTUI_StaticRoute";
    proc_body->_proc_type = eSubProcedure_cfg_communication_data;
    proc_body->_msg_type = eMessage_notify;

    const int len = sizeof(PG_RTUI_StaticRoute);
     QByteArray  d = QByteArray::fromRawData((const char*)data, len);

    VariableMsgDataBody* var = new VariableMsgDataBody();
    var->_var_name = "PG_RTUI_StaticRoute";
    var->_var_type = eData_string;
    var->_var_value = d.toBase64().toStdString();
    proc_body->_data_vector.push_back(var);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_msg_type = data->type;
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    if(var) delete var;
    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

QDomDocument*  XmlUtil::generate_PG_RTUI_AddCBR_xml(int ss_id, int ps_id, const PG_RTUI_AddCBR* data)
{
    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = "PG_RTUI_AddCBR";
    proc_body->_proc_type = eSubProcedure_cfg_communication_data;
    proc_body->_msg_type = eMessage_notify;

    const int len = sizeof(PG_RTUI_AddCBR);
     QByteArray  d = QByteArray::fromRawData((const char*)data, len);

    VariableMsgDataBody* var = new VariableMsgDataBody();
    var->_var_name = "PG_RTUI_AddCBR";
    var->_var_type = eData_string;
    var->_var_value = d.toBase64().toStdString();
    proc_body->_data_vector.push_back(var);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_msg_type = ePG_RTUI_add_cbr;
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body ;
    QDomDocument* doc = root.Attr2Document();

    if(var) delete var;
    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

QDomDocument*  XmlUtil::generate_PG_RTUI_SetDataTamperSimulationTime_xml(int ss_id, int ps_id, const PG_RTUI_SetDataTamperSimTime* data)
{
    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = "PG_RTUI_SetDataTamperSimulationTime";
    proc_body->_proc_type = eSubProcedure_cfg_communication_data;
    proc_body->_msg_type = eMessage_notify;

    const int len = sizeof(PG_RTUI_SetDataTamperSimTime);
     QByteArray  d = QByteArray::fromRawData((const char*)data, len);

    VariableMsgDataBody* var = new VariableMsgDataBody();
    var->_var_name = "PG_RTUI_SetDataTamperSimTime";
    var->_var_type = eData_string;
    var->_var_value = d.toBase64().toStdString();
    proc_body->_data_vector.push_back(var);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_msg_type = ePG_RTUI_set_data_tamper_sim_time;
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    if(var) delete var;
    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

QDomDocument*  XmlUtil::generate_PG_RTUI_SetDataTamperLastTime_xml(int ss_id, int ps_id, const PG_RTUI_SetDataTamperLastTime* data)
{
    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = "PG_RTUI_SetDataTamperLastTime";
    proc_body->_proc_type = eSubProcedure_cfg_communication_data;
    proc_body->_msg_type = eMessage_notify;

    const int len = sizeof(PG_RTUI_SetDataTamperLastTime);
     QByteArray  d = QByteArray::fromRawData((const char*)data, len);

    VariableMsgDataBody* var = new VariableMsgDataBody();
    var->_var_name = "PG_RTUI_SetDataTamperLastTime";
    var->_var_type = eData_string;
    var->_var_value = d.toBase64().toStdString();
    proc_body->_data_vector.push_back(var);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_msg_type = ePG_RTUI_set_data_tamper_last_time;
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    if(var) delete var;
    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

QDomDocument*  XmlUtil::generate_PG_RTUI_StopDataTamper_xml(int ss_id, int ps_id, const PG_RTUI_StopDataTamper* data)
{
    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = "PG_RTUI_StopDataTamper";
    proc_body->_proc_type = eSubProcedure_cfg_communication_data;
    proc_body->_msg_type = eMessage_notify;

    const int len = sizeof(PG_RTUI_StopDataTamper);
     QByteArray  d = QByteArray::fromRawData((const char*)data, len);

    VariableMsgDataBody* var = new VariableMsgDataBody();
    var->_var_name = "PG_RTUI_StopDataTamper";
    var->_var_type = eData_string;
    var->_var_value = d.toBase64().toStdString();
    proc_body->_data_vector.push_back(var);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_msg_type =  ePG_RTUI_stop_data_tamper;
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    if(var) delete var;
    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

/********************************************************************/
//控制台联合仿真参数配置.
bool XmlUtil::parse_UnionSimConfParam_xml(const DataXmlVec& vec, UnionSimConfParam& data)
{
    VariableMsgDataBody* var_elec_ctrl_prd = (VariableMsgDataBody*)vec[0];
    data.power_ctrl_prd = std::stod(var_elec_ctrl_prd->_var_value);

    VariableMsgDataBody* var_fed_sim_time = (VariableMsgDataBody*)vec[1];
    data.fed_sim_time = std::stod(var_fed_sim_time->_var_value);

    VariableMsgDataBody* var_time_prop_step = (VariableMsgDataBody*)vec[2];
    data.time_prop_step = std::stod(var_time_prop_step->_var_value);

    VariableMsgDataBody* var_syn_type = (VariableMsgDataBody*)vec[3];
    data.syn_type = (ESynchronizeType)std::stoi(var_syn_type->_var_value);

    VariableMsgDataBody* var_proto_type = (VariableMsgDataBody*)vec[4];
    data.proto_type = (EProtocolType)std::stoi(var_proto_type->_var_value);

    return true;
}

QDomDocument*  XmlUtil::generate_UnionSimConfParam_xml(int ss_id, int ps_id, const UnionSimConfParam* data)
{
    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = "UnionSimConfParam";
    proc_body->_proc_type = eSubProcedure_cfg_sim_param_data;
    proc_body->_msg_type = eMessage_notify;

    //double elec_ctrl_prd
    VariableMsgDataBody* var_elec_ctrl_prd = new VariableMsgDataBody();
    var_elec_ctrl_prd->_var_name = "elec_ctrl_prd";
    var_elec_ctrl_prd->_var_type = eData_double;
    var_elec_ctrl_prd->_var_value = std::to_string(data->power_ctrl_prd);
    //double fed_sim_time
    VariableMsgDataBody* var_fed_sim_time = new VariableMsgDataBody();
    var_fed_sim_time->_var_name = "fed_sim_time";
    var_fed_sim_time->_var_type = eData_double;
    var_fed_sim_time->_var_value = std::to_string(data->fed_sim_time);
    //double time_prop_step;
    VariableMsgDataBody* var_time_prop_step = new VariableMsgDataBody();
    var_time_prop_step->_var_name = "time_prop_step";
    var_time_prop_step->_var_type = eData_double;
    var_time_prop_step->_var_value = std::to_string(data->time_prop_step);
    //ESynchronizeType syn_type;
    VariableMsgDataBody* var_syn_type = new VariableMsgDataBody();
    var_syn_type->_var_name = "syn_type";
    var_syn_type->_var_type = eData_uint16;
    var_syn_type->_var_value = std::to_string(data->syn_type);
    //ESynchronizeType proto_type;
    VariableMsgDataBody* var_proto_type = new VariableMsgDataBody();
    var_proto_type->_var_name = "proto_type";
    var_proto_type->_var_type = eData_uint16;
    var_proto_type->_var_value = std::to_string(data->proto_type);

    proc_body->_data_vector.push_back(var_elec_ctrl_prd);
    proc_body->_data_vector.push_back(var_fed_sim_time);
    proc_body->_data_vector.push_back(var_time_prop_step);
    proc_body->_data_vector.push_back(var_syn_type);
    proc_body->_data_vector.push_back(var_proto_type);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    for(int i=0; i<proc_body->_data_vector.size(); ++i){
        delete proc_body->_data_vector[i];
    }

    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

bool XmlUtil::load_PowerSimConfParam_cfg(const char* cfg_path, PowerConfParam& data)
{
    if(!cfg_path){
        return false;
    }

    std::ifstream in(cfg_path);
    if(in.fail()){
        return false;
    }

    string param;
    StrVec cfg_par_vec;
    while(std::getline(in, param)){
        if(param.empty()){
            continue;
        }

        int index = param.find_first_of(':');
        if(index != string::npos){
            param = param.substr(index + 1);
        }

        if(!param.empty()){
            cfg_par_vec.push_back(param);
        }
    }

    if(cfg_par_vec.size() < 8){
        return false;
    }

    data.prj_name = cfg_par_vec[0];
    data.case_name = cfg_par_vec[1];
    data.sim_time = std::stod(cfg_par_vec[2]);
    data.sim_period = std::stod(cfg_par_vec[3]);
    data.input_type = std::stoi(cfg_par_vec[4]);
    data.result_type = std::stoi(cfg_par_vec[5]);
    data.input_num = std::stoi(cfg_par_vec[6]);
    data.result_num = std::stoi(cfg_par_vec[7]);

    if(cfg_par_vec.size() > 8){
        string nodes_map_tbl;
        for(int i=5; i<cfg_par_vec.size(); ++i){
            nodes_map_tbl += cfg_par_vec[i] + " ";
        }

        QByteArray d = QByteArray::fromRawData(nodes_map_tbl.c_str(), nodes_map_tbl.length());
        QByteArray cd = qCompress(d, 5);
        QByteArray encd = cd.toBase64();
        QByteArray decd = QByteArray::fromBase64(encd);
        QByteArray ud = qUncompress(decd);
        data.nodes_map = nodes_map_tbl;
    }

    return true;
}

bool  XmlUtil::parse_PowerSimConfParam_xml(const DataXmlVec& vec, PowerConfParam& data)
{
    VariableMsgDataBody* var_prj_name = (VariableMsgDataBody*)vec[0];
    data.prj_name = var_prj_name->_var_value;

    VariableMsgDataBody* var_case_name = (VariableMsgDataBody*)vec[1];
    data.case_name = var_case_name->_var_value;

    VariableMsgDataBody* var_sim_time = (VariableMsgDataBody*)vec[2];
    data.sim_time = std::stod(var_sim_time->_var_value);

    VariableMsgDataBody* var_sim_period = (VariableMsgDataBody*)vec[3];
    data.sim_period = std::stod(var_sim_period->_var_value);

    VariableMsgDataBody* var_input_type= (VariableMsgDataBody*)vec[4];
    data.input_type = std::stoi(var_input_type->_var_value);

    VariableMsgDataBody* var_result_type= (VariableMsgDataBody*)vec[5];
    data.result_type = std::stoi(var_result_type->_var_value);

    VariableMsgDataBody* var_input_num= (VariableMsgDataBody*)vec[6];
    data.input_num = std::stoi(var_input_num->_var_value);

    VariableMsgDataBody* var_result_num= (VariableMsgDataBody*)vec[7];
    data.result_num = std::stoi(var_result_num->_var_value);

    VariableMsgDataBody* var_nodes_map = (VariableMsgDataBody*)vec[8];
    data.nodes_map = var_nodes_map->_var_value;

    return true;
}
QDomDocument* XmlUtil::generate_PowerSimConfParam_xml(int ss_id, int ps_id, const PowerConfParam* data)
{
    if(!data){
        return nullptr;
    }

    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = "PowerSimConfParam";
    proc_body->_proc_type = eSubProcedure_cfg_power_data;
    proc_body->_msg_type = eMessage_notify;

    VariableMsgDataBody* var_prj_name = new VariableMsgDataBody();
    var_prj_name->_var_name = "prj_name";
    var_prj_name->_var_type = eData_string;
    var_prj_name->_var_value = data->prj_name;
    proc_body->_data_vector.push_back(var_prj_name);

    VariableMsgDataBody* var_case_name = new VariableMsgDataBody();
    var_case_name->_var_name = "case_name";
    var_case_name->_var_type = eData_string;
    var_case_name->_var_value = data->case_name;
    proc_body->_data_vector.push_back(var_case_name);

    VariableMsgDataBody* var_sim_time = new VariableMsgDataBody();
    var_sim_time->_var_name = "sim_time";
    var_sim_time->_var_type = eData_double;
    var_sim_time->_var_value = std::to_string(data->sim_time);
    proc_body->_data_vector.push_back(var_sim_time);

    VariableMsgDataBody* var_sim_period = new VariableMsgDataBody();
    var_sim_period->_var_name = "sim_period";
    var_sim_period->_var_type = eData_double;
    var_sim_period->_var_value = std::to_string(data->sim_period);
    proc_body->_data_vector.push_back(var_sim_period);

    VariableMsgDataBody* var_input_type= new VariableMsgDataBody();
    var_input_type->_var_name = "input type";
    var_input_type->_var_type = eData_int32;
    var_input_type->_var_value = std::to_string(data->input_type);
    proc_body->_data_vector.push_back(var_input_type);

    VariableMsgDataBody* var_result_type= new VariableMsgDataBody();
    var_result_type->_var_name = "result type";
    var_result_type->_var_type = eData_int32;
    var_result_type->_var_value = std::to_string(data->result_type);
    proc_body->_data_vector.push_back(var_result_type);

    VariableMsgDataBody* var_input_num= new VariableMsgDataBody();
    var_input_num->_var_name = "input num";
    var_input_num->_var_type = eData_int32;
    var_input_num->_var_value = std::to_string(data->input_num);
    proc_body->_data_vector.push_back(var_input_num);

    VariableMsgDataBody* var_result_num= new VariableMsgDataBody();
    var_result_num->_var_name = "result num";
    var_result_num->_var_type = eData_int32;
    var_result_num->_var_value = std::to_string(data->result_num);
    proc_body->_data_vector.push_back(var_result_num);

    VariableMsgDataBody* var_nodes_map= new VariableMsgDataBody();
    var_nodes_map->_var_name = "nodes_map";
    var_nodes_map->_var_type = eData_string;
    var_nodes_map->_var_value = data->nodes_map;
    proc_body->_data_vector.push_back(var_nodes_map);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    for(int i=0; i<proc_body->_data_vector.size(); ++i){
        delete proc_body->_data_vector[i];
    }

    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

bool XmlUtil::load_CommConfParam_cfg(const char* cfg_path, CommConfParam& data)
{
    if(!cfg_path){
        return false;
    }

    std::ifstream in(cfg_path);
    if(in.fail()){
        return false;
    }

    string param;
    StrVec cfg_par_vec;
    while(std::getline(in, param)){
        if(param.empty()){
            continue;
        }

        int index = param.find_first_of(':');
        if(index != string::npos){
            param = param.substr(index + 1);
        }

        if(!param.empty()){
            cfg_par_vec.push_back(param);
        }
    }

    if(cfg_par_vec.size() < 2){
        return false;
    }

    data.comm_cmd = cfg_par_vec[0];
    data.comm_sim_handle_max_delay = std::stoul(cfg_par_vec[1]);

    return true;
}

bool XmlUtil::parse_CommSimConfParam_xml(const DataXmlVec& vec, CommConfParam& data)
{
    VariableMsgDataBody* var_comm_cmd = (VariableMsgDataBody*)vec[0];
    data.comm_cmd = var_comm_cmd->_var_value;

    VariableMsgDataBody* var_max_delay = (VariableMsgDataBody*)vec[1];
    data.comm_sim_handle_max_delay = std::stoul(var_max_delay->_var_value);

    return true;
}

QDomDocument* XmlUtil::generate_CommSimConfParam_xml(int ss_id, int ps_id, const CommConfParam* data)
{
    if(!data){
        return nullptr;
    }

    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = "CommSimConfParam";
    proc_body->_proc_type = eSubProcedure_cfg_communication_data;
    proc_body->_msg_type = eMessage_notify;

    VariableMsgDataBody* var_comm_cmd = new VariableMsgDataBody();
    var_comm_cmd->_var_name = "comm operation cmd";
    var_comm_cmd->_var_type = eData_string;
    var_comm_cmd->_var_value = data->comm_cmd;
    proc_body->_data_vector.push_back(var_comm_cmd);

    VariableMsgDataBody* var_max_delay = new VariableMsgDataBody();
    var_max_delay->_var_name = "comm sim handle max delay";
    var_max_delay->_var_type = eData_uint32;
    var_max_delay->_var_value = std::to_string(data->comm_sim_handle_max_delay);
    proc_body->_data_vector.push_back(var_max_delay);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_msg_type = ePG_comm_sim_cmd_data;
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    for(int i=0; i<proc_body->_data_vector.size(); ++i){
        delete proc_body->_data_vector[i];
    }

    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

QDomDocument* XmlUtil::generate_sim_time_notify_xml(int ss_id, int ps_id, double sim_time)
{
    VariableMsgDataBody* var_time = new VariableMsgDataBody();
    var_time->_var_name = "sim time";
    var_time->_var_type = eData_double;
    var_time->_var_value = std::to_string(sim_time);

    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = "sim time notify";
    proc_body->_proc_type = eSubProcedure_sim_time_notify_data;
    proc_body->_msg_type = eMessage_notify;
    proc_body->_data_vector.push_back(var_time);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    if(var_time) delete var_time;
    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;

    return doc;
}

void XmlUtil::generate_xml_file(const std::string& fileName, QDomDocument* doc)
{
    QFile file(QString::fromStdString(fileName));        //写xml文件
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        return ;
    }

    QTextStream out(&file);
    doc->save(out, 4);
}

//解析.
/*******************************************************/
bool XmlUtil::parse_PG_RTUI_Msg_GetInterfaceNum_xml(const DataXmlVec& vec,PG_RTUI_Msg_GetInterfaceNum &data )
{
    if(vec.size() == 0){
        return false;
    }

    VariableMsgDataBody* vb = (VariableMsgDataBody*)vec[0];
    QByteArray od = QByteArray::fromBase64(QByteArray::fromRawData(vb->_var_value.c_str(), vb->_var_value.length()));
    data = *(PG_RTUI_Msg_GetInterfaceNum*)od.data();

    return true;
}

bool XmlUtil::parse_PG_RTUI_Ack_GetInterfaceNum_xml(const DataXmlVec& vec,PG_RTUI_Ack_GetInterfaceNum &data )
{
    if(vec.size() == 0){
        return false;
    }

    VariableMsgDataBody* vb = (VariableMsgDataBody*)vec[0];
    QByteArray od = QByteArray::fromBase64(QByteArray::fromRawData(vb->_var_value.c_str(), vb->_var_value.length()));
    data = *(PG_RTUI_Ack_GetInterfaceNum*)od.data();

    return true;
}

bool XmlUtil::parse_PG_RTUI_Msg_GetInterface_xml(const DataXmlVec& vec,PG_RTUI_Msg_GetInterface &data )
{
    if(vec.size() == 0){
        return false;
    }

    VariableMsgDataBody* vb = (VariableMsgDataBody*)vec[0];
    QByteArray od = QByteArray::fromBase64(QByteArray::fromRawData(vb->_var_value.c_str(), vb->_var_value.length()));
    data = *(PG_RTUI_Msg_GetInterface*)od.data();

    return true;
}

bool XmlUtil::parse_PG_RTUI_Ack_GetInterface_xml(const DataXmlVec& vec,PG_RTUI_Ack_GetInterface &data )
{
    if(vec.size() == 0){
        return false;
    }

    VariableMsgDataBody* vb = (VariableMsgDataBody*)vec[0];
    QByteArray od = QByteArray::fromBase64(QByteArray::fromRawData(vb->_var_value.c_str(), vb->_var_value.length()));
    data = *(PG_RTUI_Ack_GetInterface*)od.data();

    return true;
}

bool XmlUtil::parse_PG_RTUI_Msg_SetInterface_xml(const DataXmlVec& vec,PG_RTUI_Msg_SetInterface &data )
{
    if(vec.size() == 0){
        return false;
    }

    VariableMsgDataBody* vb = (VariableMsgDataBody*)vec[0];
    QByteArray od = QByteArray::fromBase64(QByteArray::fromRawData(vb->_var_value.c_str(), vb->_var_value.length()));
    data = *(PG_RTUI_Msg_SetInterface*)od.data();

    return true;
}


bool XmlUtil::parse_PG_RTUI_Msg_GetLink_xml(const DataXmlVec& vec,PG_RTUI_Msg_GetLink &data )
{
    if(vec.size() == 0){
        return false;
    }

    VariableMsgDataBody* vb = (VariableMsgDataBody*)vec[0];
    QByteArray od = QByteArray::fromBase64(QByteArray::fromRawData(vb->_var_value.c_str(), vb->_var_value.length()));
    data = *(PG_RTUI_Msg_GetLink*)od.data();

    return true;
}

bool XmlUtil::parse_PG_RTUI_Ack_GetLink_xml(const DataXmlVec& vec,PG_RTUI_Ack_GetLink &data )
{
    if(vec.size() == 0){
        return false;
    }

    VariableMsgDataBody* vb = (VariableMsgDataBody*)vec[0];
    QByteArray od = QByteArray::fromBase64(QByteArray::fromRawData(vb->_var_value.c_str(), vb->_var_value.length()));
    data = *(PG_RTUI_Ack_GetLink*)od.data();

    return true;
}

bool XmlUtil::parse_PG_RTUI_Msg_SetLink_xml(const DataXmlVec& vec,PG_RTUI_Msg_SetLink &data )
{
    if(vec.size() == 0){
        return false;
    }

    VariableMsgDataBody* vb = (VariableMsgDataBody*)vec[0];
    QByteArray od = QByteArray::fromBase64(QByteArray::fromRawData(vb->_var_value.c_str(), vb->_var_value.length()));
    data = *(PG_RTUI_Msg_SetLink*)od.data();

    return true;
}


bool XmlUtil::parse_PG_RTUI_ChangeNodeStatus_xml(const DataXmlVec& vec,PG_RTUI_ChangeNodeStatus &data )
{
    if(vec.size() == 0){
        return false;
    }

    VariableMsgDataBody* vb = (VariableMsgDataBody*)vec[0];
    QByteArray od = QByteArray::fromBase64(QByteArray::fromRawData(vb->_var_value.c_str(), vb->_var_value.length()));
    data = *(PG_RTUI_ChangeNodeStatus*)od.data();

    return true;
}

bool XmlUtil::parse_PG_RTUI_ChangePortStatus_xml(const DataXmlVec& vec,PG_RTUI_ChangePortStatus &data )
{
    if(vec.size() == 0){
        return false;
    }

    VariableMsgDataBody* vb = (VariableMsgDataBody*)vec[0];
    QByteArray od = QByteArray::fromBase64(QByteArray::fromRawData(vb->_var_value.c_str(), vb->_var_value.length()));
    data = *(PG_RTUI_ChangePortStatus*)od.data();

    return true;
}

bool XmlUtil::parse_PG_RTUI_StaticRoute_xml(const DataXmlVec& vec,PG_RTUI_StaticRoute &data )
{
    if(vec.size() == 0){
        return false;
    }

    VariableMsgDataBody* vb = (VariableMsgDataBody*)vec[0];
    QByteArray od = QByteArray::fromBase64(QByteArray::fromRawData(vb->_var_value.c_str(), vb->_var_value.length()));
    data = *(PG_RTUI_StaticRoute*)od.data();

    return true;
}

bool XmlUtil::parse_PG_RTUI_AddCBR_xml(const DataXmlVec& vec,PG_RTUI_AddCBR &data )
{
    if(vec.size() == 0){
        return false;
    }

    VariableMsgDataBody* vb = (VariableMsgDataBody*)vec[0];
    QByteArray od = QByteArray::fromBase64(QByteArray::fromRawData(vb->_var_value.c_str(), vb->_var_value.length()));
    data = *(PG_RTUI_AddCBR*)od.data();

    return true;
}

bool XmlUtil::parse_PG_RTUI_SetDataTamperSimTime_xml(const DataXmlVec& vec,PG_RTUI_SetDataTamperSimTime &data )
{
    if(vec.size() == 0){
        return false;
    }

    VariableMsgDataBody* vb = (VariableMsgDataBody*)vec[0];
    QByteArray od = QByteArray::fromBase64(QByteArray::fromRawData(vb->_var_value.c_str(), vb->_var_value.length()));
    data = *(PG_RTUI_SetDataTamperSimTime*)od.data();

    return true;
}

bool XmlUtil::parse_PG_RTUI_SetDataTamperLastTime_xml(const DataXmlVec& vec,PG_RTUI_SetDataTamperLastTime &data )
{
    if(vec.size() == 0){
        return false;
    }

    VariableMsgDataBody* vb = (VariableMsgDataBody*)vec[0];
    QByteArray od = QByteArray::fromBase64(QByteArray::fromRawData(vb->_var_value.c_str(), vb->_var_value.length()));
    data = *(PG_RTUI_SetDataTamperLastTime*)od.data();

    return true;
}

bool XmlUtil::parse_PG_RTUI_StopDataTamper_xml(const DataXmlVec& vec,PG_RTUI_StopDataTamper &data )
{
    if(vec.size() == 0){
        return false;
    }

    VariableMsgDataBody* vb = (VariableMsgDataBody*)vec[0];
    QByteArray od = QByteArray::fromBase64(QByteArray::fromRawData(vb->_var_value.c_str(), vb->_var_value.length()));
    data = *(PG_RTUI_StopDataTamper*)od.data();

    return true;
}

//协同仿真数据
void XmlUtil::parse_xml_power_sim_data(const DataXmlVec& vec, UnionSimDatVec& data)
{
    data.clear();
    UnionSimData dat;

    for(int i=0; i<vec.size(); ++i){
        VariableMsgDataBody* var = (VariableMsgDataBody*)vec[i];
        QByteArray d_base64 = QByteArray::fromRawData(var->_var_value.c_str(), var->_var_value.length());
        QByteArray d = QByteArray::fromBase64(d_base64);
        memcpy(&dat, d.data(), d.length());
        data.push_back(dat);
    }
}

void XmlUtil::generate_xml_power_sim_data(const UnionSimDatVec& data, DataXmlVec& vec)
{
    const int dat_size = sizeof(UnionSimData);
    for(int i=0; i<data.size(); ++i){
        QByteArray d = QByteArray::fromRawData((const char*)&data[i], dat_size);
        QByteArray d_base64 = d.toBase64();

        VariableMsgDataBody* var = new VariableMsgDataBody();
        var->_var_name = "UnionSimData";
        var->_var_type = eData_string;
        var->_var_value = d_base64.toStdString();
        vec.push_back(var);
    }
}

void XmlUtil::parse_xml_power_appl_data(const DataXmlVec& vec, DblVec& data, UnionSimDatVec& us_data)
{
    data.clear();
    us_data.clear();

    if(vec.empty()){
        return;
    }

    DataXmlVec& xml_vec = const_cast<DataXmlVec&>(vec);
    VariableMsgDataBody* var = (VariableMsgDataBody*)xml_vec.back();
    xml_vec.pop_back();

    parse_xml_power_sim_data(xml_vec, us_data);

    QByteArray d_base64 = QByteArray::fromRawData(var->_var_value.c_str(), var->_var_value.length());
    QByteArray d = QByteArray::fromBase64(d_base64);

    double dvg;
    const int db_size = sizeof(double);
    for(int i=0; i<d.length(); i+=db_size){
        memcpy(&dvg, d.data() + i, db_size);
        data.push_back(dvg);
    }
}

void XmlUtil::generate_xml_power_appl_data(const DblVec& data, const UnionSimDatVec& us_data, DataXmlVec& vec)
{
    vec.clear();
    generate_xml_power_sim_data(us_data, vec);

    char dvg[1024 * 100] = {0};
    const int db_size = sizeof(double);

    for(int i=0; i<data.size(); ++i){
        memcpy(dvg + i *db_size, &data[i], db_size);
    }

    int dvg_size = db_size * data.size();
    QByteArray d = QByteArray::fromRawData(dvg, dvg_size);
    VariableMsgDataBody* var = new VariableMsgDataBody();
    var->_var_name = "dvg";
    var->_var_type = eData_string;
    var->_var_value = d.toBase64().toStdString();
    vec.push_back(var);
}

QByteArray XmlUtil::_comm_sim_conf_data;
void XmlUtil::add_CommSimEventConf_data(const QByteArray& d)
{
    _comm_sim_conf_data.append(d);
}

void XmlUtil::delete_CommSimEventConf_data(const QByteArray& d)
{
    int index = _comm_sim_conf_data.indexOf(d);
    if(index < 0){
        return;
    }

    _comm_sim_conf_data.remove(index, d.length());
}

bool XmlUtil::parse_CommSimEventConf_xml(const DataXmlVec& vec, ByteArrVec& data, DblVec& time)
{
    if(vec.empty()){
        return false;
    }

    VariableMsgDataBody* var_event_data = (VariableMsgDataBody*)vec[0];
    QByteArray d_base64 = QByteArray::fromRawData(var_event_data->_var_value.c_str(), var_event_data->_var_value.length());
    QByteArray d = QByteArray::fromBase64(d_base64);

    int index = 0;
    const int len_size = sizeof(uint16_t);
    const int time_size = sizeof(double);
    const int offset_len = sizeof(LocalAddr) + sizeof(uint16_t);

    while(index < d.length()){
        int d_len = d.mid(index + offset_len, len_size).toInt();
        QByteArray d_data = d.mid(index, d_len);
        data.push_back(d_data);
        QByteArray d_time = d.mid(index + d_len, time_size);
        time.push_back(d_time.toDouble());
        index += d_len + time_size;
    }

    return true;
}

QDomDocument* XmlUtil::generate_CommSimEventConf_xml(int ss_id, int ps_id)
{
    if(_comm_sim_conf_data.isEmpty()){
        return nullptr;
    }

    QByteArray d = _comm_sim_conf_data.toBase64();

    VariableMsgDataBody* var_event_data = new VariableMsgDataBody();
    var_event_data->_var_name = "CommSimEventConfData";
    var_event_data->_var_type = eData_string;
    var_event_data->_var_value = d.toStdString();

    ProcedureMessageBody* proc_body = new ProcedureMessageBody();
    proc_body->_appl_msg_body = nullptr;
    proc_body->_msg_name = "CommSimEventData";
    proc_body->_proc_type = eSubProcedure_cfg_communication_data;
    proc_body->_msg_type = eMessage_notify;
    proc_body->_data_vector.push_back(var_event_data);

    SessionMessageBody* sess_body = new SessionMessageBody();
    sess_body->_msg_type = ePG_comm_sim_event_data;
    sess_body->_id_i2u = ss_id;
    sess_body->_id_u2i = ps_id;
    sess_body->_procedure_msg_body = proc_body;

    RootMessageBody root;
    root._session_msg_body = sess_body;
    QDomDocument* doc = root.Attr2Document();

    for(int i=0; i<proc_body->_data_vector.size(); ++i){
        delete proc_body->_data_vector[i];
    }

    if(proc_body) delete proc_body;
    if(sess_body) delete sess_body;
    _comm_sim_conf_data.clear();

    return doc;
}

