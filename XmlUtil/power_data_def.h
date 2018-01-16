#ifndef POWER_DATA_DEF_H
#define POWER_DATA_DEF_H

#include <vector>
#include <string>
#include <inttypes.h>

#define TEXT_MAX_LEN 32

typedef enum E_PowerPrjType{
    ePowerPrj_undef = -1,
    ePowerPrj_avr_ctrl_39 = 0,
    ePowerPrj_psse_jiangsu = 1,
    ePowerPrj_conf_power = 2,
}EPowerPrjType;

typedef enum E_PowerDataType{
    ePowerData_undef  = -1,
    ePowerData_dginfor = 0,
    ePowerData_businfor = 1,
    ePowerData_ctrlorder= 2,
    ePowerData_poweroper = 3,
    ePowerData_freqinfor = 4,
    ePowerData_dpnode   = 5,
}EPowerDataType;

#pragma pack(push)
#pragma pack(1)

//电力数据基类
typedef struct PowerDataBase{
    uint32_t bus_id;                       //母线编号.
}PowerDataBase;

//电力仿真上行数据
typedef struct PowerSimUpStmData : public PowerDataBase{
    int data_length;
    double cur_sim_time;
    EPowerDataType data_type;
}PowerSimUpStmData;

//电力仿真下行数据
typedef struct PowerSimDwStmData : public PowerDataBase{
    EPowerDataType data_type;
}PowerSimDwStmtData;

//BusInfor
typedef struct PowerBusInforData : public PowerSimUpStmData{
    double bus_volt;                     //母线电压.
    double bus_angle;                   //母线幅角.
}PowerDvgData;

//DGInfor
typedef struct PowerDGInforData : public PowerSimDwStmData{
    double dv;
    double time_diff;
}PowerDGInforData;

//PowerOperation
typedef struct PowerOperData : public PowerSimUpStmData{
    double lne1_power;
    double lne2_power;
    char lne1_name[TEXT_MAX_LEN];
    char lne2_name[TEXT_MAX_LEN];
}PowerOperData;

//CtrlOrder
typedef struct PowerCtrlOrderData : public PowerSimDwStmData{
    int flag;
    char gname[TEXT_MAX_LEN];
    int comm_beg_id;
    int comm_end_id;
    int comm_fault_type;
    double comm_trans_time;
}PowerCtrlOrderData;

//FreqInfor
typedef struct PowerFreqInforData : public PowerSimUpStmData{
    double freq;
}PowerFreqInforData;

//DpNode
typedef struct PowerDpNodeData : public PowerSimDwStmData{
    double dp;
    double sim_time;
}PowerDpNodeData;

typedef std::vector<PowerSimUpStmData*> PowerSUDataVec;
typedef std::vector<PowerSimDwStmData*> PowerSDDataVec;

#pragma pack(pop)

#endif // POWER_DATA_DEF_H
