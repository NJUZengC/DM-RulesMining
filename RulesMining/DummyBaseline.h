#ifndef __DUMMYBASELINE_H_
#define __DUMMYBASELINE_H_

#include<map>
#include"DataSet.h"


class DummyBaseline :public DataSet{

	vector <map<vector<string>,int>> freqSet;//总频繁项集
	vector<string> oneItem;//单项频繁项集
	int SupportCount;
	vector<vector<int>> combination;
	double support;
	double conf;
	double lift;

public:

	DummyBaseline();
	DummyBaseline(int count, double sup, double confi, double lif);
	void dataMining();
	void printFrequentSets(string filename);//输出频繁项集

private:

	void findoneitems();//生成1项集
	bool Find(map<vector<string>, int> &mapmow, vector<string> &s);
	void getFreSets();
	void combine(vector<int> arr, int start, vector<int> result, int count, const int NUM, vector<vector<int>> &combination);
	int findcount(vector<string> &emerge, map<vector<string>, int> &prevL);
	bool containRule(RULE &r);
};


#endif