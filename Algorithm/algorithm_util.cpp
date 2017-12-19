#include "algorithm_util.h"

#include<string>
#include<vector>
#include<sstream>

typedef vector<std::string> StrVec;
typedef vector<StrVec> Mat;

AlgorithmUtil::AlgorithmUtil()
{

}

AlgorithmUtil::~AlgorithmUtil()
{

}

bool AlgorithmUtil::Alth_Calculate_dVg(const char* sim_data, double* dvg_ret)
{
    int nbus = 0;
    BusInfor businfo[39];

    std::string tmp;
    StrVec data_vec;
    std::istringstream istr(sim_data);
    while(istr >> tmp){
        data_vec.push_back(tmp);
    }

    nbus = data_vec.size();
    for (int m = 0; m < 39; ++m){
        businfo[m].busno = m + 1;
        businfo[m].Volt = (double)atof(data_vec[2 * m].c_str());
        businfo[m].V_ang = (double)atof(data_vec[2 * m + 1].c_str());
    }

    return CalculateDVG(&businfo[0], 39, dvg_ret);
}
