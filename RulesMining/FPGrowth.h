#ifndef __FPGROWTH_H_
#define __FPGROWTH_H_

#include<algorithm>
#include"DataSet.h"
#include"map"


struct FPTreeNode{//FP���ڵ�
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

struct DataItem{//��ͷ��
	string name;
	int count;
	FPTreeNode *head;
};

class FPGrowth : public DataSet{

	
	int minSupportCount;
	map<vector<string>,int> totalSet;//��Ƶ���
	double support;
	double conf;
	double lift;

public:

	FPGrowth();
	FPGrowth(int minsupportcount, double sup, double confi, double lif);

	void DataMining();//�����ھ�
	void printFrequentSets(string filename);//���Ƶ���

private:
	void doGetFre();//����Ƶ���
	vector<DataItem> createItemList(const vector<vector<string>> &ds);//������ͷ��
	FPTreeNode * createFPTree(const vector<vector<string>> &data, const vector<DataItem> &itemlist);//����FP������ͷ�ڵ�
	bool cmpItems(const string& s1, const string &s2, const vector<DataItem> &itemlist);//�Ƚ�����ͷ����s1�Ƿ�����s2����
	void insert_tree(const string&p, vector<string> &P, FPTreeNode *T, const vector<DataItem> &itemlist);//�������FP��

	void getFrequentSets(vector<vector<string>> ds, vector<string> prePattern);//������������Ƶ���
	
	void Combine(vector<int> arr, int start, vector<int> result, int count, const int NUM, vector<vector<int>> &combination);
	int getcount(vector<string> &emerge, map<vector<string>, int> &prevL);
	bool containRule(RULE &r);
};


#endif