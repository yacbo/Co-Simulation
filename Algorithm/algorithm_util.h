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
    bool Alth_Calculate_dVg(const char* sim_data, int bus_num,  double* dvg_ret);
    bool Alth_Calculate_CtrlOrder(const char* sim_data, char* order_ret);
    bool Alth_Calculate_Iterator(const char* sim_data,  char* order_ret);

private:
    const char* Parse_Iterator_data(const char* sim_data, int& type, int& length, int& count);
    void Generate_Upload_Iterator_data(char* iter_data);
    void Generate_Iterator_Converge_data(const void* dgfb, int count, char* converge_data);
};

#endif // ALGORITHM_H
