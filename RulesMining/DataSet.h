#ifndef __DATASET_H
#define __DATASET_H


#include<vector>
using namespace std;

struct RULE{//规则数据结构
	vector<string> premise;
	vector<string> result;
	double support;
	double confidence;
	double lift;
};


class DataSet{//数据集类

public:
	vector<vector<string>> dataset;
	vector<RULE> strongRule;//生成的强规则

public:

	DataSet();
	void loadFile(char* filename); //加载商店文件
	void loadLinux(string filename);//加载unix用户操作文件
	void printData();//打印输入数据
	void printRule(string filename);//打印规则
};






#endif
