#ifndef CTRL_COMMU_H
#define CTRL_COMMU_H

typedef enum EInteractType{
        eInteract_power_bus = 0,
        eInteract_upload_bus = 1,
        eInteract_download_bus = 2,
        eIneract_ctrl_to_comm = 3,
        eIneract_comm_to_ctrl = 4,
        eInteract_ctrl_to_power = 5,
}EInteractType;

/*********电力控制算法和通信接口******/
typedef struct _Bus_Info_Upload{  //上传频率值至通信软件
        double PowerSimTime;     //仿真时刻
        double PowerFrequency;   //电网频率
        int Startnode_ID;     //起始节点编号
        int Destnode_ID;      //目的节点编号
}bus_info_upload;

typedef struct _Ctrl_to_Commu {       //算法中DG节点上传通信软件结构体   
        double PowerSimTime;    //仿真时刻
        int Startnode_ID;       //始发通信节点编号
        int Destnode_ID;        //通信终点编号
        bool Converge_Flag;     //算法收敛标志
        double Data[16];        //节点通信交换数据
}ctrl_to_commu;

typedef struct _Bus_Info_Download{  //通信软件下发频率值至控制算法结构体
        double PowerSimTime;    //仿真时刻
        double PowerFrequency;   //电网频率
        int Startnode_ID;       //始发通信节点编号
        int Destnode_ID;        //通信终点编号
        int ErrorType;          //故障类型
        double TimeDelay;       //通信时延
}bus_info_download;

typedef struct _Commu_to_Ctrl{      //通信软件下发至控制算法DG节点数据结构体
        double PowerSimTime;    //仿真时刻
        int Startnode_ID;       //始发通信节点编号
        int Destnode_ID;        //通信终点编号
        double TimeDelay;       //通信时延
        int ErrorType;          //故障类型
        bool Startnode_State;   //始发节点状态,0表示节点正常
        bool Destnode_State;    //目的节点状态,0表示节点正常
        bool Converge_Flag;     //算法收敛标志
        double data[16];        //节点通信交换数据
}commu_to_ctrl;


/*
//输入为上传至通信软件的母线结构体
//输出为下传至控制算法的母线信息结构体
*/
/*bus_info_download DownSamplingFreq(bus_info_upload bus0,double time_delay){
    bus_info_download bus1;
    bus1.startnode=bus0.startnode;
    bus1.destnode=bus0.startnode;
    bus1.time_sim=bus1.time_sim;
    bus1.freq=bus0.freq;
    bus1.time_delay=time_delay;//通信时延,由通信软件得到
    return bus1;
}
*/
/*
//输入为上传至通信软件的DG数据结构体数组,
输出为下传至控制算法的DG节点数据结构体数组
*/
/*void DownDGData(ctrl_to_commu dg_node_upload[],int struct_num,commu_to_ctrl dg_node_download[],double time_delay[]){
    for(int i=0;i<struct_num;i++){
        if(dg_node_download[i].errortype=0){
        dg_node_download[i].startnode_ID=dg_node_upload[i].startnode_ID;
        dg_node_download[i].destnode_ID=dg_node_upload[i].destnode_ID;
        dg_node_download[i].time_sim=dg_node_upload[i].tim_sim;
        dg_node_download[i].converge_flag=dg_node_upload[i].converge_flag;
        memcpy(dg_node_download[i].data,dg_node_upload[i].data,sizeof(dg_node_upload[i].data));
    }
    */
#endif
