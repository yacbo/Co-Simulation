
#include "comm_table.h"
#include "decisionalth.h"
#include "algorithm_util.h"

DecisionAlth::DecisionAlth()
{

}

DecisionAlth* DecisionAlth::get_instance()
{
    static DecisionAlth _decision_alth;
    return &_decision_alth;
}

//appl request
void DecisionAlth::execute_event_proc_algorithm(const ProcEventParam* param, void* customData)
{
    if(!param || !customData){
        return;
    }

    AlgorithmUtil* util = new AlgorithmUtil();
    ProcEventParam* ev_p = const_cast<ProcEventParam*>(param);
    switch(ev_p->_type){
    case ePowerData_businfor:{
        ev_p->_handle_success = util->Alth_Calculate_dVg(ev_p->_in_out_info.c_str(), ev_p->_bus_num,  (double*)customData);
        break;
    }
    default: break;
    }

    delete util;
}

