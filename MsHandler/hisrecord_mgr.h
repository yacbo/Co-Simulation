#ifndef HISRECORD_MGR_H
#define HISRECORD_MGR_H

#include "comm_type_def.h"
#include "power_data_def.h"

class HisRecordMgr
{
private:
    HisRecordMgr();
    virtual ~HisRecordMgr();

public:
    static HisRecordMgr* instance();

public:
    bool load_recorde(int power_data_type, UnionSimDatVec& vec, int64_t sim_time = -1);
    bool write_recorde(int64_t sim_time, int power_data_type, const UnionSimDatVec& vec);
    bool fill_recorde(int power_data_type, const UnionSimDatVec& his, UnionSimDatVec& recorde);

private:
    bool load_businfor_recorde(UnionSimDatVec& vec, int64_t sim_time);
    bool write_businfor_recorde(int64_t sim_time, const UnionSimDatVec& vec);
    bool fill_businfor_recorde(const UnionSimDatVec& his, UnionSimDatVec& recorde);

private:
    bool load_file_list(StrVec& vec);

private:
    const string _rec_name_prefix;
    const string _rec_name_postfix;

private:
    StrVec _file_list_vec;
};

#endif // HISRECORD_MGR_H
