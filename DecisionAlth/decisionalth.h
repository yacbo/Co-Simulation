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
    void execute_event_proc_algorithm(const ProcEventParam* param, void* customData);
};

#endif // DECISIONALTH_H
