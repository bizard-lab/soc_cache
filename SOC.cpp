#include "SOC.hpp"
extern data_cell_num_type cache_data_cell_size;
equivalence_class_num_type SOC::get_the_number_of_equivalence_classes() const
{
    return SOC_Container.size();
}
data_cell_num_type SOC::get_the_number_of_data_units() const
{
    return cache_data_cell_size;
}
bool SOC::query_in_cache(queryItem &queryItem)
{
    bool match_up = false;
    bool match_low = false;
    if (get_the_number_of_data_units() == 0)
    {
        return false;
    }
    layer_type queryItem_index = queryItem.getLayer();
    cacheItem_Index::index<by_layer>::type &indexOfLayer = SOC_Container.get<by_layer>();
    cacheItem_Index::index<by_layer>::type::iterator start_index = indexOfLayer.upper_bound(0);
    cacheItem_Index::index<by_layer>::type::iterator end_index = indexOfLayer.lower_bound(queryItem_index + 1);
    while (start_index != end_index)
    {
        match_up = false;
        match_low = false;
        for (auto up : start_index->getUp())
        {
            if (cacheItem::cover(queryItem.getDims(), up))
            {
                match_up = true;
                break;
            }
            else
            {
                match_up = false;
            }
        }
        if (match_up)
        {
            if (start_index->getLow().empty())
            {
                if (cacheItem::cover(*start_index->getUp().begin(), queryItem.getDims()))
                {
                    match_low = true;
                }
                else
                {
                    match_low = false;
                }
            }
            else
            {
                for (auto low : start_index->getLow())
                {
                    if (cacheItem::cover(low, queryItem.getDims()))
                    {
                        match_low = true;
                        break;
                    }
                    else
                    {
                        match_low = false;
                    }
                }
            }
        }

        if (match_up && match_low)
        {
            queryItem.setMeasure(start_index->getMeasure());
            break;
        }
        ++start_index;
    }
    if (match_up && match_low)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SOC::refresh_cache(const queryItem &queryItem)
{
    if (get_the_number_of_equivalence_classes() == 0)
    {
        cacheItem new_cacheItem(queryItem);
        cache_data_cell_size++;
        SOC_Container.insert(new_cacheItem);
        return;
    }
    vector<cacheItem> merge_case1;
    vector<cacheItem> merge_case2;
    bool can_refresh_cache = false;

    cacheItem_Index::index<by_measure>::type &indexOfMeasure = SOC_Container.get<by_measure>();
    std::pair<cacheItem_Index::index<by_measure>::type::iterator, cacheItem_Index::index<by_measure>::type::iterator> pair_range = indexOfMeasure.equal_range(queryItem.getMeasure());
    while (pair_range.first != pair_range.second)
    {
        int state = 0;
        cacheItem temp = *pair_range.first;
        if (temp.addToCacheItem(queryItem.getDims(), state))
        {
            cache_data_cell_size = cache_data_cell_size - temp.getSz();
            indexOfMeasure.erase(pair_range.first++);
            can_refresh_cache = true;
            if (state == 1 || state == 3)
            {
                merge_case1.push_back(temp);
            }
            else if (state == 2 || state == 4)
            {
                merge_case2.push_back(temp);
            }
        }
        else
        {
            pair_range.first++;
        }
    }

    if (merge_case1.size() != 0)
    {
        vector<cacheItem>::iterator first = merge_case1.begin();
        for (auto iter = merge_case1.begin() + 1; iter != merge_case1.end(); ++iter)
        {
            first->merge_up(*iter);
        }
        first->update_eqClass_sz();
        first->update_state();
        first->update_ca();
        SOC_Container.insert(*first);
    }

    if (merge_case2.size() != 0)
    {
        vector<cacheItem>::iterator first = merge_case2.begin();
        for (auto iter = merge_case2.begin() + 1; iter != merge_case2.end(); ++iter)
        {
            first->merge_low(*iter);
        }
        first->update_eqClass_sz();
        first->update_state();
        first->update_ca();
        SOC_Container.insert(*first);
    }
    if (!can_refresh_cache)
    {
        SOC_Container.insert(cacheItem(queryItem));
        cache_data_cell_size++;
    }
    while (get_the_number_of_data_units() > data_cells_limit_size)
    {
        cache_replacement_policy();
    }
}

void SOC::cache_replacement_policy()
{
    auto min = SOC_Container.begin();
    for (auto a = SOC_Container.begin(); a != SOC_Container.end(); ++a)
    {
        if (a->getCa() < min->getCa())
        {
            min = a;
        }
    }
    cache_data_cell_size = cache_data_cell_size - min->getSz();
    SOC_Container.erase(min);
}

vector<queryItem> SOC::load_query_file()
{
    vector<vector<string>> temp_queryItemSet;
    ifstream infile(querySetFilePath);
    string temp;
    if (!infile.is_open())
    {
        cout << querySetFilePath + "file not open" << endl;
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

void SOC::displasy_cachaItem() const
{
    cout << endl
         << endl
         << "print all eal-classes:" << endl;
    for (auto a : SOC_Container)
    {
        a.display();
    }
    cout << endl
         << endl
         << "total number of equal classes:" << get_the_number_of_equivalence_classes() << endl;
}

void SOC::get_ca_frequency() const
{
    vector<int> ca_values;
    for (auto a : SOC_Container)
    {
        ca_values.push_back(a.getCa());
    }
    vector<int> temp_ca_values = ca_values;
    sort(ca_values.begin(), ca_values.end());
    ca_values.erase(unique(ca_values.begin(), ca_values.end()), ca_values.end());
    map<int, int> m;
    for (auto a : ca_values)
    {
        m.insert(make_pair(a, 0));
    }
    for (auto &a : m)
    {
        int count = 0;
        for (auto b : temp_ca_values)
        {
            if (a.first == b)
            {
                count++;
            }
        }
        a.second = count;
    }
    cout << "ca value and its frequence" << endl;
    for (auto a : m)
    {
        cout << "'" << a.first << "',";
    }
    cout << endl
         << endl;
    for (auto a : m)
    {
        cout << a.second << ",";
    }
    cout << endl
         << endl;
}

void SOC::dispaly_experimental_result_to_file(data_cell_num_type queryItemSet_size, int hit, time_t all_time) const
{
    ofstream alg;
    alg.open(basePath + soc_alg_name + "al.txt", ios::trunc);
    if (!alg.is_open())
    {
        cout << "fail to write result into file" << endl;
        exit(0);
    }
    alg << "alg name:" + soc_alg_name << endl;
    alg << "the number of query items:" << queryItemSet_size << endl;
    alg << "cache limit size(the number of data cells):" << data_cells_limit_size << endl;
    alg << "hit times:" << hit << endl;
    alg << "hit rate:" << ((float)hit) / queryItemSet_size << endl;
    alg << "total use time(s):" << all_time << endl;
    alg << "the number of data cells:" << get_the_number_of_data_units() << endl;
    alg << "the number of equal classes:" << get_the_number_of_equivalence_classes() << endl;
    vector<int> ca_values;
    for (auto a : SOC_Container)
    {
        ca_values.push_back(a.getCa());
    }
    vector<int> temp_ca_values = ca_values;
    sort(ca_values.begin(), ca_values.end());
    ca_values.erase(unique(ca_values.begin(), ca_values.end()), ca_values.end());
    map<int, int> m;
    for (auto a : ca_values)
    {
        m.insert(make_pair(a, 0));
    }
    for (auto &a : m)
    {
        int count = 0;
        for (auto b : temp_ca_values)
        {
            if (a.first == b)
            {
                count++;
            }
        }
        a.second = count;
    }
    alg << "print different ca and its frequence" << endl;
    for (auto a : m)
    {
        alg << "'" << a.first << "',";
    }
    alg << endl
        << endl;
    for (auto a : m)
    {
        alg << a.second << ",";
    }
    alg << endl
        << endl;
}

vector<queryItem> SOC::loadQueryFile()
{
    vector<queryItem> ret_queryItemSet;
    return ret_queryItemSet;
}
