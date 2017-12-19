
#include <QLibrary>
#include "comm_table.h"

//ProcEventParam
ProcEventParam::ProcEventParam()
{
    _appl_msg = nullptr;
    _data_msg = nullptr;
}

//MetaProcedure
MetaProcedure::MetaProcedure()
{

}

void MetaProcedure::SetProcedureHandler(EEventType type, ProcedureHandler handler)
{
    switch (type) {
    case eEvent_proc_req_snding: _ProcedureRequestSendBefore = handler; break;
    case eEvent_proc_req_snded: _ProcedureRequestSendAfter = handler; break;
    case eEvent_proc_req_rcved: _ProcedureRequestSendReceived = handler; break;
    case eEvent_proc_rep_snded: _ProcedureResponseSendAfter = handler; break;
    case eEvent_proc_cnf_rcved: _ProcedureResponseConfirmReceived = handler; break;
    case eEvent_proc_err_rcved: _ProcedureResponseErrorReceived = handler; break;
    default: break;
    }
}

void MetaProcedure::ParseProcedureHandler()
{
    EvTypeFuncMap::const_iterator  it = _device_method.cbegin();
    for(; it != _device_method.cend(); ++it){
        SetProcedureHandler(it.key(), it.value());
    }
}

//DataSndProcedure : public MetaProcedure
DataSndProcedure::DataSndProcedure() : MetaProcedure()
{
    _proc_type = eSubProcedure_data_send;
}

//InvokeProcedure : public MetaProcedure
InvokeProcedure::InvokeProcedure() : MetaProcedure()
{
    _proc_type = eSubProcedure_invoke;
}

//ApplReqProcedure : public MetaProcedure
ApplReqProcedure::ApplReqProcedure() : MetaProcedure()
{
    _proc_type = eSubProcedure_appl_request;
}

//ApplProcedureOper
ApplProcedureOper::ApplProcedureOper()
{

}

ApplProcedureOper::ApplProcedureOper(const string& proc_name, ESubProcedureType proc_type, int proc_steps)
{
    _appl_msg = nullptr;
    _session_msg = nullptr;

    _proc_type = proc_type;
    _proc_name = proc_name;
    _proc_steps = proc_steps;

    for(int i=0; i<proc_steps; ++i){
        _meta_proc_que.push(CreateApplProcedure(proc_type));
    }
}

ApplProcedureOper::ApplProcedureOper(const ApplProcedureOper& proc_oper)
{
    *this = proc_oper;
}

ApplProcedureOper::~ApplProcedureOper()
{
    while(_meta_proc_que.size()){
        MetaProcedure* meta_proc = _meta_proc_que.front();
        _meta_proc_que.pop();
        delete meta_proc;
    }

    if(_appl_msg){
        delete _appl_msg;
    }

    if(_session_msg){
        delete _session_msg;
    }
}

 ApplProcedureOper& ApplProcedureOper::operator = (const ApplProcedureOper& proc_oper)
 {
    if(this == &proc_oper){
        return *this;
    }

    _bactive = proc_oper._bactive;
    _proc_type = proc_oper._proc_type;
    _proc_name = proc_oper._proc_name;
    _proc_steps = proc_oper._proc_steps;

    _appl_msg = proc_oper._appl_msg ? new ApplMessageBody(*proc_oper._appl_msg) : nullptr;
    _session_msg = proc_oper._session_msg ? new SessionMessageBody(*proc_oper._session_msg) : nullptr;

    _meta_proc_que = proc_oper._meta_proc_que;
 }

MetaProcedure* ApplProcedureOper::CreateApplProcedure(ESubProcedureType type)
{
    MetaProcedure* proc = nullptr;

    switch(type){
    case eSubProcedure_appl_request: proc = new ApplReqProcedure(); break;
    case eSubProcedure_data_send: proc = new DataSndProcedure(); break;
    case eSubProcedure_invoke: proc = new InvokeProcedure(); break;
    default: break;
    }

    return proc;
}

bool ApplProcedureOper::ExchangeRole(bool bactive)
{
    return true;
}

bool ApplProcedureOper::ExecuteProcedure(bool bactive)
{
    if(_meta_proc_que.size() == 0){
        return true;
    }

    void* custom_data = nullptr;
    ProcEventParam proc_ev_param;
    proc_ev_param._appl_msg = _appl_msg;
    proc_ev_param._data_msg = proc_ev_param._data_msg = nullptr;

    MetaProcedure* meta_proc = _meta_proc_que.front();
    _meta_proc_que.pop();

    //主动请求端
    if(bactive){
        //过程请求消息发送前的处理函数
        meta_proc->_ProcedureRequestSendBefore(&proc_ev_param, custom_data);
        //发送过程请求消息

        //过程请求消息发送后的处理函数
        meta_proc->_ProcedureRequestSendAfter(&proc_ev_param, custom_data);

        //等待响应消息

        //接收响应消息后的处理函数
        if(true){ //确认
            meta_proc->_ProcedureResponseConfirmReceived(&proc_ev_param, custom_data);
        }else{     //错误
            meta_proc->_ProcedureResponseErrorReceived(&proc_ev_param, custom_data);
        }
    }else{
        //接收请求消息后的处理函数
        meta_proc->_ProcedureRequestSendReceived(&proc_ev_param, custom_data);

        //发送响应消息

        //响应消息发送后的处理函数
        meta_proc->_ProcedureResponseSendAfter(&proc_ev_param, custom_data);
    }

    delete meta_proc;

    return true;
}

MetaProcQue& ApplProcedureOper::GetMetaProcQueue()
{
    return _meta_proc_que;
}

void ApplProcedureOper::SetActiveFlag(bool bactive)
{
    _bactive = bactive;
}

//DeviceProcMethodTable
bool DeviceProcMethodTable::LoadEventHandlerDll(const string& device_name, const string& dll_path)
{
    QLibrary parser(dll_path.c_str());
    if(!parser.load()){
        return false;
    }

    static const int func_num = 6;
    static char func_arr[][128] = {
        "ProcedureRequestSendBefore",
        "ProcedureRequestSendAfter",
        "ProcedureRequestSendReceived",
        "ProcedureResponseSendAfter",
        "ProcedureResponseConfirmReceived",
        "ProcedureResponseErrorReceived",
    };

    string proc_name = "appl request";
    ProcMethodMap proc_method_map;
    for(int i=0; i<func_num; ++i){
         /*ProcedureHandler func_ptr = (ProcedureHandler)parser.resolve(func_arr[i]);
         if(func_ptr) {
             proc_method_map[proc_name][(EEventType)i] = func_ptr;
         }*/
    }

    _device_proc_method_map[device_name] = proc_method_map;

    return true;
}

const  DeviceMethodMap& DeviceProcMethodTable::GetDeviceMethodMap() const
 {
     return _device_proc_method_map;
 }

void DeviceProcMethodTable::SetDeviceMethodMap(DeviceMethodMap& method)
{
    _device_proc_method_map = method;
}

//ApplProcedureTable
ApplProcedureTable::ApplProcedureTable()
{
    InitApplProcTable();
}

ApplProcedureOper* ApplProcedureTable::CreateProcInstance(const string& device_name, const string& proc_name, const DeviceProcMethodTable& method, bool bactive)
{
    ApplProcedureOper* appl_proc_oper_tmplate = FindProcedureTemplate(proc_name);
    if(!appl_proc_oper_tmplate){
        return nullptr;
    }

    ApplProcedureOper* appl_proc_oper = new ApplProcedureOper(*appl_proc_oper_tmplate);
    if(!appl_proc_oper){
        return nullptr;
    }

    const DeviceMethodMap& device_method_map = method.GetDeviceMethodMap();
    DeviceMethodMap::const_iterator it_o = device_method_map.find(device_name);
    if(it_o == device_method_map.cend()){
        return nullptr;
    }

    const ProcMethodMap& proc_method_map = it_o.value();
    ProcMethodMap::const_iterator it_i = proc_method_map.cbegin();

    for(; it_i != proc_method_map.cend(); ++it_i){
        const EvTypeFuncMap& func_ev_map = it_i.value();
        MetaProcQue& meta_proc_que = appl_proc_oper->GetMetaProcQueue();
        int size = meta_proc_que.size();

        for(int i=0; i<size; ++i){
            MetaProcedure* meta_proc = meta_proc_que.front();
            meta_proc_que.pop();

            meta_proc->_device_method = func_ev_map;
            meta_proc->ParseProcedureHandler();

            meta_proc_que.push(meta_proc);
        }
    }

    appl_proc_oper->SetActiveFlag(bactive);

    return appl_proc_oper;
}

ApplProcedureOper* ApplProcedureTable::FindProcedureTemplate(const string& proc_name)
{
    StrApplProcOperMap::const_iterator it = _appl_proc_oper_map.find(proc_name);
    if(it == _appl_proc_oper_map.cend()){
        return nullptr;
    }

    return it.value();
}

bool ApplProcedureTable::AddProcedure(ESubProcedureType type, const string& proc_name, int proc_steps)
{
    if(_appl_proc_oper_map.find(proc_name) != _appl_proc_oper_map.end()){
        return false;
    }

    ApplProcedureOper* appl_proc_oper = new ApplProcedureOper(proc_name,  type, proc_steps);
    _appl_proc_oper_map[proc_name] = appl_proc_oper;

    return true;
}

void ApplProcedureTable::RemoveProcedure(const string& proc_name)
{
    StrApplProcOperMap::iterator it = _appl_proc_oper_map.find(proc_name);
    if(it == _appl_proc_oper_map.end()){
        return;
    }

    ApplProcedureOper* appl_proc_oper = it.value();
    delete appl_proc_oper;

    _appl_proc_oper_map.erase(it);
}

void ApplProcedureTable::InitApplProcTable()
{
    //通信仿真
    EApplProcType comm_sim_type = eApplProc_comm_sim;
    ApplProcedureOper* comm_sim = new ApplProcedureOper();
    comm_sim->_proc_name = ProcNamesSet[comm_sim_type];
    comm_sim->_proc_steps = 3;
    comm_sim->_proc_type = eProcedure_interoper;
    MetaProcQue& comm_meta_que =  comm_sim->GetMetaProcQueue();
    comm_meta_que.push(new ApplReqProcedure());
    comm_meta_que.push(new DataSndProcedure());
    comm_meta_que.push(new InvokeProcedure());
    _appl_proc_oper_map[ProcNamesSet[comm_sim_type]] = comm_sim;

    //决策计算
    EApplProcType ctrl_calc_type = eApplProc_control_calc;
    ApplProcedureOper* ctrl_calc = new ApplProcedureOper();
    ctrl_calc->_proc_name = ProcNamesSet[ctrl_calc_type];
    ctrl_calc->_proc_steps = 4;
    ctrl_calc->_proc_type = eProcedure_interoper;
    MetaProcQue& calc_meta_que =  ctrl_calc->GetMetaProcQueue();
    calc_meta_que.push(new ApplReqProcedure());
    calc_meta_que.push(new DataSndProcedure());
    calc_meta_que.push(new InvokeProcedure());
    calc_meta_que.push(new DataSndProcedure());
    _appl_proc_oper_map[ProcNamesSet[ctrl_calc_type]] = ctrl_calc;

    //电力仿真
    EApplProcType power_sim_type = eApplProc_power_sim;
    ApplProcedureOper* power_sim = new ApplProcedureOper();
    power_sim->_proc_name = ProcNamesSet[power_sim_type];
    power_sim->_proc_steps = 3;
    power_sim->_proc_type = eProcedure_interoper;
    MetaProcQue& power_meta_que =  power_sim->GetMetaProcQueue();
    power_meta_que.push(new ApplReqProcedure());
    power_meta_que.push(new DataSndProcedure());
    power_meta_que.push(new InvokeProcedure());
    _appl_proc_oper_map[ProcNamesSet[power_sim_type]] = power_sim;
}



