#include "queryItem.hpp"
queryItem::queryItem(dims_type _dims, measure_type _measure) : dims(_dims), measure(_measure)
{
    layer = dimsNum - count(dims.begin(), dims.end(), 0);
    LFU_count = 0;
}
void queryItem::display() const
{
    cout << "dim value:";
    for_each(dims.begin(), dims.end(), [](const int &num)
             { cout << num << ","; });
    cout << "meaure:" << measure << "layer number:" << layer << endl;
}
string queryItem::get_Dims()
{
    string dimstr;
    array<int, dimsNum>::iterator v;
    for (v = dims.begin(); v != dims.end(); ++v)
    {
        dimstr = dimstr + to_string(*v);
    }
    return dimstr;
}
void queryItem::add_LFU_count()
{
    ++LFU_count;
}
int queryItem::get_LFU_count() const
{
    return LFU_count;
}
