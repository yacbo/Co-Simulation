#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "algorithm_global.h"
#include "dVg/dVgCalculation.h"

class ALGORITHMSHARED_EXPORT AlgorithmUtil
{
public:
    AlgorithmUtil();
    virtual ~AlgorithmUtil();

public:
    bool Alth_Calculate_dVg(const char* sim_data, double* dvg_ret);
};

#endif // ALGORITHM_H
