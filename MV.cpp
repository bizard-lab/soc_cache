#include "MV.hpp"
void MV::setCost(double p_cost)
{
	cost = p_cost;
}
void MV::setCount(int p_count)
{
	count = p_count;
}
void MV::setSqlstr(string p_sqlstr)
{
	sqlstr = p_sqlstr;
}
void MV::setDims(int p_dims)
{
	dims = p_dims;
}
void MV::setBenefit(int p_benefit)
{
	benefit = p_benefit;
}
void MV::setChr(vector<char> p_chr)
{
	chr = p_chr;
}
double MV::getCost()
{
	return cost;
}
int MV::getCount()
{
	return count;
}
int MV::getDims()
{
	return dims;
}
int MV::getBenefit()
{
	return benefit;
}
string MV::getSqlstr()
{
	return sqlstr;
}
vector<char> MV::getChr()
{
	return chr;
}
void MV::display()
{
	cout << "count:" << count << "cost:" << cost << endl;
	cout << "sql:" << sqlstr << endl;
	cout << "benefit:" << benefit << endl;
}
