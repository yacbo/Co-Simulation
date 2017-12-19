#ifndef PG_RTUI_DEF_H
#define PG_RTUI_DEF_H

#include <vector>
#include <inttypes.h>
#include <QByteArray>

#pragma pack(push)
#pragma pack(1)

//communication
typedef enum E_PG_RTUI_Type{
    ePG_RTUI_undef = -1,

    ePG_RTUI_msg_getinterfacenum = 13,
    ePG_RTUI_ack_getinterfacenum = 14,
    ePG_RTUI_msg_getinterface = 9,
    ePG_RTUI_ack_getinterface = 12,
    ePG_RTUI_msg_setinterface = 10,

    ePG_RTUI_msg_getlink = 7,
    ePG_RTUI_ack_getlink = 11,
    ePG_RTUI_msg_setlink = 8,

    //恶意路由攻击增加和移除.
    ePG_RTUI_add_staticroute = 15,
    ePG_RTUI_remove_staticroute = 16,

    //非半实物.
    ePG_RTUI_change_node_status = 23,
    ePG_RTUI_recover_node_status = 24,

    ePG_RTUI_change_port_status = 25,
    ePG_RTUI_recover_port_status = 26,

    //业务通道配置和移除.
    ePG_RTUI_static_route = 17,
    ePG_RTUI_recover_static_route =18,

    ePG_RTUI_add_cbr = 19,

    //数据篡改攻击
    ePG_RTUI_set_data_tamper_sim_time = 20,
    ePG_RTUI_set_data_tamper_last_time = 21,
    ePG_RTUI_stop_data_tamper = 22,

    //半实物.
    ePG_RTUI_hr_change_node_status = 5,
    ePG_RTUI_hr_recover_node_status = 6,

    ePG_RTUI_hr_change_port_status = 3,
    ePG_RTUI_hr_recover_port_status = 4,

    //仿真数据.
    ePG_sim_interoper_data = 100,
    ePG_comm_sim_cmd_data = 110,

    ePG_comm_sim_event_data = 120,
}EPGRTUIType;

//
struct LocalAddr{
    uint32_t ip_addr;
    uint16_t port;
};

struct PG_RTUI_Base{
    LocalAddr clientAddr;
    uint16_t type;
    uint16_t length;
};

struct PG_RTUI_Msg_GetInterfaceNum : public PG_RTUI_Base{
    uint16_t nodeId;
};

struct PG_RTUI_Ack_GetInterfaceNum : public PG_RTUI_Base{
    uint16_t nodeId;
    int32_t interfaceNum;
};

struct PG_RTUI_Msg_GetInterface : public PG_RTUI_Base{
    uint16_t nodeId;
    uint16_t interfaceIndex;
};

struct PG_RTUI_Ack_GetInterface : public PG_RTUI_Base{
    uint16_t nodeId;
    long long bandwidth;
    double txPower;
    double rxSensitivity;
    bool mode;
};

struct PG_RTUI_Msg_SetInterface : public PG_RTUI_Base{
    uint16_t nodeId;
    uint16_t interfaceIndex;
    long long bandwidth;
    double txPower;
    double rxSensitivity;
    bool mode;
};

struct PG_RTUI_Msg_GetLink : public PG_RTUI_Base{
    uint16_t nodeSrcId;
    uint16_t nodeDstId;
};

struct PG_RTUI_Ack_GetLink : public PG_RTUI_Base{
    uint16_t nodeSrcId;
    uint16_t nodeDstId;
    int32_t switchNum;
    double switchLoss;
    int32_t connectorNum;
    double connectorLoss;
    double lossCoefficient;
    double ber;
    int32_t time;
};

struct PG_RTUI_Msg_SetLink : public PG_RTUI_Base{
    uint16_t nodeSrcId;
    uint16_t nodeDstId;
    int32_t switchNum;
    double switchLoss;
    int32_t connectorNum;
    double connectorLoss;
    double lossCoefficient;
};

struct PG_RTUI_ChangeNodeStatus : public PG_RTUI_Base{
    uint16_t nodeId;
    //非半实物不包含下面两项
    float pdsi_delay_time;
    float change_delay_time;
};

struct PG_RTUI_ChangePortStatus : public PG_RTUI_Base{
    uint16_t nodeId1;
    uint16_t nodeId2;
    //非半实物不包含下面两项
    float pdsi_delay_time;
    float change_delay_time;
};

struct PG_RTUI_StaticRoute : public PG_RTUI_Base{
    float pdsi_delay_time;
    uint16_t nodeId;
    char dst_ip[20];
    char nxt_hop[20];
    char out_itf_ip[20];
    int32_t cost;
};

struct PG_RTUI_AddCBR : public PG_RTUI_Base{
    uint32_t srcId;
    uint32_t dstId;
    int32_t itemsToSend;
    int32_t itemSize;
    char startTime[20];
    char interval[20];
    char endTime[20];
};

struct PG_RTUI_SetDataTamperSimTime : public PG_RTUI_Base{
    uint16_t nodeId;           //需要修改数据的通信起点
    uint16_t powernodeId; //需要修改数据的电力起点
    uint16_t datatype;        //要修改的数据类型
    uint16_t dataPlace;       //是在电网信息的第几个字节开始修改
    char dataDev[10];         //修改量,与需要修改的类型一致即可，放在这个char数组内即可
    float preset_time;
    float tamper_time;
};

struct PG_RTUI_SetDataTamperLastTime : public PG_RTUI_Base{
    uint16_t powernodeId; //需要修改数据的通信起点
    uint16_t nodeId;
    uint16_t datatype;        //要修改的数据类型
    uint16_t dataPlace;       //是在电网信息的第几个字节开始修改
    char dataDev[10];         //修改量,与需要修改的类型一致即可，放在这个char数组内即可
    float preset_time;
    float tamper_time;
};

struct PG_RTUI_StopDataTamper : public PG_RTUI_Base{};

//设置通信仿真处理时延间隔
struct PG_RTUI_Msg_CommSimHandleDelay : public PG_RTUI_Base{
uint32_t  comm_sim_handle_delay;
};

#pragma pack(pop)

typedef std::vector<QByteArray> ByteArrVec;
typedef std::vector<PG_RTUI_Base*> PGBaseVec;

#endif // PG_RTUI_DEF_H
