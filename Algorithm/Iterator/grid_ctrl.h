typedef struct _Bus_Info{    //电力上传母线信息至电力控制算法
	double busno;
	double sampling_time;
	double freq;
}Bus_Info;
typedef struct DG_exchangedata{    //电力软件上传至电力控制算法结构体
	double DG_NO;
	double generation_value;
	double simtime;
}DG_Node;

typedef struct _Ctrl_to_Grid{    //电力控制算法下发至电力软件结构体
	double DG_node_ID;           //电力节点编号（DG）
	double PowerUpadateTime;     //更新时间
	double dP;                   //控制指令
}Ctrl_to_Grid;
