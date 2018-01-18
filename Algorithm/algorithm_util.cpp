#include "algorithm_util.h"

#include<string>
#include<vector>
#include<sstream>
#include "./Iterator/client.h"

typedef vector<std::string> StrVec;
typedef vector<StrVec> Mat;

AlgorithmUtil::AlgorithmUtil()
{
        _iterator_count = 0;
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

bool AlgorithmUtil::Alth_Calculate_CtrlOrder(const char* sim_data, char* order_ret)
{
        std::string tmp;
        StrVec data_vec;
        std::istringstream istr(sim_data);
        while(istr >> tmp){
                data_vec.push_back(tmp);
        }

        const int size_int = sizeof(int);
        const int size_2int = size_int << 1;
        const std::string& node = data_vec[1];
        double lne1_power = atof(data_vec[2].c_str());
        double lne2_power = atof(data_vec[3].c_str());

        int prefix_i = node.find_first_of('_');
        int post_i = node.find_last_of('_');

        int flag = 0;
        char gname[32] = {0};                                       //"sym_41051_1"
        strcpy_s(gname, node.c_str());
        int exe_sta_bus_id = std::stoi(node.substr(prefix_i + 1, post_i - prefix_i - 1));   //41051

        if(lne1_power < 0.001 && lne2_power < 0.001){
                memcpy(order_ret, &flag, size_int);
                memcpy(order_ret + size_int, &exe_sta_bus_id, size_int);
                memcpy(order_ret + size_2int,  gname, sizeof(gname));
        }

        return true;
}

bool AlgorithmUtil::Alth_Calculate_Iterator(const char* sim_data, char* order_ret)
{
        int type, length, count;
        const char* value = Parse_Iterator_data(sim_data, type, length, count);

        switch(type){
        case eInteract_power_bus: {
                initialize_env();

                _Bus_Info bus;
                memcpy(&bus, value, sizeof(_Bus_Info));

                _Bus_Info_Commu bus_upload;
                generate_upload_sample_freq(bus, bus_upload);

                count = 1;
                length = 0;
                type = eInteract_upload_bus;

                memcpy(order_ret, &type, sizeof(int)); length = 2 * sizeof(int);
                memcpy(order_ret + length, &count, sizeof(int)); length += sizeof(int);
                memcpy(order_ret + length, &bus_upload, sizeof(_Bus_Info_Commu)); length += sizeof(_Bus_Info_Commu);
                memcpy(order_ret + sizeof(int), &length, sizeof(int));
                break;
        }
        case eInteract_download_bus: {
                _Bus_Info_Commu bus;
                memcpy(&bus, value, sizeof(_Bus_Info_Commu));
                config_param_state(bus);

                Generate_Upload_Iterator_data(order_ret);
                break;
        }
        case eIneract_comm_to_ctrl: {
                int offset = 0;
                vector<Commu_Ctrl> rcv_vec(count);
                for(int i=0; i<count; ++i){
                        memcpy(&rcv_vec[i], value + offset, sizeof(Commu_Ctrl));
                        offset += sizeof(Commu_Ctrl);
                }

                _Ctrl_to_Grid grid[5];
                if(iterator_calculate(rcv_vec, grid)){              //收敛
                        Generate_Iterator_Converge_data(grid, 5, order_ret);
                }
                else{
                        Generate_Upload_Iterator_data(order_ret);
                }

                break;
        }
        default: break;
        }

        return true;
}

const char* AlgorithmUtil::Parse_Iterator_data(const char* sim_data, int& type, int& length, int& count)
{
        //encode format: type | length |  count | value
        int offset = 0;
        memcpy(&type, sim_data + offset, sizeof(int)); offset += sizeof(int);
        memcpy(&length, sim_data + offset, sizeof(int)); offset += sizeof(int);
        memcpy(&count, sim_data + offset, sizeof(int)); offset += sizeof(int);

        return sim_data + offset;
}

 void AlgorithmUtil::Generate_Upload_Iterator_data(char* iter_data)
 {
         int length = 0;
         int type = eIneract_ctrl_to_comm;

         vector<Commu_Ctrl>  snd_vec;
         generate_upload_dgpacket(snd_vec);

         int count = snd_vec.size();
         memcpy(iter_data, &type, sizeof(int)); length = 2 * sizeof(int);
         memcpy(iter_data + length, &count, sizeof(int)); length += sizeof(int);

         for(int i=0; i<count; ++i){
                 memcpy(iter_data + length, &snd_vec[i], sizeof(Commu_Ctrl));
                 length += sizeof(Commu_Ctrl);
         }

         memcpy(iter_data + sizeof(int), &length, sizeof(int));

         ++_iterator_count;
 }

 void AlgorithmUtil::Generate_Iterator_Converge_data(const void* dgfb,  int count, char* converge_data)
 {
          const _Ctrl_to_Grid* grid = (const _Ctrl_to_Grid*)dgfb;

          int length = 0;
          int type = eInteract_ctrl_to_power;

          memcpy(converge_data, &type, sizeof(int)); length = 2 * sizeof(int);
          memcpy(converge_data + length, &count, sizeof(int)); length += sizeof(int);

          for(int i=0; i<count; ++i){
                  memcpy(converge_data + length, &grid[i], sizeof(_Ctrl_to_Grid));
                  length += sizeof(_Ctrl_to_Grid);
          }

          memcpy(converge_data + sizeof(int), &length, sizeof(int));
 }
