#include "cacheItem.hpp"
using namespace std;
data_cell_num_type cache_data_cell_size = 0;
const dims_type cacheItem::conjunction(const dims_type &left, const dims_type &right)
{
    if (left == right)
    {
        return left;
    }
    dims_type res;
    for (int i = 0; i < dimsNum; ++i)
    {
        res[i] = left[i] == right[i] ? left[i] : (left[i] != 0 ? left[i] : right[i]);
    }
    return res;
}

ca_type cacheItem::std_ca(const pair<dims_type, dims_type> &region)
{
    const dims_type &up = region.first;
    const dims_type &low = region.second;
    layer_type layer_up = dimsNum - count(up.begin(), up.end(), 0);
    layer_type layer_low = dimsNum - count(low.begin(), low.end(), 0);
    return pow(2, layer_up - layer_low);
}

void Cij(int i, int j, vector<int> &r, int num, vector<vector<int>> &result)
{
    if (j == 1)
    {
        for (int k = 0; k < i; k++)
        {
            vector<int> temp(num);
            r[num - 1] = k;
            for (int i = 0; i < num; i++)
            {
                temp[i] = r[i];
            }
            result.push_back(temp);
        }
    }
    else if (j == 0)
    {
    }
    else
    {
        for (int k = i; k >= j; k--)
        {
            r[j - 2] = k - 1;
            Cij(k - 1, j - 1, r, num, result);
        }
    }
}
cacheItem::cacheItem(queryItem self_queryItem)
{
    upper.insert(self_queryItem.getDims());
    lowers.clear();
    measure = self_queryItem.getMeasure();
    sz = 1;
    ca = 1;
    low_min_layer = self_queryItem.getLayer();
}

void cacheItem::update_state()
{
    if (upper.size() != 1)
    {
        data_cell_num_type bef_up_size = upper.size();
        dims_type common_upper = {0};
        for (int i = 0; i < dimsNum; ++i)
        {
            for (auto every_upper : upper)
            {
                if (every_upper[i] != 0)
                {
                    common_upper[i] = every_upper[i];
                    break;
                }
            }
        }
        upper.clear();
        upper.insert(common_upper);
        cache_data_cell_size = cache_data_cell_size - bef_up_size + 1;
    }

    if (lowers.empty())
    {
        dims_type unique_upper = *upper.begin();
        low_min_layer = static_cast<int>(dimsNum - count(unique_upper.begin(), unique_upper.end(), 0));
    }
    else
    {
        int temp_layer = dimsNum;
        for_each(lowers.begin(), lowers.end(), [&temp_layer](const dims_type &every_lower)
                 {
            int every_layer = static_cast<int> (dimsNum - count(every_lower.begin(), every_lower.end(),0));
            if(temp_layer > every_layer){
                temp_layer = every_layer;
            } });
        low_min_layer = temp_layer;
    }
}
void cacheItem::update_eqClass_sz()
{
    sz = upper.size() + lowers.size();
    cache_data_cell_size = cache_data_cell_size + sz;
}
void cacheItem::update_ca()
{
    vector<dims_type> regions;
    map<int, dims_type> index;
    if (lowers.empty())
    {
        regions.push_back(*upper.begin());
    }
    else
    {
        for (auto every_lower : lowers)
        {
            regions.push_back(every_lower);
        }
    }
    int num = 0;
    ca = 0;
    for (int i = 0; i < regions.size(); ++i)
    {
        vector<int> resulttemp(i + 1);
        vector<vector<int>> result;
        Cij((int)regions.size(), i + 1, resulttemp, i + 1, result);
        num = 0;
        for (auto a : result)
        {
            dims_type temp = {0};
            string index_str;
            for (auto b : a)
            {
                temp = cacheItem::conjunction(regions[b], temp);
            }
            num += cacheItem::std_ca(make_pair(*upper.begin(), temp));
        }
        resulttemp.clear();
        result.clear();
        if (i % 2 != 0)
        {
            ca += -1 * num;
        }
        else
        {
            ca += num;
        }
    }
}

bool cacheItem::cover(const dims_type &left_queryItem_dims, const dims_type &right_queryItem_dims)
{
    bool flag = false;
    for (int i = 0; i < dimsNum; ++i)
    {
        if ((left_queryItem_dims[i] == 0) || (left_queryItem_dims[i] == right_queryItem_dims[i]))
        {
            flag = true;
        }
        else
        {
            flag = false;
            break;
        }
    }
    return flag;
}

bool cacheItem::addToCacheItem(const dims_type &queryItem_dims, int &state)
{
    bool only_cover_low = false;
    if (lowers.empty())
    {
        if (cacheItem::cover(queryItem_dims, *upper.begin()))
        {
            cout << "the lower bound that query items can cover cache items" << endl;
            state = 2; // 4
            lowers.insert(queryItem_dims);
            update_state();
            cache_data_cell_size++;
            return true;
        }
        else if (cacheItem::cover(*upper.begin(), queryItem_dims))
        {
            state = 1; // 3
            cout << "the lower bound that cache items can cover query items" << endl;
            lowers.insert(*upper.begin());
            upper.clear();
            upper.insert(queryItem_dims);
            cache_data_cell_size++;
            return true;
        }
    }
    else
    {
        if (cacheItem::cover(*upper.begin(), queryItem_dims))
        {
            state = 1;
            cout << "the upper bound of the cache item can cover query item" << endl;
            upper.clear();
            upper.insert(queryItem_dims);
            return true;
        }
        else
        {
            for (auto iter = lowers.begin(); iter != lowers.end(); ++iter)
            {
                if (cacheItem::cover(*iter, queryItem_dims))
                {
                    state = 3;
                    cout << "the lower bound of the cache item can cover query item" << endl;
                    upper.insert(queryItem_dims);
                    cache_data_cell_size++;
                    update_state();
                    return true;
                }
            }
        }
        for (auto iter = lowers.begin(); iter != lowers.end();)
        {
            if (cacheItem::cover(queryItem_dims, *iter))
            {
                cout << "query items can cover the lower bound of the cache items" << endl;
                state = 4;
                lowers.erase(iter++);
                cache_data_cell_size--;
                only_cover_low = true;
            }
            else
            {
                ++iter;
            }
        }
        if (only_cover_low)
        {
            lowers.insert(queryItem_dims);
            update_state();
            cache_data_cell_size++;
            return true;
        }

        if (cacheItem::cover(queryItem_dims, *upper.begin()))
        {
            state = 2;
            cout << "query items only cover the uppqer bound of the cache items" << endl;
            lowers.insert(queryItem_dims);
            cache_data_cell_size++;
            update_state();
            return true;
        }
    }

    return false;
}

void cacheItem::merge_up(const cacheItem &merged_cacheItem)
{
    for (auto up : merged_cacheItem.getUp())
    {
        upper.insert(up);
    }
    for (auto low : merged_cacheItem.getLow())
    {
        lowers.insert(low);
    }
}

void cacheItem::merge_low(const cacheItem &merged_cacheItem)
{
    for (auto up : merged_cacheItem.getUp())
    {
        upper.insert(up);
    }
    for (auto low : merged_cacheItem.getLow())
    {
        lowers.insert(low);
    }
}

void cacheItem::display() const
{
    cout << "one upper bound:" << endl;
    for_each(upper.begin(), upper.end(), [](const dims_type &every_upper)
             {
        for_each(every_upper.begin(), every_upper.end(), [](const int& dims ){
            cout<<dims<<",";
        });
        cout<<endl; });
    cout << endl
         << "multi lower bounds: (if no upper bounds,lower bound itself" << endl;
    for_each(lowers.begin(), lowers.end(), [](const dims_type &every_lower)
             {
        for_each(every_lower.begin(), every_lower.end(), [](const int& dims ){
            cout<<dims<<",";
        });
        cout<<endl; });
    cout << "measure: " << measure;
    cout << endl
         << "the sum of the upper bounds and lower bounds" << getSz() << endl;
    cout << "the value of the least layer level number of lower bound " << getLow_min_layer() << endl;
    cout << "covering capacity:" << getCa() << endl;
    cout << "---------bound line-----------" << endl;
}
