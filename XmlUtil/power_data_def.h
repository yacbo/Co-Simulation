#ifndef POWER_DATA_DEF_H
#define POWER_DATA_DEF_H

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

//电力仿真输入数据
typedef struct PowerSimInputData : public PowerDataBase{
    EPowerDataType data_type;
}PowerSimInputData;

//电力仿真结果数据
typedef struct PowerSimResultData : public PowerDataBase{
    int data_length;
    double cur_sim_time;
    EPowerDataType data_type;
}PowerSimResultData;

//DGInfor
typedef struct PowerDGInforData : public PowerSimInputData{
    double dv;
    double time_diff;
}PowerDGInforData;

//BusInfor
typedef struct PowerBusInforData : public PowerSimResultData{
    double bus_volt;                     //母线电压.
    double bus_angle;                   //母线幅角.
}PowerDvgData;


#pragma pack(pop)

#endif // POWER_DATA_DEF_H
