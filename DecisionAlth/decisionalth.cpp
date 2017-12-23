
#include "comm_table.h"
#include "decisionalth.h"
#include "algorithm_util.h"


DecisionAlth* GetInstance()
{
    return DecisionAlth::get_instance();
}

bool CalculateDvg(const char* data, double* ret)
{
    ProcEventParam param;
    param._in_out_info = data;

    DecisionAlth* alth = DecisionAlth::get_instance();

    alth->ApplReq_ProcedureRequestSendBefore(&param, ret);

    return param._handle_success;
}

DecisionAlth::DecisionAlth()
{

}

DecisionAlth* DecisionAlth::get_instance()
{
    static DecisionAlth _decision_alth;
    return &_decision_alth;
}

//appl request
void DecisionAlth::ApplReq_ProcedureRequestSendBefore(const ProcEventParam* param, void* customData)
{
    if(!param || !customData){
        return;
    }

    AlgorithmUtil* util = new AlgorithmUtil();
    ProcEventParam* ev_p = const_cast<ProcEventParam*>(param);
    ev_p->_handle_success = util->Alth_Calculate_dVg(ev_p->_in_out_info.c_str(), ev_p->bus_num,  (double*)customData);
    delete util;
}

void DecisionAlth::ApplReq_ProcedureRequestSendAfter(const ProcEventParam* param, void* customData)
{

}

void DecisionAlth::ApplReq_ProcedureRequestSendReceived(const ProcEventParam* param, void* customData)
{

}

void DecisionAlth::ApplReq_ProcedureResponseSendAfter(const ProcEventParam* param, void* customData)
{

}

void DecisionAlth::ApplReq_ProcedureResponseConfirmReceived(const ProcEventParam* param, void* customData)
{

}

void DecisionAlth::ApplReq_ProcedureResponseErrorReceived(const ProcEventParam* param, void* customData)
{

}

//data send
void DecisionAlth::DataSnd_ProcedureRequestSendBefore(const ProcEventParam* param, void* customData)
{

}

void DecisionAlth::DataSnd_ProcedureRequestSendAfter(const ProcEventParam* param, void* customData)
{

}

void DecisionAlth::DataSnd_ProcedureRequestSendReceived(const ProcEventParam* param, void* customData)
{

}

void DecisionAlth::DataSnd_ProcedureResponseSendAfter(const ProcEventParam* param, void* customData)
{

}

void DecisionAlth::DataSnd_ProcedureResponseConfirmReceived(const ProcEventParam* param, void* customData)
{

}

void DecisionAlth::DataSnd_ProcedureResponseErrorReceived(const ProcEventParam* param, void* customData)
{

}

//invoke
void DecisionAlth::Invoke_ProcedureRequestSendBefore(const ProcEventParam* param, void* customData)
{

}

void DecisionAlth::Invoke_ProcedureRequestSendAfter(const ProcEventParam* param, void* customData)
{

}

void DecisionAlth::Invoke_ProcedureRequestSendReceived(const ProcEventParam* param, void* customData)
{

}

void DecisionAlth::Invoke_ProcedureResponseSendAfter(const ProcEventParam* param, void* customData)
{

}

void DecisionAlth::Invoke_ProcedureResponseConfirmReceived(const ProcEventParam* param, void* customData)
{

}

void DecisionAlth::Invoke_ProcedureResponseErrorReceived(const ProcEventParam* param, void* customData)
{

}

