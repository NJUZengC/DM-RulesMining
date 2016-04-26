#ifndef __DUMMYBASELINE_H_
#define __DUMMYBASELINE_H_

#include<map>
#include"DataSet.h"


class DummyBaseline :public DataSet{

	vector <map<vector<string>,int>> freqSet;//��Ƶ���
	vector<string> oneItem;//����Ƶ���
	int SupportCount;
	vector<vector<int>> combination;
	double support;
	double conf;
	double lift;

public:

	DummyBaseline();
	DummyBaseline(int count, double sup, double confi, double lif);
	void dataMining();
	void printFrequentSets(string filename);//���Ƶ���

private:

	void findoneitems();//����1�
	bool Find(map<vector<string>, int> &mapmow, vector<string> &s);
	void getFreSets();
	void combine(vector<int> arr, int start, vector<int> result, int count, const int NUM, vector<vector<int>> &combination);
	int findcount(vector<string> &emerge, map<vector<string>, int> &prevL);
	bool containRule(RULE &r);
};


#endif