#include "LFU.hpp"
equivalence_class_num_type LFU::get_the_number_of_equivalence_classes() const
{
    cout << "cache_manager_LRU::get_the_number_of_equivalence_classes()const not need to acomplish" << endl;
    exit(0);
    return 0;
}

data_cell_num_type LFU::get_the_number_of_data_units() const
{
    return LFU_Container.size();
}

bool LFU::query_in_cache(queryItem &queryItem)
{
    if (LFU_Container.empty())
    {
        return false;
    }
    bool flag = false;
    for (auto &a : LFU_Container)
    {
        if (a.getDims() == queryItem.getDims())
        {
            queryItem.setMeasure(a.getMeasure());
            a.add_LFU_count();
            flag = true;
            break;
        }
    }
    if (flag)
    {

        return true;
    }
    else
    {
        return false;
    }
}

void LFU::refresh_cache(const queryItem &queryItem)
{
    LFU_Container.push_back(queryItem);
    while (get_the_number_of_data_units() > data_cells_limit_size)
    {
        cache_replacement_policy();
    }
}

void LFU::cache_replacement_policy()
{
    auto min = LFU_Container.begin();
    for (auto a = LFU_Container.begin()++; a != LFU_Container.end(); ++a)
    {
        if (a->get_LFU_count() < min->get_LFU_count())
        {
            min = a;
        }
    }
    LFU_Container.erase(min);
}

vector<queryItem> LFU::load_query_file()
{
    vector<vector<string>> temp_queryItemSet;
    ifstream infile(querySetFilePath);
    string temp;
    if (!infile.is_open())
    {
        cout << querySetFilePath + "the file is not opened under the path" << endl;
        exit(0);
    }
    while (getline(infile, temp))
    {
        vector<string> tempvec;
        SplitString(temp, tempvec, ",");
        temp_queryItemSet.push_back(tempvec);
    }
    infile.close();
    vector<queryItem> ret_queryItemSet;
    for (auto a : temp_queryItemSet)
    {
        dims_type temp_dims;
        measure_type temp_measure = 0.0;
        for (int i = 0; i < dimsNum; ++i)
        {
            temp_dims[i] = atoi(a[i].c_str());
        }
        queryItem temp_queryItem(temp_dims, temp_measure);
        ret_queryItemSet.push_back(temp_queryItem);
    }
    return ret_queryItemSet;
}

void LFU::displasy_cachaItem() const
{
    cout << endl
         << endl
         << "the number of the data cells:" << get_the_number_of_data_units() << endl;
    for_each(LFU_Container.begin(), LFU_Container.end(), [](const queryItem &every_quertItem)
             { every_quertItem.display(); });
}

void LFU::dispaly_experimental_result_to_file(data_cell_num_type queryItemSet_size, int hit, time_t all_time) const
{
    ofstream to_file;
    to_file.open(basePath + "LFUalg.txt", ios::trunc);
    if (!to_file.is_open())
    {
        cout << "fail to write results into files" << endl;
        exit(0);
    }
    to_file << "alg name:LFU" << endl;
    to_file << "the number ofquery items:" << queryItemSet_size << endl;
    to_file << "cache limit(the number of the data cells):" << data_cells_limit_size << endl;
    to_file << "hit number:" << hit << endl;
    to_file << "hit rate:" << (float)hit / queryItemSet_size << endl;
    to_file << "total use time(s):" << all_time << endl;
    to_file << "the number of data cells:" << get_the_number_of_data_units() << endl;
}

vector<queryItem> LFU::loadQueryFile()
{
    vector<queryItem> ret_queryItemSet;
    return ret_queryItemSet;
}
