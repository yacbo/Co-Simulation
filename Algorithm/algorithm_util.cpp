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

bool AlgorithmUtil::Alth_Calculate_dVg(const char* sim_data,  int bus_num, double* dvg_ret)
{
    std::string tmp;
    StrVec data_vec;
    std::istringstream istr(sim_data);
    while(istr >> tmp){
        data_vec.push_back(tmp);
    }

    BusInfor* businfo = new BusInfor[bus_num];
    for (int m = 0; m < bus_num; ++m){
        businfo[m].busno = m + 1;
        businfo[m].Volt = (double)atof(data_vec[2 * m + 1].c_str());
        businfo[m].V_ang = (double)atof(data_vec[2 * m + 2].c_str());
    }

    bool ret = CalculateDVG(&businfo[0], bus_num, dvg_ret);
    delete[] businfo;

    return ret;
}
