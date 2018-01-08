
/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifndef __COMM_TYPE_DEF_H__
#define __COMM_TYPE_DEF_H__

#include <set>
#include <queue>
#include <string>
#include <vector>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <inttypes.h>
using std::set;
using std::queue;
using std::string;
using std::vector;
using std::pair;
using std::unordered_set;
using std::unordered_map;
#define MACRO_CSS_BUTTON_ORI "QPushButton{border-radius:5px; background:#696969; color:white;min-height:20;}"
#define MACRO_CSS_BUTTON_HOVER "QPushButton:hover{background-color:#828282}"
#define MACRO_CSS_BUTTON_PRESS "QPushButton:pressed{background-color:#2C2C2C}"

typedef enum E_SimDevType{
    eSimDev_undef               = -1,
    eSimDev_power              = 0,                  //电力
    eSimDev_communication = 1,                  //通信
    eSimDev_power_appl       = 2,                 //电力应用，决策
    eSimDev_sim_controller   = 3,                 //仿真控制台
    eSimDev_half_real_entity = 4,                 //半实物
}ESimDevType;

const char DevNamesSet[][128] = {
    "power",
    "communication",
    "power application",
    "simulation controller",
    "half real entity",
};

typedef enum E_ApplProcType{
    eApplProc_undef        = -1,
    eApplProc_comm_sim = 0,
    eApplProc_control_calc = 1,
    eApplProc_power_sim = 2,
}EApplProcType;

const char ProcNamesSet[][128] = {
    "communication simulation",
    "control calculation",
    "power system simulation",
};

typedef enum E_BaseDataType{
    eData_undef = -1,
    eData_int8 = 0,
    eData_uint8 = 1,
    eData_int16 = 2,
    eData_uint16 = 3,
    eData_int32 = 4,
    eData_uint32 = 5,
    eData_int64 = 6,
    eData_uint64 = 7,
    eData_float = 8,
    eData_double = 9,
    eData_string = 10,
    eData_ptr32 = 11,
    eData_ptr64 = 12,
}EBaseDataType;

typedef enum E_EnumBase{
    eMsg_base = 0x10000001,
}EEnumBase;

typedef enum E_MessageType {
    eMessage_undef     = -1,
    eMessage_request  = eMsg_base + 0,
    eMessage_confirm = eMsg_base + 1,
    eMessage_notify  = eMsg_base + 2,
    eMessage_error   = eMsg_base + 3,
}EMessageType;

typedef enum E_ProcedureType {
	eProcedure_undef       = -1,
    eProcedure_session     = 0x100000,             //1048576
    eProcedure_interoper   = 0x010000,           //65536
    eProcedure_application = 0x001000,          //4096
    eProcedure_custom       = 0x000100,          //256
}EProcedureType;

typedef enum E_SubProcedureType {
	eSubProcedure_undef           = -1,

	//session 
    eSubProcedure_register        = eProcedure_session  + 0,            //1048576
    eSubProcedure_unregister      = eProcedure_session  + 1,          //1048577
    eSubProcedure_session_begin   = eProcedure_session  + 2,        //1048578
    eSubProcedure_session_end     = eProcedure_session  + 3,         //1048579

	//interoper
    eSubProcedure_appl_request    = eProcedure_interoper + 0,      //65536
    eSubProcedure_data_send       = eProcedure_interoper  + 1,       //65537
    eSubProcedure_invoke          = eProcedure_interoper  + 2,         //65538
    eSubProcedure_note_wait       = eProcedure_interoper  + 3,       //65539

    //custom
    eSubProcedure_test                    = eProcedure_custom + 0,                              //256
    eSubProcedure_dev_type_id_data = eProcedure_custom + 1,                            //257
    eSubProcedure_cfg_communication_data = eProcedure_custom + 2,                 //通信软件配置数据, 258
    eSubProcedure_cfg_power_data         = eProcedure_custom + 3,                        //电力仿真配置数据, 259
    eSubProcedure_cfg_power_appl_data = eProcedure_custom + 4,                       //电力应用配置数据, 260
    eSubProcedure_cfg_sim_param_data = eProcedure_custom + 5,                        //仿真参数配置数据，如仿真时长，通信方式（TCP/UDP）等, 261
    eSubProcedure_sim_time_notify_data = eProcedure_custom + 6,                       //电力仿真时间数据, 262
    eSubProcedure_sim_cmd = eProcedure_custom + 1000,                                       //仿真控制命令，包括初始化，开始、暂停、结束仿真等, 356
}ESubProcedureType;

typedef enum E_SimCmdType{
    eSimCmd_undef = -1,
    eSimCmd_start_sim = eSubProcedure_sim_cmd + 0,
    eSimCmd_pause_sim = eSubProcedure_sim_cmd + 1,
    eSimCmd_keep_sim = eSubProcedure_sim_cmd + 2,
    eSimCmd_stop_sim = eSubProcedure_sim_cmd + 3,
}ESimCmdType;

typedef enum E_CommCmdType{
    eCommCmd_sim_delay_cfg = 38,
    eCommCmd_start_send_data = 1000,
    eCommCmd_stop_send_data = 1001,
}ECommCmdType;

typedef enum E_TransDataType{
	eTransData_undef      = -1,
    eTransData_net_addr   = eSubProcedure_data_send  + 100,
    eTransData_file       = eSubProcedure_data_send  + 101,
    eTransData_table      = eSubProcedure_data_send  + 102,
    eTransData_variable   = eSubProcedure_data_send  + 103,
}ETransDataType;

typedef enum E_ProtocolType {
	eProtocol_undef = -1,
	eProtocol_tcp   = 0,
	eProtocol_udp   = 1,
}EProtocolType;

typedef enum E_EncodeType {
	eEncode_undef = -1,
	eEncode_utf8  = 0,
    eEncode_ascii = 1,
}EEncodeType;

typedef enum E_ProxyState {
	eProxyState_undef          = -1,
	//server
	eProxyState_none_ready     = 0,
	eProxyState_both_ready     = 1,
	eProxyState_active_ready   = 2,
	eProxyState_passive_ready  = 3,

	//client
	eProxyState_stopped            = 10,
	eProxyState_starting           = 11,
	eProxyState_idle               = 12,
	eProxyState_stopping           = 13,
	eProxyState_registing          = 14,
	eProxyState_unregisting        = 15,
	eProxyState_session_starting   = 16,
	eProxyState_session_started    = 17,
	eProxyState_session_ending     = 18,
	eProxyState_appl_processing    = 19,
}EProxyState;

typedef enum E_SimDeviceState {
	eDeviceState_undef        = -1,
	eDeviceState_unregister   = 0,
	eDeviceState_registing    = 1,
	eDeviceState_ready        = 2,
	eDeviceState_busy         = 3,
	eDeviceState_unregisting  = 4,
}ESimDeviceState;

typedef enum E_ErrCauseType {
	eErrCause_undef = -1,
	eErrCause_reregister = 0,
}EErrCauseType;

typedef enum E_SynchronizeType{
    eSynch_undef = -1,
    eSynch_event = 0,
    eSynch_time = 1,
    eSynch_mix = 2,
}ESynchronizeType;

#pragma pack(push)
#pragma pack(1)

//elec sim param conf
typedef struct PowerConfParam{
    std::string prj_name;
    std::string case_name;
    double sim_time;
    double sim_period;
    int32_t upstm_type;     //上行类型，如DGInfor
    int32_t dwstm_type;    //下行类型，如BusInfor
    int32_t upstm_num;     //上行数据数目， 如39
    int32_t dwstm_num;    //下行数据数目， 如10
    std::string nodes_map;
}PowerConfParam;

//comm sim param conf
typedef struct CommConfParam{
    std::string comm_cmd;
    int32_t comm_sim_handle_max_delay;
}CommConfParam;

//union sim param conf MT控制端界面.
typedef struct UnionSimConfParam{
    double power_ctrl_prd;
    double fed_sim_time;
    double time_prop_step;
    ESynchronizeType syn_type;
    EProtocolType proto_type;
}UnionSimConfParam;

//error type
typedef enum E_ErrorType{
    eError_undef = -1,
    eError_noerror = 0,                 //无错误.
    eError_interrupt = 1,               //中断.
    eError_delay = 2,                    //延时.
    eError_errorbit = 3,                //误码.
    eError_tamper = 4,                 //篡改.
}EErrorType;

typedef struct CommSimData{
    uint32_t src_id;                     //起点节点编号.
    uint32_t dst_id;                     //终点节点编号.
    int32_t err_type;                   //错误类型.
    long long trans_delay;           //传输时延.
}CommSimData;

typedef struct UnionSimData{
    double realtime;                      //物理时戳.
    double sim_time;                     //仿真时刻.
    //communication sim
    CommSimData comm_dat;      //通信信息数据.
    //power sim data
    char power_dat[192];              //电力信息数据.
}UnionSimData;

#pragma pack(pop)

//
typedef vector<int> IntVec;
typedef vector<long> LongVec;
typedef vector<double> DblVec;
typedef vector<std::string> StrVec;
typedef pair<int, int> IntPair;
typedef vector<IntPair> PairVec;
typedef set<int> SimpleIntSet;
typedef unordered_set<int> IntSet;
typedef unordered_set<std::string> StrSet;
typedef unordered_map<int, int> IntMap;
typedef unordered_map<int, std::string> IntStrMap;
typedef unordered_map<std::string, int> StrIntMap;
typedef unordered_map<int, IntPair> IntPairMap;
typedef vector<UnionSimData> UnionSimDatVec;

typedef std::function<void(const char*, int)> RcvHandler;

#endif
