#ifndef HISRECORD_MGR_H
#define HISRECORD_MGR_H

#include "comm_type_def.h"

class hisrecord_mgr
{
public:
    hisrecord_mgr();
    virtual ~hisrecord_mgr();

public:
    bool load_recodes(UnionSimDatVec& vec, int64_t sim_time = -1 );
    bool write_recordes(int64_t sim_time, const UnionSimDatVec& vec);

private:
    bool load_file_list(StrVec& vec);

private:
    const string _rec_name_prefix;
    const string _rec_name_postfix;

private:
    StrVec _file_list_vec;
};

#endif // HISRECORD_MGR_H
