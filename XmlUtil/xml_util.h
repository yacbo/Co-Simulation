#ifndef XML_UTIL_H
#define XML_UTIL_H

#include "xml_message.h"
#include "pg_rtui_def.h"

class XMLUTILSHARED_EXPORT XmlUtil
{
public:
    XmlUtil();
    virtual ~XmlUtil();

public:
     //生成登录或登出xml, 请求/应答/错误
     static QDomDocument* generate_conn_xml(const char* dev_name, uint32_t dev_ip, uint16_t dev_port, int ss_id, int ps_id, long reg_type, long msg_type, const char* err_msg = nullptr);

     //生成登录或登出xml, 发送到仿真控制端的
     static QDomDocument* generate_reg_xml(int ss_id, int ps_id, const char* dev_name, long reg_type);

     //生成会话xml，开始/结束/请求/确认/错误
     static QDomDocument* generate_session_xml(int ss_id, int ps_id, const char* dst_dev, long sess_type, long msg_type, const char* err_msg = nullptr);

     //生成请求应用过程xml，请求/确认/错误/通知
     static QDomDocument* generate_appl_req_xml(int ss_id, int ps_id, const char* appl_name, const IntVec& steps,  long msg_type, const char* err_msg = nullptr);

     //生成发送数据过程xml，请求/确认/通知
     static QDomDocument* generate_snd_data_xml(int ss_id, int ps_id, const char* appl_name, long steps, const DataXmlVec& var, long msg_type);

     //生成调用过程xml，请求/确认/错误/通知
     static QDomDocument* generate_invoke_xml(int ss_id, int ps_id, const char* invoke_name, const char* appl_name, long steps, const DataXmlVec& var, long msg_type, const char* err_msg = nullptr);

public:
     //生成传输器件id及类型的xml，器件登录时由SBS向全部登录用户发送.
     static QDomDocument* generate_notify_dev_id_xml(int ss_id, int ps_id, const IntMap& dev_ids);
     static bool parse_device_id_type_data(const DataXmlVec& data, IntMap& type_id);

     //生成控制命令xml
     static QDomDocument* generate_sim_cmd_xml(int ss_id, int ps_id, int cmd);

public:
     static ESimDevType query_sim_dev_type(const char* name);

/****************动态控制界面各模块消息结构体******************/
public:
    static PG_RTUI_Base* parse_PG_RTUI_xml(EPGRTUIType type, const DataXmlVec& vec);
    static QDomDocument* create_PG_RTUI_xml(int ss_id, int ps_id, const PG_RTUI_Base* param);

    static QDomDocument*  generate_PG_RTUI_Msg_GetInterfaceNum_xml(int ss_id, int ps_id,  const PG_RTUI_Msg_GetInterfaceNum* data);

    static QDomDocument*  generate_PG_RTUI_Ack_GetInterfaceNum_xml(int ss_id, int ps_id,  const PG_RTUI_Ack_GetInterfaceNum* data);

    static QDomDocument*  generate_PG_RTUI_Msg_GetInterface_xml(int ss_id, int ps_id, const PG_RTUI_Msg_GetInterface* data);

    static QDomDocument*  generate_PG_RTUI_Ack_GetInterface_xml(int ss_id, int ps_id,  const PG_RTUI_Ack_GetInterface* data);

    static  QDomDocument*  generate_PG_RTUI_Msg_SetInterface_xml(int ss_id, int ps_id,  const PG_RTUI_Msg_SetInterface* data);

    static  QDomDocument*  generate_PG_RTUI_GetLink_xml(int ss_id, int ps_id, const PG_RTUI_Msg_GetLink* data);

    static  QDomDocument*  generate_PG_RTUI_Ack_GetLink_xml(int ss_id, int ps_id, const PG_RTUI_Ack_GetLink* data);

    static  QDomDocument*  generate_PG_RTUI_Msg_SetLink_xml(int ss_id, int ps_id,  const PG_RTUI_Msg_SetLink* data);

    static  QDomDocument*  generate_PG_RTUI_ChangeNodeStatus_xml(int ss_id, int ps_id,  const PG_RTUI_ChangeNodeStatus* data);

    static  QDomDocument*  generate_PG_RTUI_ChangePortStatus_xml(int ss_id, int ps_id, const PG_RTUI_ChangePortStatus* data);

    static  QDomDocument*  generate_PG_RTUI_StaticRoute_xml(int ss_id, int ps_id, const PG_RTUI_StaticRoute* data);

    static  QDomDocument*  generate_PG_RTUI_AddCBR_xml(int ss_id, int ps_id, const PG_RTUI_AddCBR* data);

    static  QDomDocument*  generate_PG_RTUI_SetDataTamperSimulationTime_xml(int ss_id, int ps_id, const PG_RTUI_SetDataTamperSimTime* data);

    static  QDomDocument*  generate_PG_RTUI_SetDataTamperLastTime_xml(int ss_id, int ps_id, const PG_RTUI_SetDataTamperLastTime* data);

    static  QDomDocument*  generate_PG_RTUI_StopDataTamper_xml(int ss_id, int ps_id, const PG_RTUI_StopDataTamper* data);

    static bool parse_PG_RTUI_Msg_GetInterfaceNum_xml(const DataXmlVec& vec,PG_RTUI_Msg_GetInterfaceNum &data );

    static bool parse_PG_RTUI_Ack_GetInterfaceNum_xml(const DataXmlVec& vec,PG_RTUI_Ack_GetInterfaceNum &data );

    static bool parse_PG_RTUI_Msg_GetInterface_xml(const DataXmlVec& vec,PG_RTUI_Msg_GetInterface &data );

    static bool parse_PG_RTUI_Ack_GetInterface_xml(const DataXmlVec& vec,PG_RTUI_Ack_GetInterface &data );

    static bool parse_PG_RTUI_Msg_SetInterface_xml(const DataXmlVec& vec,PG_RTUI_Msg_SetInterface &data );

    static bool parse_PG_RTUI_Msg_GetLink_xml(const DataXmlVec& vec,PG_RTUI_Msg_GetLink &data );

    static bool parse_PG_RTUI_Ack_GetLink_xml(const DataXmlVec& vec,PG_RTUI_Ack_GetLink &data );

    static bool parse_PG_RTUI_Msg_SetLink_xml(const DataXmlVec& vec,PG_RTUI_Msg_SetLink &data );

    static bool parse_PG_RTUI_ChangeNodeStatus_xml(const DataXmlVec& vec,PG_RTUI_ChangeNodeStatus &data );

    static bool parse_PG_RTUI_ChangePortStatus_xml(const DataXmlVec& vec,PG_RTUI_ChangePortStatus &data );

    static bool parse_PG_RTUI_StaticRoute_xml(const DataXmlVec& vec,PG_RTUI_StaticRoute &data );

    static bool parse_PG_RTUI_AddCBR_xml(const DataXmlVec& vec,PG_RTUI_AddCBR &data );

    static bool parse_PG_RTUI_SetDataTamperSimTime_xml(const DataXmlVec& vec,PG_RTUI_SetDataTamperSimTime &data );

    static bool parse_PG_RTUI_SetDataTamperLastTime_xml(const DataXmlVec& vec,PG_RTUI_SetDataTamperLastTime &data );

    static bool parse_PG_RTUI_StopDataTamper_xml(const DataXmlVec& vec,PG_RTUI_StopDataTamper &data );

public:
    //控制台联合仿真参数配置.
    static bool parse_UnionSimConfParam_xml(const DataXmlVec& vec, UnionSimConfParam& data);
    static QDomDocument* generate_UnionSimConfParam_xml(int ss_id, int ps_id,  const UnionSimConfParam* data);

    //电力仿真参数配置.
    static bool load_PowerSimConfParam_cfg(const char* cfg_path, PowerConfParam& data);
    static bool parse_PowerSimConfParam_xml(const DataXmlVec& vec, PowerConfParam& data);
    static QDomDocument* generate_PowerSimConfParam_xml(int ss_id, int ps_id, const PowerConfParam* data);

    //通信仿真参数配置.
    static bool load_CommConfParam_cfg(const char* cfg_path, CommConfParam& data);
    static bool parse_CommSimConfParam_xml(const DataXmlVec& vec, CommConfParam& data);
    static QDomDocument* generate_CommSimConfParam_xml(int ss_id, int ps_id, const CommConfParam* data);

    //协同仿真数据,电力->通信, 通信->电力应用
    static void parse_xml_power_sim_data(const DataXmlVec& vec, UnionSimDatVec& data);
    static void generate_xml_power_sim_data(const UnionSimDatVec& data, DataXmlVec& vec);

    static void parse_xml_power_appl_data(const DataXmlVec& vec, DblVec& data, UnionSimDatVec& us_data);
    static void generate_xml_power_appl_data(const DblVec& data, const UnionSimDatVec& us_data, DataXmlVec& vec);

    //通信仿真事件配置
    static void add_CommSimEventConf_data(const char* data, int len);
    static void delete_CommSimEventConf_data(const char* data, int len);
    static bool parse_CommSimConfParam_xml(const DataXmlVec& vec, DblVec& time, PGBaseVec& data);
    static QDomDocument* generate_CommSimEventConf_xml(int ss_id, int ps_id);

public:
    static void generate_xml_file(const std::string& fileName, QDomDocument* doc);

private:
    static QByteArray _comm_sim_conf_data;
};
#endif // XML_UTIL_H
