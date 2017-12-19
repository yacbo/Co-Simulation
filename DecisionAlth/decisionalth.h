#ifndef DECISIONALTH_H
#define DECISIONALTH_H

#include "decisionalth_global.h"

struct ProcEventParam;
class DECISIONALTHSHARED_EXPORT DecisionAlth
{
private:
    DecisionAlth();

public:
    static DecisionAlth* get_instance();

public:
    //appl request
    void ApplReq_ProcedureRequestSendBefore(const ProcEventParam* param, void* customData);
    void ApplReq_ProcedureRequestSendAfter(const ProcEventParam* param, void* customData);
    void ApplReq_ProcedureRequestSendReceived(const ProcEventParam* param, void* customData);
    void ApplReq_ProcedureResponseSendAfter(const ProcEventParam* param, void* customData);
    void ApplReq_ProcedureResponseConfirmReceived(const ProcEventParam* param, void* customData);
    void ApplReq_ProcedureResponseErrorReceived(const ProcEventParam* param, void* customData);

    //data send
    void DataSnd_ProcedureRequestSendBefore(const ProcEventParam* param, void* customData);
    void DataSnd_ProcedureRequestSendAfter(const ProcEventParam* param, void* customData);
    void DataSnd_ProcedureRequestSendReceived(const ProcEventParam* param, void* customData);
    void DataSnd_ProcedureResponseSendAfter(const ProcEventParam* param, void* customData);
    void DataSnd_ProcedureResponseConfirmReceived(const ProcEventParam* param, void* customData);
    void DataSnd_ProcedureResponseErrorReceived(const ProcEventParam* param, void* customData);

    //invoke
    void Invoke_ProcedureRequestSendBefore(const ProcEventParam* param, void* customData);
    void Invoke_ProcedureRequestSendAfter(const ProcEventParam* param, void* customData);
    void Invoke_ProcedureRequestSendReceived(const ProcEventParam* param, void* customData);
    void Invoke_ProcedureResponseSendAfter(const ProcEventParam* param, void* customData);
    void Invoke_ProcedureResponseConfirmReceived(const ProcEventParam* param, void* customData);
    void Invoke_ProcedureResponseErrorReceived(const ProcEventParam* param, void* customData);
};

extern "C" DECISIONALTHSHARED_EXPORT DecisionAlth* GetInstance();
extern "C" DECISIONALTHSHARED_EXPORT bool  CalculateDvg(const char* data, double* ret);

#endif // DECISIONALTH_H
