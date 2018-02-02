 /**************节点间双向通信********************/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>

using namespace std;
#include "discoctrl.h"
#include "ctrl_commu.h"
#include "grid_ctrl.h"

typedef vector<vector<string> > Mat;

Mat input(const char* filename)
{
	ifstream in(filename);
	Mat a;
	istringstream istr;
	string str;
	vector<string> varvec;
	while(getline(in,str))
	{
		istr.str(str);
		string buf;
		while(istr>>buf)
		{
			// std::cout<<"buf:"<<buf<<std::endl;
			varvec.push_back(buf);
		}
		a.push_back(varvec);
		varvec.clear();
		istr.clear();
	}
	return a;
} 
//上传带频率的母线节点信息至通信软件，bus为电力软件上传电力应用结构体，bus1为电力应用上传通信结构体
void UploadSamplingFreq(Bus_Info bus,bus_info_commu *bus1)
{
	bus1->PowerSimTime=bus.sampling_time;
	bus1->PowerFrequency=bus.freq;
	bus1->BusNo=bus.busno;
	bus1->Startnode_ID=10;   //bus1对应通信编号为10,这里先为自定义
	bus1->Destnode_ID=6;    //目的节点为牵引节点编号为6
	//----bus1发送至通信软件
}

double DownloadSamplingFreq(Para &para,bus_info_commu *bus2){
	//-----bus2由通信软件下发
	para.time=bus2->PowerSimTime + bus2->TimeDelay;
	double freq=bus2->PowerFrequency;
	return freq;
};

//解析数据包结构体
int packet_prase(commu_ctrl tRecv,State &state,Para &para){ 

	para.time=tRecv.PowerSimTime+tRecv.TimeDelay;
	if (tRecv.Destnode_ID == tRecv.Startnode_ID)  //目的节点和源节点相同表示收敛返回
	{
		return 1;
	}

	else 
	{
		dis_ctrl_DG_node(tRecv,state,para);
	}
	return 0;
}

//数据打包发送至通信软件，tSendDG为各节点数据包
void UploadDGPacket(State &state,Para &para,int node_id,vector<commu_ctrl>&tSendDG) {
	commu_ctrl a;
	size_t i,j;
	//unsigned char end=0xff; 
	a.Startnode_ID=node_id;
	a.PowerSimTime=para.time;
	a.Converge_Flag=state.flag_converge;

	//始发节点是牵引节点
	if(a.Startnode_ID==6){   
		a.data[0]=state.miu_pin;
		for( i = 1; i <=NODE_NUM; i++) {
			if (para.D[i-1] != 0) {
			a.Destnode_ID = i;     //与牵引节点相连节点
			a.data[0]=state.miu_pin;
			tSendDG.push_back(a);
			}
		}
	}
	//始发节点不是牵引节点
	else{       
		for (j = 1; j <= NODE_NUM ; j++) {
			if (para.B[node_id-1][j-1] != 0) {
				a.Destnode_ID = j;     //与之相连节点
				a.data[0]=state.miu_trans_step[node_id-1];
				tSendDG.push_back(a);
			}
		}
    }
}
//接收通信软件下发至算法的各节点数据，tRecvDG为数据包结构体数组
void DownloadDGPacket(State &state,Para &para,vector<commu_ctrl>&tRecvDG){
	//---先要获得数据包tRecvDG
    int len=tRecvDG.size();
	state.flag_converge=0;

	for(int i=0;i<len;i++){
		packet_prase(tRecvDG[i],state,para);   //解析数据包
	}

	dis_ctrl_pin_node(state,para);
	memcpy(state.miu_previous_step,state.miu_hold_step,sizeof(state.miu_previous_step));
	memcpy(state.miu_hold_step,state.miu_trans_step,sizeof(state.miu_trans_step));

    FILE *fp;
        fopen_s(&fp,"power_application_iterator_data.txt","a");
        ExportIterationResult(state,para,fp);   //记录迭代数据
        fclose(fp);


}
//电力应用返回电力软件控制指令
static void api_struct_feedback(State state,Para para,DG_Node dg_node[],Ctrl_to_Grid pct[]){
	double dP[NODE_NUM];
	printf("************** 向电力软件返回完成算法结果数据【开始】**************\n\n");
	for(int i=0;i<NODE_NUM;i++){
		/*
		dP[i]=state.miu_trans_step[i]*para.Capacity[i]*100;
		dP[i]=(-dP[i])/dg_Node[i].generation_value;
		printf("dP:%0.5f ",dP[i]);
		*/
		dP[i]=(state.miu_this_time[i]-state.miu_previous_time[i])*para.Capacity[i]*100;
		if(dg_node[i].generation_value>0){
			dP[i]=(-dP[i])/dg_node[i].generation_value;  //返回电力结果为当前时刻各节点的有功调整比例
		}
		else{
			dP[i]=(dP[i])/dg_node[i].generation_value;
		}

			//输出为下发至电力软件数据
		for(int m=0;m<NODE_NUM;m++)
		{
			pct[m].DG_node_ID=m+1;
			pct[m].dP=dP[m];
			pct[m].PowerUpadateTime=para.time;
		}
	}
	
	for(int i=0;i<NODE_NUM;i++){
		printf("DG_Node_ID:%0f",pct[i].DG_node_ID);
		printf(" dP:%0.5f ",dP[i]);
		printf("%% \n");
	}
	printf("************** 向电力软件返回完成算法结果数据【完成】**************\n\n");
}

void getDemand(State &state,Para &para,bus_info_commu &bus1,bus_info_commu &bus2){
	para.Freq=DownloadSamplingFreq(para,&bus2);  //通信软件向电力应用下发母线信息
	para.P_Demand=setDemand(para,&(state.flag_miu));  //调用控制中心算法计算缺额功率
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Para g_para;
static State g_state;
static DG_Node g_dgnode[5];
static bus_info_commu g_upload_bus;
static bus_info_commu g_dwload_bus;

void initialize_env()
{
        initialize(g_state, g_para);          //初始化算法参数
        double init_value[5] = {0.006666, 0.011664, 0.006667, 0.011667, 0.011663};
        for(int i=0;i<5;++i){
                g_dgnode[i].DG_NO=i+1;
                g_dgnode[i].simtime=5.0;
                g_dgnode[i].generation_value=init_value[i];
        }
}

void generate_upload_sample_freq(const Bus_Info &bus, bus_info_commu& upload_bus_infor)
{
        UploadSamplingFreq(bus, &g_upload_bus);
        memcpy(&upload_bus_infor, &g_upload_bus, sizeof(bus_info_commu));
}

void config_param_state(const bus_info_commu& dwload_bus_infor)
{
        memcpy(&g_dwload_bus, &dwload_bus_infor, sizeof(bus_info_commu));
        getDemand(g_state, g_para, g_upload_bus, g_dwload_bus);
        dis_ctrl_pin_node(g_state,g_para);
}

void generate_upload_dgpacket(vector<commu_ctrl>& tSendDG)
{
        for(int i=1;i<=6;i++){
                UploadDGPacket(g_state, g_para, i, tSendDG);
        }
}

bool iterator_calculate(vector<commu_ctrl>& tRecvDG, Ctrl_to_Grid* dg_feedback)
{
        DownloadDGPacket(g_state, g_para, tRecvDG);    //通信软件向电力应用下发数据包
        tRecvDG.clear();
        if(g_state.flag_converge == 0){  //迭代收敛
                memcpy(g_state.miu_this_time, g_state.miu_trans_step, sizeof(g_state.miu_trans_step));
                api_struct_feedback(g_state, g_para, g_dgnode, dg_feedback);     //向电力软件程序返回控制指令
                memcpy(g_state.miu_previous_time, g_state.miu_trans_step, sizeof(g_state.miu_trans_step));
                //保存此次仿真结果，用于下次计算功率调节量
        }

        return g_state.flag_converge == 0;
}





////一次迭代交互
//void Interact(State &state,Para &para,vector<commu_ctrl>&tRecvDG,DG_Node dg_node[],Ctrl_to_Grid dg_feedback[]){
	    
//		DownloadDGPacket(state,para,tRecvDG);    //通信软件向电力应用下发数据包
//		tRecvDG.clear();
//			if(!state.flag_converge)  //迭代收敛
//			{
//				memcpy(state.miu_this_time,state.miu_trans_step,sizeof(state.miu_trans_step));

//				api_struct_feedback(state,para,dg_node,dg_feedback);     //向电力软件程序返回控制指令【
//				memcpy(state.miu_previous_time,state.miu_trans_step,sizeof(state.miu_trans_step));
//				//保存此次仿真结果，用于下次计算功率调节量
//			}
//			else{
//				state.flag_converge=0;
//			}
//}
//void test(Bus_Info &bus,DG_Node dg_node[],Ctrl_to_Grid dg_feedback[]){
//	Para para;
//	State state;a

//	initialize(state,para);  //初始化算法参数

//	vector<commu_ctrl>tSendDG;  //电力应用上传至通信软件数据包
//	vector<commu_ctrl>tRecvDG;  //通信软件下发至电力应用数据包

//	bus_info_commu bus1;            //电力应用上传通信的母线数据
//	UploadSamplingFreq(bus,&bus1);  //电力软件通过电力算法模块向通信软件上传母线
//	bus_info_commu bus2;          //通信软件下发至电力应用的母线数据

//	getDemand(state,para,bus1,bus2);
//	for(int i=1;i<=6;i++){
//		UploadDGPacket(state,para,i,tSendDG);
//	}
//	Interact(state,para,tRecvDG,dg_node,dg_feedback); //一次迭代交互

//	return ;
//}

