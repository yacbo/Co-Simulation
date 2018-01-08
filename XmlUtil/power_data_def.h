#ifndef POWER_DATA_DEF_H
#define POWER_DATA_DEF_H

#include <vector>
#include <inttypes.h>

typedef enum E_PowerDataType{
    ePowerData_undef  = -1,
    ePowerData_dginfor = 0,
    ePowerData_businfor = 1,
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

//DGInfor
typedef struct PowerDGInforData : public PowerSimDwStmData{
    double dv;
    double time_diff;
}PowerDGInforData;

//BusInfor
typedef struct PowerBusInforData : public PowerSimUpStmData{
    double bus_volt;                     //母线电压.
    double bus_angle;                   //母线幅角.
}PowerDvgData;

typedef std::vector<PowerSimUpStmData*> PowerSUDataVec;
typedef std::vector<PowerSimDwStmData*> PowerSDDataVec;

#pragma pack(pop)

#endif // POWER_DATA_DEF_H
