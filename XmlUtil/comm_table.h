
/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifndef __COMM_TABLE_H__
#define __COMM_TABLE_H__

#include <QMap>
#include <functional>
#include "xml_message.h"

struct XMLUTILSHARED_EXPORT ProcEventParam
{
public:
    ProcEventParam();

public:
    int bus_num;
    string _in_out_info;
    DataXmlMsgBase* _data_msg;
    ApplMessageBody* _appl_msg;
    bool _handle_success;
};

typedef std::function<void(const ProcEventParam*, void*)> ProcedureHandler;
typedef QMap<EEventType, ProcedureHandler> EvTypeFuncMap;
typedef QMap<ESubProcedureType, EvTypeFuncMap> ProcMethodMap;

class XMLUTILSHARED_EXPORT MetaProcedure
{
public:
    MetaProcedure();

public:
    void SetProcedureHandler(EEventType type, ProcedureHandler handler);
    void ParseProcedureHandler();

public:
    long _proc_type;
    EvTypeFuncMap _device_method;
	
public:
    ProcedureHandler _ProcedureRequestSendBefore;                  //过程开始前的处理函数，本地调用
    ProcedureHandler _ProcedureRequestSendAfter;                     //发送过程请求消息后的处理函数，本地调用
    ProcedureHandler _ProcedureRequestSendReceived;               //接收到请求消息后的处理函数，远端调用
    ProcedureHandler _ProcedureResponseSendAfter;                   //响应消息发出后的处理函数，远端调用
    ProcedureHandler _ProcedureResponseConfirmReceived;        //接收到确认响应后的处理函数，本地调用
    ProcedureHandler _ProcedureResponseErrorReceived;            //接收到错误响应后的处理函数，本地调用

    //virtual void ProcedureRequestSending(const ProcEventParam& param, void* customData) = 0;
    //virtual void ProcedureRequestSent(const ProcEventParam& param, void* customData) = 0;
    //virtual void ProcedureRequestReceived(const ProcEventParam& param, void* customData) = 0;
    //virtual void ProcedureResponseSent(const ProcEventParam& param, void* customData) = 0;
    //virtual void ProcedureConfirmReceived(const ProcEventParam& param, void* customData) = 0;
    //virtual void ProcedureErrorReceived(const ProcEventParam& param, void* customData) = 0;
};

class XMLUTILSHARED_EXPORT DataSndProcedure : public MetaProcedure
{
public:
    DataSndProcedure();
};

class XMLUTILSHARED_EXPORT InvokeProcedure : public MetaProcedure
{
public:
    InvokeProcedure();
};

class XMLUTILSHARED_EXPORT ApplReqProcedure : public MetaProcedure
{
public:
    ApplReqProcedure();
};

typedef queue<MetaProcedure*> MetaProcQue;
class XMLUTILSHARED_EXPORT ApplProcedureOper
{
public:
    ApplProcedureOper();
    ApplProcedureOper(const string& proc_name, ESubProcedureType proc_type, int proc_steps);
    explicit ApplProcedureOper(const ApplProcedureOper& proc_oper);
    virtual ~ApplProcedureOper();

public:
    ApplProcedureOper& operator = (const ApplProcedureOper& proc_oper);

public:
    bool ExchangeRole(bool bactive);
    bool ExecuteProcedure(bool bactive);

    MetaProcQue& GetMetaProcQueue();
    void SetActiveFlag(bool bactive);

private:
    MetaProcedure* CreateApplProcedure(ESubProcedureType type);

public:
    int _proc_steps;
	string _proc_name;
    long _proc_type;

private:
    bool _bactive;
    MetaProcQue _meta_proc_que;
    ApplMessageBody* _appl_msg;
    SessionMessageBody* _session_msg;
};

struct MsDeviceTable
{
    int _session_id;
	ESimDeviceState _device_state;

    string _dev_ip;
    uint16_t _dev_port;
    EProtocolType _proto_type;
    uint16_t _business_port;

    string _comm_host_ip;
    uint16_t _comm_host_port;
};

struct SbsDeviceTable
{
    string _dev_ip;
    uint16_t _dev_port;
    EProtocolType _proto_type;
    uint16_t _dev_id;
    ESimDeviceState _device_state;
};

typedef QMap<string, MsDeviceTable> MsDeviceMap;
typedef QMap<string, SbsDeviceTable> SbsDeviceMap;
typedef QMap<string, ProcMethodMap> DeviceMethodMap;

class XMLUTILSHARED_EXPORT DeviceProcMethodTable
{
private:
    DeviceMethodMap _device_proc_method_map;
public:
	bool LoadEventHandlerDll(const string& device_name, const string& dll_path);
    const DeviceMethodMap& GetDeviceMethodMap() const;
    void SetDeviceMethodMap(DeviceMethodMap& method);
};

typedef QMap<string, ApplProcedureOper*> StrApplProcOperMap;
class XMLUTILSHARED_EXPORT ApplProcedureTable
{
public:
    ApplProcedureTable();
private:
	StrApplProcOperMap _appl_proc_oper_map;
public:
    ApplProcedureOper* CreateProcInstance(const string& device_name, const string& proc_name, const DeviceProcMethodTable& method, bool bactive);
    ApplProcedureOper* FindProcedureTemplate(const string& proc_name);

public:
    bool AddProcedure(ESubProcedureType type, const string& proc_name, int proc_steps);
    void RemoveProcedure(const string& proc_name);

private:
    void InitApplProcTable();
};

#endif

