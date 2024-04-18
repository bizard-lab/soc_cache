#include "LRU.hpp"
equivalence_class_num_type LRU::get_the_number_of_equivalence_classes() const
{
    cout << "cache_manager_LRU::get_the_number_of_equivalence_classes()const not need to accomplish,if error,quit" << endl;
    exit(0);
    return 0;
}
data_cell_num_type LRU::get_the_number_of_data_units() const
{
    return LRU_Container.size();
}
bool LRU::query_in_cache(queryItem &queryItem)
{
    bool flag = false;
    if (LRU_Container.empty())
    {
        return false;
    }
    for (auto iter = LRU_Container.begin(); iter != LRU_Container.end(); ++iter)
    {
        if (iter->getDims() == queryItem.getDims())
        {
            queryItem.setMeasure(iter->getMeasure());
            LRU_Container.push_front(*iter);
            LRU_Container.erase(iter);
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
void LRU::refresh_cache(const queryItem &queryItem)
{
    LRU_Container.push_front(queryItem);
    while (get_the_number_of_data_units() > data_cells_limit_size)
    {
        cache_replacement_policy();
    }
}

void LRU::cache_replacement_policy()
{
    LRU_Container.pop_back();
}
vector<queryItem> LRU::load_query_file()
{
    vector<vector<string>> temp_queryItemSet;
    ifstream infile(querySetFilePath);
    string temp;
    if (!infile.is_open())
    {
        cout << querySetFilePath + "the file fail to open on this path" << endl;
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
        dims_type temp_dims = {0};
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

void LRU::displasy_cachaItem() const
{
    cout << endl
         << endl
         << "the number of data cells:" << get_the_number_of_data_units() << endl;
    for_each(LRU_Container.begin(), LRU_Container.end(), [](const queryItem &every_queryItem)
             { every_queryItem.display(); });
}

void LRU::dispaly_experimental_result_to_file(data_cell_num_type queryItemSet_size, int hit, time_t all_time) const
{
    ofstream alg;
    alg.open(basePath + "LRUalg.txt", ios::trunc);
    if (!alg.is_open())
    {
        cout << "fail to write results into files" << endl;
        exit(0);
    }
    alg << "alg name:LRU" << endl;
    alg << "the number of query items" << queryItemSet_size << endl;
    alg << "cache limit size(the number of data cells" << data_cells_limit_size << endl;
    alg << "hit times" << hit << endl;
    alg << "hit rate:" << (float)hit / queryItemSet_size << endl;
    alg << "total use time(s)" << all_time << endl;
    alg << "the number of data cells:" << get_the_number_of_data_units() << endl;
}

vector<queryItem> LRU::loadQueryFile()
{
    vector<queryItem> ret_queryItemSet;
    return ret_queryItemSet;
}
