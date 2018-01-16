#ifndef ALGORITHM_DISCONTROL_H
#define ALGORITHM_DISCONTROL_H

#include "ctrl_commu.h"
#ifdef __cplusplus
extern "C" {
#endif

#define NODE_NUM 5
#define MIU double
#define ActivePower double

	/*typedef struct _ctrl_commu {       //控制C平台-通信软件结构体   
		int Startnode_ID;
		int Destnode_ID;
		bool state1;
		bool state2;
		double data[16];
	}ctrl_commu;
	*/
	typedef struct _state{
		MIU miu_pin;
		MIU miu_trans_step[NODE_NUM];
		MIU miu_hold_step[NODE_NUM];
		MIU miu_previous_step[NODE_NUM];
		bool flag_converge;
		bool flag_miu;
		MIU miu_this_time[NODE_NUM];
		MIU miu_previous_time[NODE_NUM];
	}State;

	typedef struct _para{
		int B[NODE_NUM][NODE_NUM];  //连接矩阵
		int L[NODE_NUM][NODE_NUM];
		int D[NODE_NUM];
		ActivePower Capacity[NODE_NUM];  //可调节能力
		ActivePower P_Demand;   //缺额功率
		ActivePower P_Realtime;  //迭代状态量
		double time;
		double t_hold;
		double step;  //步长
		double beta;   
		double kappa;
		double Freq;    //电网频率
		double Kg;     //调节系数
	}Para;

	extern State state;
	extern Para para;

	void initialize(State &state,Para &para){
		para.step= 0.001;
		para.beta = 300;   //coupling strength for distributed protocol
		para.kappa = 800;   //positive gain factor
		para.Freq=50;
		para.time=0;
		para.t_hold=0;
		para.P_Demand=0;
		para.P_Realtime=0;
		ActivePower capacity[NODE_NUM] = { 0.020, 0.035, 0.020, 0.035, 0.035 }; //每个节点的可调节能力,单位MW
		int B[NODE_NUM][NODE_NUM] =
		{ { 0,1,0,0,1 },
		{ 1,0,1,0,0 },
		{ 0,1,0,1,0 },
		{ 0,0,1,0,1 },
		{ 1,0,0,1,0 },
		};    //输入连接矩阵，1表示相连，0表示无连接
		int D[NODE_NUM] = { 1,0,0,0,0 };  //与牵引点的连接关系，1为连接，0为不连接
		int L[NODE_NUM][NODE_NUM] = { 0 };
		para.Kg=0.0004;
		state.miu_pin=0;

		memset(state.miu_previous_step,0,sizeof(state.miu_previous_step));
		memset(state.miu_hold_step,0,sizeof(state.miu_hold_step));
		memset(state.miu_trans_step,0,sizeof(state.miu_trans_step));
		memset(state.miu_previous_time,0,sizeof(state.miu_this_time));
		memset(state.miu_this_time,0,sizeof(state.miu_this_time));
		memcpy(para.B,B,sizeof(B));
		memcpy(para.D,D,sizeof(D));
		memcpy(&para.L,L,sizeof(L));
		memcpy(&para.Capacity,capacity,sizeof(capacity));
	}
	ActivePower setDemand(Para &para,bool *flag) {
		double k=para.Kg;
		double dF=para.Freq-50;
		*flag=0;
		if(dF>0.05||dF<-0.05){     //频率偏差量的绝对值大于0.05才开始调节
			*flag=1;               //示牵引率将在调节中发生变化，若为0则牵引率不变，单各个节点参与度会向牵引率靠近
			para.t_hold=para.time;
			return -k*1.2 * dF*1000;    //需求量计算
		}  
		else if(dF>0.04||dF<0.04){  //当偏差回到一个范围内后实行需求量保持不变的策略
			*flag=1;
			return para.P_Realtime;
		}
		else{
			*flag=1;
			if(para.time>=para.t_hold+8){       //当调节超过8秒后，将逐步减少需求量
				return para.P_Realtime*0.95;
			}
			else{
				return para.P_Realtime;
			}
		}

	}
	void dis_ctrl_pin_node(State &state,Para &para) {

		MIU miu_pin=state.miu_pin;
		MIU *miu_trans_step=state.miu_trans_step;
		const double *capacity=para.Capacity;
		const double step=para.step;
		const double kappa=para.kappa;

		para.P_Realtime=0;
		for (int i = 0; i <= NODE_NUM - 1; i++){
			para.P_Realtime += miu_trans_step[i] * capacity[i];  //计算各节点迭代总功率
		}
		miu_pin = miu_pin + step*kappa*(para.P_Demand - para.P_Realtime);   //设置牵引率上下限为（0，1）
		miu_pin = (miu_pin<1)*miu_pin + (miu_pin>1);
		miu_pin = miu_pin - miu_pin*(miu_pin<0);

		state.miu_pin=miu_pin;
	}
	bool converge(State &state,commu_to_ctrl tRecv){   //判断收敛
		bool flag=state.flag_converge;
		bool flag_tmp=state.flag_converge;
		double precision = 1.0e-4, delta;
		if (state.miu_trans_step[tRecv.Destnode_ID-1]==0)
		{
			delta=0;
		}
		else
		{
			delta = state.miu_trans_step[tRecv.Destnode_ID-1] - tRecv.data[0];
			delta = fabs(delta) / state.miu_trans_step[tRecv.Destnode_ID-1];
		}
		if (delta > precision)
			flag = 1;
		else flag = 0;
		flag = flag | flag_tmp;

		return flag;

	}
	void dis_ctrl_DG_node(commu_to_ctrl tRecv,State&state,Para &para) {
		MIU *miu_hold_step=state.miu_hold_step;
		MIU *miu_previous_step=state.miu_previous_step;
		MIU *miu_trans_step=state.miu_trans_step;
		const double step=para.step;
		const double beta=para.beta;

		if ((tRecv.Startnode_State == 1) && (tRecv.Destnode_State == 1))     //目的地与始发地均正常
		{
			miu_trans_step[tRecv.Destnode_ID-1] =
				miu_trans_step[tRecv.Destnode_ID-1] - step*beta*(miu_hold_step[tRecv.Destnode_ID-1] - tRecv.data[0]);
		}

		if ((tRecv.Startnode_State == 0) && (tRecv.Destnode_State== 1))     //始发地不正常，目的地正常
		{
			miu_trans_step[tRecv.Destnode_ID-1] =
				miu_hold_step[tRecv.Destnode_ID-1] - step*beta*(miu_hold_step[tRecv.Destnode_ID-1] - miu_previous_step[tRecv.Startnode_ID-1]);
		}
		if ((tRecv.Startnode_State == 1) && (tRecv.Destnode_State== 0))     //始发地正常，目的地不正常
		{
			miu_trans_step[tRecv.Destnode_ID-1]= miu_hold_step[tRecv.Destnode_ID-1];
			miu_hold_step[tRecv.Destnode_ID-1] =
				miu_hold_step[tRecv.Destnode_ID-1] - step*beta*(miu_hold_step[tRecv.Destnode_ID-1] - tRecv.data[0]);
		}
		if ((tRecv.Startnode_State== 0) && (tRecv.Destnode_State == 0))     //始发地和目的地均不正常
		{
			miu_trans_step[tRecv.Destnode_ID-1]= miu_hold_step[tRecv.Destnode_ID-1];
			miu_hold_step[tRecv.Destnode_ID-1] =
				miu_hold_step[tRecv.Destnode_ID-1] - step*beta*(miu_hold_step[tRecv.Destnode_ID-1] - miu_previous_step[tRecv.Startnode_ID-1]);
		}

		//更新收敛标志,0表示收敛，1表示不收敛
		state.flag_converge=converge(state,tRecv);

	}
	int ExportIterationResult(State state, FILE *fp) {
		char c = '\n';
		for (int i = 0; i < NODE_NUM ; i++) {
			//fprintf(fp, "%0.9f ", state.miu_trans_step[i]);
			printf("%0.9f ", state.miu_trans_step[i]);
		}
		fprintf(fp, "%c", c);
		printf("%c",c);
		return 0;
	}

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ALGORITHM_DISCONTROL_H */