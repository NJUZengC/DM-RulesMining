#ifndef __FPGROWTH_H_
#define __FPGROWTH_H_

#include<algorithm>
#include"DataSet.h"
#include"map"


struct FPTreeNode{//FP树节点
	string name;
	int count;
	FPTreeNode *parent;
	vector<FPTreeNode *> children;
	FPTreeNode *nextBrother;
	
	FPTreeNode(){
		nextBrother = NULL;
		parent = NULL;
		count = 0;
	}
	FPTreeNode(string s)
	{
		nextBrother = NULL;
		parent = NULL;
		count = 0;
		name = s;
	}
	FPTreeNode *insertChild(string name)
	{
		for (unsigned int i = 0; i < children.size(); i++)
		{
			if (children[i]->name .compare(name)==0)
			{
				children[i]->count++;
				return children[i];
			}
		}
		return NULL;
	}
};

struct DataItem{//项头表
	string name;
	int count;
	FPTreeNode *head;
};

class FPGrowth : public DataSet{

	
	int minSupportCount;
	map<vector<string>,int> totalSet;//总频繁项集
	double support;
	double conf;
	double lift;

public:

	FPGrowth();
	FPGrowth(int minsupportcount, double sup, double confi, double lif);

	void DataMining();//数据挖掘
	void printFrequentSets(string filename);//输出频繁项集

private:
	void doGetFre();//生成频繁项集
	vector<DataItem> createItemList(const vector<vector<string>> &ds);//生成项头表
	FPTreeNode * createFPTree(const vector<vector<string>> &data, const vector<DataItem> &itemlist);//创建FP树返回头节点
	bool cmpItems(const string& s1, const string &s2, const vector<DataItem> &itemlist);//比较在项头表中s1是否先于s2出现
	void insert_tree(const string&p, vector<string> &P, FPTreeNode *T, const vector<DataItem> &itemlist);//事务插入FP树

	void getFrequentSets(vector<vector<string>> ds, vector<string> prePattern);//给定数据生成频繁项集
	
	void Combine(vector<int> arr, int start, vector<int> result, int count, const int NUM, vector<vector<int>> &combination);
	int getcount(vector<string> &emerge, map<vector<string>, int> &prevL);
	bool containRule(RULE &r);
};


#endif