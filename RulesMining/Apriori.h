#ifndef __APRIORI_H_
#define __APRIORI_H_

#include"DataSet.h"
#include<map>
#include<algorithm>
using std::string;

extern bool SortVector(const string& s1, const string& s2);//��vector<string>������

class Apriori : public DataSet {

	int minsupCount;//��С֧�ֶȼ���
	int totalItemSize;//�����ܵ�����
	double support;//��С֧�ֶ�
	double conf;//��С���Ŷ�
	double lift;//��С������
    vector<map<vector<string>, int>> frequentSet;//Ƶ���

public:

	Apriori();
	Apriori(int minsupportCount,double sup,double confi,double lif);
	void dataMining();//���������ھ�
	void printFrequentSets(string filename);

private:

	void getFreItems();//����Ƶ���
	map<vector<string>, int> apriori_gen(map<vector<string>, int> &prevL);//��k-1����ɱ�ѡk�
	map<vector<string>, int> find_frequent_1_items();//��ȡƵ��1�
	bool has_infrequent_subset(vector<string> &emerge, map<vector<string>, int> &prevL);//��ʾ��emerge k����Ƿ������Ƶ��k-1���prevL��ʾ����k-1�
	bool find(map<vector<string>, int> &mapmow, vector<string> &s);//�ж�mapmow���Ƿ������ֵΪs����

	//arrΪԭʼ����//startΪ������ʼλ��//result��������Ϊһά����//countΪresult���������ֵ����������
	//NUMΪҪѡȡ��Ԫ�ظ���//arr_lenΪԭʼ����ĳ��ȣ�Ϊ��ֵ//�������б�����combination��
	void combine(vector<int> arr, int start, vector<int> result, int count, const int NUM, vector<vector<int>> &combination);//��arrԪ����ȡNUM��ֵ���
	
	int findcount(vector<string> &emerge, map<vector<string>, int> &prevL);//��ȡemerge��ֵ��Ӧ����Ŀ
	bool containRule(RULE &r);//�ж�����ǿ�������Ƿ��������r

};

#endif