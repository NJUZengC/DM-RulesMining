#ifndef __DATASET_H
#define __DATASET_H


#include<vector>
using namespace std;

struct RULE{//�������ݽṹ
	vector<string> premise;
	vector<string> result;
	double support;
	double confidence;
	double lift;
};


class DataSet{//���ݼ���

public:
	vector<vector<string>> dataset;
	vector<RULE> strongRule;//���ɵ�ǿ����

public:

	DataSet();
	void loadFile(char* filename); //�����̵��ļ�
	void loadLinux(string filename);//����unix�û������ļ�
	void printData();//��ӡ��������
	void printRule(string filename);//��ӡ����
};






#endif
