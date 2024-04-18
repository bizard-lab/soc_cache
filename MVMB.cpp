#include "MVMB.hpp"
void mvmb::setMV(vector<MV> p_mv)
{
	mv = p_mv;
}
vector<MV> mvmb::getMV()
{
	return mv;
}
void mvmb::powSet(int i, vector<char> chr, vector<string> &tempstr_vector)
{
	int j = 0;
	bool pre = false;
	string tempstr = "";
	while (i > 0)
	{
		if (i & 1)
		{
			tempstr += chr[j];
			pre = true;
		}
		i >>= 1;
		j++;
	}
	tempstr_vector.push_back(tempstr);
}
vector<int> mvmb::sort_MV(vector<MV> p_mv)
{
	static const int k = 30; // MV_NUMBER;
	int max_benefit = 0;
	int tag = 0;
	int tag_temp = 0;
	int len = 0;
	int tags[k];
	int tags_falg = 1;
	int total = 1;
	for (auto a : p_mv)
	{
		a.setBenefit((p_mv[0].getCost() - a.getCost()) * pow(2, a.getDims()));
		if (a.getBenefit() > max_benefit)
		{
			max_benefit = a.getBenefit();
			tag = tag_temp;
			p_mv[tag_temp].setBenefit(max_benefit);
		}
		tag_temp++;
	}
	vector<string> tempstr_vector;
	char temp_chr_arr[p_mv[tag].getDims()];
	for (int i = 0; i < p_mv[tag].getDims(); ++i)
	{
		temp_chr_arr[i] = p_mv[tag].getChr()[i];
	}
	len = 1 << sizeof(temp_chr_arr) / sizeof(char);
	for (int i = 0; i < len; i++)
	{
		powSet(i, p_mv[tag].getChr(), tempstr_vector);
	}
	for (int i = 0; i < tempstr_vector.size(); i++)
	{
		mv_map.insert(map<string, int>::value_type(tempstr_vector[i], p_mv[tag].getCost()));
	}
	tags[0] = tag;
	tag_temp = 0;
	for (int i = 1; i < k; ++i)
	{
		max_benefit = 0;
		int max_cost = 0;
		vector<string> temp_vector_for_map;
		tag_temp = 0;
		for (auto a : p_mv)
		{
			char temp_chr[a.getDims()];
			int sum = 0;
			int temp_len = 0;
			vector<string> temp_vector;
			for (int j = 0; j < a.getDims(); ++j)
			{
				temp_chr[j] = a.getChr()[j];
			}
			temp_len = 1 << sizeof(temp_chr) / sizeof(char);
			for (int m = 0; m < temp_len; m++)
			{
				powSet(m, a.getChr(), temp_vector);
			}
			for (int n = 0; n < temp_vector.size(); n++)
			{
				if (mv_map.count(temp_vector[n]) == 0)
				{
					sum++;
				}
			}
			map<string, int>::iterator iter;
			iter = mv_map.find(temp_vector[temp_vector.size() - 1]);
			if (iter != mv_map.end())
			{

				a.setBenefit((iter->second - a.getCost()) * pow(2, sum));
			}
			else
			{
				a.setBenefit((p_mv[0].getCost() - a.getCost()) * pow(2, sum));
				p_mv[tag_temp].setBenefit((p_mv[0].getCost() - a.getCost()) * pow(2, sum));
			}
			if (i >= 29)
			{
				if (a.getBenefit() > max_benefit)
				{
				}
			}
			if (a.getBenefit() > max_benefit)
			{
				max_benefit = a.getBenefit();
				tags[tags_falg] = tag_temp;
				temp_vector_for_map = temp_vector;
				max_cost = a.getCost();
				p_mv[tag_temp].setBenefit(max_benefit);
			}
			tag_temp++;
		}

		for (int index = 0; index < temp_vector_for_map.size(); index++)
		{
			if (mv_map.count(temp_vector_for_map[index]) == 0)
			{
				mv_map.insert(map<string, int>::value_type(temp_vector_for_map[index], max_cost));
			}
		}
		tags_falg++;
	}
	cout << tags_falg << endl;
	cout << "------------" << endl;
	vector<int> res;
	for (int i = 0; i < tags_falg; ++i)
	{
		p_mv[tags[i]].display();
		res.push_back(tags[i]);
	}
	return res;
}

vector<int> mvmb::sort_MV2(vector<MV> p_mv)
{
	int k = 2;
	int max_benefit = 0;
	int tag = 0;
	int tag_temp = 0;
	int len = 0;
	int tags[2];
	int tags_falg = 1;
	int total = 1;
	map<int, int> chosed_map;
	for (auto a : p_mv)
	{
		a.setBenefit((p_mv[0].getCost() - a.getCost()) * pow(2, a.getDims()));
		if (a.getBenefit() > max_benefit)
		{
			max_benefit = a.getBenefit();
			tag = tag_temp;
			p_mv[tag_temp].setBenefit(max_benefit);
		}
		tag_temp++;
	}
	vector<string> tempstr_vector;
	vector<char> temp_chr_arr(p_mv[tag].getDims());
	for (int i = 0; i < p_mv[tag].getDims(); ++i)
	{
		temp_chr_arr[i] = p_mv[tag].getChr()[i];
	}
	len = 1 << sizeof(temp_chr_arr) / sizeof(char);
	for (int i = 0; i < len; i++)
	{
		powSet(i, p_mv[tag].getChr(), tempstr_vector);
	}
	for (int i = 0; i < tempstr_vector.size(); i++)
	{
		mv_map.insert(map<string, int>::value_type(tempstr_vector[i], p_mv[tag].getCost()));
	}
	chosed_map.insert(map<int, int>::value_type(tag, 1));
	tags[0] = tag;
	for (int i = 1; i < k; ++i)
	{
		max_benefit = 0;
		int max_cost = 0;
		vector<string> temp_powerset_vector_for_map;
		tag_temp = 0;
		for (int w = 0; w < p_mv.size(); ++w)
		{
			if (chosed_map.count(w) != 0)
			{
				continue;
			}
			vector<char> temp_chr(p_mv[w].getDims());
			int sum = 0;
			int temp_len = 0;
			vector<string> temp_powset_vector;
			for (int j = 0; j < p_mv[w].getDims(); ++j)
			{
				temp_chr[j] = p_mv[w].getChr()[j];
			}
			temp_len = 1 << sizeof(temp_chr) / sizeof(char);
			for (int m = 0; m < temp_len; m++)
			{
				powSet(m, p_mv[w].getChr(), temp_powset_vector);
			}

			for (int n = 0; n < temp_powset_vector.size(); n++)
			{

				if (mv_map.count(temp_powset_vector[n]) == 0)
				{
					sum++;
				}
			}
			map<string, int>::iterator iter;
			iter = mv_map.find(temp_powset_vector[temp_powset_vector.size() - 1]);
			if (iter != mv_map.end())
			{
				p_mv[w].setBenefit((iter->second - p_mv[w].getCost()) * pow(2, sum));
			}
			else
			{
				p_mv[w].setBenefit((p_mv[0].getCost() - p_mv[w].getCost()) * pow(2, sum));
			}
			if (p_mv[w].getBenefit() > max_benefit)
			{
				max_benefit = p_mv[w].getBenefit();
				tags[tags_falg] = w;
				tag_temp = w;
				temp_powerset_vector_for_map = temp_powset_vector;
				max_cost = p_mv[w].getCost();
				p_mv[w].setBenefit(max_benefit);
			}
		}

		for (int index = 0; index < temp_powerset_vector_for_map.size(); index++)
		{
			if (mv_map.count(temp_powerset_vector_for_map[index]) == 0)
			{
				mv_map.insert(map<string, int>::value_type(temp_powerset_vector_for_map[index], max_cost));
			}
		}
		chosed_map.insert(map<int, int>::value_type(tag_temp, 1));
		tags_falg++;
	}
	cout << tags_falg << endl;
	cout << "------------" << endl;
	vector<int> res;
	for (int i = 0; i < tags_falg; ++i)
	{
		p_mv[tags[i]].display();
		res.push_back(tags[i]);
	}
	return res;
}
equivalence_class_num_type mvmb::get_the_number_of_equivalence_classes() const
{
	cout << "cache_manager_MVMB::get_the_number_of_equivalence_classes()const not need to accomplish,if error,quit." << endl;
	exit(0);
	return 0;
}

data_cell_num_type mvmb::get_the_number_of_data_units() const
{
	return MVMB_Container.size();
}

bool mvmb::query_in_cache(queryItem &queryItem)
{
	bool flag = false;
	if (MVMB_Container.empty())
	{
		return false;
	}
	for (auto iter = MVMB_Container.begin(); iter != MVMB_Container.end(); ++iter)
	{
		if (iter->getDims() == queryItem.getDims())
		{
			queryItem.setMeasure(iter->getMeasure());
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

void mvmb::refresh_cache(const queryItem &queryItem_1)
{
	queryItem temp_queryItem = queryItem_1;
	string str = temp_queryItem.get_Dims();
	if (MVMB_map.count(str) == 0)
	{
		MVMB_Container.push_front(queryItem_1);
		MVMB_map.insert(map<string, int>::value_type(str, 1));
	}
	else
	{
	}
	while (get_the_number_of_data_units() > data_cells_limit_size)
	{
		cache_replacement_policy();
	}
}

void mvmb::cache_replacement_policy()
{
	MVMB_Container.pop_back();
}

vector<queryItem> mvmb::load_query_file()
{
	vector<vector<string>> temp_queryItemSet;
	ifstream infile(querySetFilePath);
	string temp;
	if (!infile.is_open())
	{
		cout << querySetFilePath + "the file not is open on path" << endl;
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

vector<queryItem> mvmb::loadQueryFile()
{
	vector<vector<string>> temp_queryItemSet;
	cout << "..." << endl;
	ifstream infile(mvQueryFilePath);
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

void mvmb::match_best_MV()
{
}

void mvmb::displasy_cachaItem() const
{
	cout << endl
		 << endl
		 << "the number of the data cells" << get_the_number_of_data_units() << endl;
	for_each(MVMB_Container.begin(), MVMB_Container.end(), [](const queryItem &every_queryItem)
			 { every_queryItem.display(); });
}

vector<int> mvmb::load_MV()
{
	ifstream alg(basePath + "static_mv.txt");
	string line;
	vector<int> mv;
	if (!alg.is_open())
	{
		cout << "fail to read mv" << endl;
		exit(0);
	}
	while (getline(alg, line))
	{
		mv.push_back(atoi(line.c_str()));
	}
	return mv;
}

void mvmb::write_mv_to_file(vector<int> mv_flag)
{
	ofstream alg;
	alg.open(basePath + "static_mv.txt", ios::trunc);
	if (!alg.is_open())
	{
		cout << "fail to write mv into file" << endl;
		exit(0);
	}
	for (auto &mv : mv_flag)
	{
		alg << mv << endl;
	}
}

void mvmb::dispaly_experimental_result_to_file(data_cell_num_type queryItemSet_size, int hit, time_t all_time) const
{
	ofstream alg;
	alg.open(basePath + "MVMBalg.txt", ios::trunc);
	if (!alg.is_open())
	{
		cout << "fail to write results into files" << endl;
		exit(0);
	}
	alg << "alg name:MVMB" << endl;
	alg << "the number of query items:" << queryItemSet_size << endl;
	alg << "cache limit size(the number of data cells):" << data_cells_limit_size << endl;
	alg << "hit times:" << hit << endl;
	alg << "hit rate:" << (float)hit / queryItemSet_size << endl;
	alg << "total use time(s):" << all_time << endl;
	alg << "the number of data cells:" << get_the_number_of_data_units() << endl;
}
