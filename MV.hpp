#ifndef MV_hpp
#define MV_hpp
#include "all_header.h"
#include "database_operation.hpp"
class MV
{
private:
   string sqlstr;
   int count;
   double cost;
   int dims;
   int benefit;
   vector<char> chr;
public:
   void setCost(double p_cost);
   void setCount(int p_count);
   void setDims(int p_dims);
   void setBenefit(int p_benefit);
   void setSqlstr(string p_sqlstr);
   void setChr(vector<char> p_chr);
   void display();
   double getCost();
   int getCount();
   int getDims();
   int getBenefit();
   string getSqlstr();
   vector<char> getChr();
};
#endif
