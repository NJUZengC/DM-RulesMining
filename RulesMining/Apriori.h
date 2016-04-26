#ifndef __APRIORI_H_
#define __APRIORI_H_

#include"DataSet.h"
#include<map>
#include<algorithm>
using std::string;

extern bool SortVector(const string& s1, const string& s2);//对vector<string>排序函数

class Apriori : public DataSet {

	int minsupCount;//最小支持度计数
	int totalItemSize;//数据总单项数
	double support;//最小支持度
	double conf;//最小置信度
	double lift;//最小提升度
    vector<map<vector<string>, int>> frequentSet;//频繁项集

public:

	Apriori();
	Apriori(int minsupportCount,double sup,double confi,double lif);
	void dataMining();//进行数据挖掘
	void printFrequentSets(string filename);

private:

	void getFreItems();//生成频繁项集
	map<vector<string>, int> apriori_gen(map<vector<string>, int> &prevL);//由k-1项集生成备选k项集
	map<vector<string>, int> find_frequent_1_items();//获取频繁1项集
	bool has_infrequent_subset(vector<string> &emerge, map<vector<string>, int> &prevL);//表示在emerge k项集中是否包含不频繁k-1项集，prevL表示所有k-1项集
	bool find(map<vector<string>, int> &mapmow, vector<string> &s);//判断mapmow中是否包含键值为s的项

	//arr为原始数组//start为遍历起始位置//result保存结果，为一维数组//count为result数组的索引值，起辅助作用
	//NUM为要选取的元素个数//arr_len为原始数组的长度，为定值//所有序列保存在combination中
	void combine(vector<int> arr, int start, vector<int> result, int count, const int NUM, vector<vector<int>> &combination);//求arr元素中取NUM个值组合
	
	int findcount(vector<string> &emerge, map<vector<string>, int> &prevL);//获取emerge键值对应的数目
	bool containRule(RULE &r);//判断已有强规则中是否包含规则r

};

#endif