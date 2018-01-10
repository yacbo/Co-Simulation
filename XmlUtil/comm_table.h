
/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifndef __COMM_TABLE_H__
#define __COMM_TABLE_H__

#include <QMap>
#include <functional>
#include "xml_message.h"
#include "power_data_def.h"

struct ProcEventParam
{
    int _bus_num;
    string _in_out_info;
    bool _handle_success;

    DataXmlVec _param;              //待处理参数
    EPowerPrjType _type;           //参数类型
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

#endif

