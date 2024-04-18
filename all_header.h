#ifndef all_header_h
#define all_header_h
#define WIN32
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <array>
#include <vector>
#include <list>
#include <string>
#include <set>
#include <sstream>
#include <map>
#include <string.h>
#include <algorithm>
#include <cmath>
#include "sqlite3.h"
#include <time.h>
#include <algorithm>
#include <functional>
#include <cstdlib>
#include "support_fun.hpp"
using namespace boost::multi_index;
using namespace std;
const string soc_alg_name = "SOC_MinCa";
const int dimsNum = 5;
const int totalNum = 6;
const int MV_NUMBER = 10;
const unsigned int data_cells_limit_size = 8000;
const string DB_USERNAME = "root";
const string DB_PASSWORD = "123456";
const string DB_DBNAME = "TPCH.db";
const string DB_TABLENAME = "TPCH";
const string basePath = "/root/soc/";
const string querySetFilePath = basePath + "queryset.txt";
const string mvQueryFilePath = basePath + "mv_6dims.txt";
typedef array<int, dimsNum> dims_type;
typedef set<dims_type> bound_type;
typedef double measure_type;
typedef set<dims_type>::size_type data_cell_num_type;
typedef unsigned long equivalence_class_num_type;
typedef unsigned int ca_type;
typedef unsigned short layer_type;

#endif /* all_header_h */
