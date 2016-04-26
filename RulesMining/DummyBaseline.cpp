#include<algorithm>
#include<iostream>
#include"DummyBaseline.h"
#include<string>
#include<fstream>

DummyBaseline::DummyBaseline(){
	SupportCount = 100;
	support = 0;
	conf = 0;
	lift = 1;
};

DummyBaseline::DummyBaseline(int count, double sup, double confi, double lif){
	SupportCount = count;
	support = sup;
	conf = confi;
	lift = lif;
}

void DummyBaseline::printFrequentSets(string filename){//输出频繁项集
	ofstream output(filename);
	if (!output){
		cout << "输出文件未打开" << endl;
		return;
	}
	for (unsigned int j = 0; j < freqSet.size(); j++){
		map<vector<string>, int>::iterator it1 = freqSet[j].begin();
		for (; it1 != freqSet[j].end(); it1++){
			for (unsigned int i = 0; i < it1->first.size(); i++)
				output << it1->first[i] << " ";

			output << it1->second << endl;
		}
	}
	output.close();
};

bool DummyBaseline::containRule(RULE &r){
	for (unsigned int i = 0; i < strongRule.size(); i++)
	{
		vector<string> rulepre = strongRule[i].premise;
		vector<string> ruleres = strongRule[i].result;
		if ((rulepre.size() != r.premise.size()) || (ruleres.size() != r.result.size()))
			continue;
		sort(rulepre.begin(), rulepre.end());
		sort(r.premise.begin(), r.premise.end());

		bool flag = true;
		for (unsigned int j = 0; j < rulepre.size(); j++)
			if (rulepre[j] != r.premise[j])
			{
				flag = false;
				break;
			}
		if (flag == false)
			continue;


		sort(ruleres.begin(), ruleres.end());
		sort(r.result.begin(), r.result.end());

		for (unsigned int j = 0; j < ruleres.size(); j++)
			if (ruleres[j] != r.result[j])
			{
				flag = false;
				break;
			}

		if (flag == false)
			continue;
		else return true;
	}
	return false;
};

bool DummyBaseline::Find(map<vector<string>, int> &mapmow, vector<string> &s){
	for (map<vector<string>, int>::iterator it = mapmow.begin(); it != mapmow.end(); it++){
		vector<string> m = it->first;
		unsigned int i = 0;
		for (; i < m.size(); i++)
		{
			if (m[i].compare(s[i]) != 0)
				break;
		}
		if (i == m.size())
			return true;
	}
	return false;
}

void DummyBaseline::findoneitems(){
	map<vector<string>, int> frequent;
	for (unsigned int i = 0; i < dataset.size(); i++)
	{


		for (unsigned int j = 0; j < dataset[i].size(); j++)
		{
			vector<string> now;
			now.push_back(dataset[i][j]);
			bool nowflag = Find(frequent, now);
			if (!nowflag)
				frequent.insert(pair<vector<string>, int>(now, 1));
			else
				frequent[now] = frequent[now] + 1;
		}

	}
	
	map<vector<string>, int>::iterator iti = frequent.begin();
	for (; iti != frequent.end();)
	{
		if (iti->second < SupportCount)
		{
			frequent.erase(iti++);
		}
		else
		{
			string str = iti->first[0];
			iti++;
		}
	}
	
	iti = frequent.begin();
	
	for (; iti != frequent.end(); iti++)
	{
		oneItem.push_back(iti->first[0]);
	}
	freqSet.push_back(frequent);

	
	
};

void DummyBaseline::dataMining(){

	getFreSets();
	cout << endl;
	for (unsigned int i = 1; i < freqSet.size(); i++)
	{
		map<vector<string>, int> item = freqSet[i];
		map<vector<string>, int>::iterator it = item.begin();
		for (; it != item.end(); it++)
		{
			vector<string> ds = it->first;
			int count = it->second;
			for (unsigned int j = 1; j < ds.size(); j++)
			{
				vector<int> arr;
				arr.resize(ds.size());
				vector<int> result;
				result.resize(ds.size());
				vector<vector<int>> comb;
				combine(arr, 0, result, j, j, comb);

				for (unsigned int k = 0; k < comb.size(); k++)
				{
					vector<string> pre;
					vector<string> then;
					for (unsigned int l = 0; l < comb[k].size(); l++){
						pre.push_back(ds[comb[k][l]]);
					}
					for (unsigned int m = 0; m < ds.size(); m++)
					{
						if (std::find(pre.begin(), pre.end(), ds[m]) == pre.end())
							then.push_back(ds[m]);
					}

					int precount = findcount(pre, freqSet[pre.size() - 1]);
					int thencount = findcount(then, freqSet[then.size() - 1]);

					double sup = double(count) / dataset.size();
					double confidence = double(count) / precount;
					double supb = double(thencount) / dataset.size();
					double lifts = confidence / supb;
					if (support <= sup&&conf <= confidence&&lift<=lifts)
					{
						RULE r;
						r.premise = pre;
						r.result = then;
						r.support = sup;
						r.confidence = confidence;
						r.lift = lifts;
						if (!containRule(r)){
							strongRule.push_back(r);

							if (strongRule.size() <= 10){
								cout << "     ";
								for (unsigned s = 0; s < pre.size(); s++)
									cout << pre[s] << "  ";
								cout << " ----->  ";

								for (unsigned s = 0; s < then.size(); s++)
									cout << then[s] << "  ";

								cout << "   SUP : " << sup << "  CNF : " << confidence << "  LIF : " << lifts << endl;
							}
						}
					}

				}

			}
		}
	}
	
	cout << endl << "     " << "共挖掘出强关联规则 ：" << strongRule.size() << " 条" << endl;
	cout << "     " << "由于篇幅缘故只显示前 10 条 " << endl;
};


void DummyBaseline::combine(vector<int> arr, int start, vector<int> result, int count, const int NUM, vector<vector<int>> &combination){
	unsigned int i = 0;
	for (i = start; i < arr.size() + 1 - count; i++)
	{
		result[count - 1] = i;
		if (count - 1 == 0)
		{
			int j;
			vector<int> s;
			for (j = NUM - 1; j >= 0; j--){
				s.push_back(result[j]);
			}

			combination.push_back(s);
		}
		else
			combine(arr, i + 1, result, count - 1, NUM, combination);
	}
};

int DummyBaseline::findcount(vector<string> &emerge, map<vector<string>, int> &prevL){
	for (map<vector<string>, int>::iterator it = prevL.begin(); it != prevL.end(); it++){
		vector<string> m = it->first;
		sort(m.begin(), m.end());
		sort(emerge.begin(), emerge.end());
		unsigned int i = 0;
		for (; i < m.size(); i++)
		{
			if (m[i].compare(emerge[i]) != 0)
				break;
		}
		if (i == m.size())
			return it->second;
	}
	return 0;

};


void DummyBaseline::getFreSets(){
	findoneitems();
	if (freqSet.size() == 0)
	{
		cout << "     " << 1 << "频繁项集 ：" << 0 << endl;
		cout << endl << "     共计 " << 0 << " 项频繁项集" << endl;
		return;
	}
	int sum = 0;
	while (freqSet[freqSet.size() - 1].size() > 0)
	{
		int layer = freqSet.size()+1;
		cout << "     " << layer - 1 << "频繁项集 ：" << freqSet[freqSet.size() - 1].size() << endl;
		sum += freqSet[freqSet.size() - 1].size();
		vector<vector<int>> comb;

		int count = freqSet[0].size();
		vector<int> array;
		for (int i = 0; i < count; i++)
			array.push_back(i);
		vector<int> res;
		res.resize(count);

		combine(array, 0, res, layer, layer, comb);
		map<vector<string>, int> layerpair;
		for (unsigned int i = 0; i < comb.size(); i++){
			vector<string> data;
			for (unsigned int j = 0; j < comb[i].size(); j++){
				data.push_back(oneItem[comb[i][j]]);
			}

			sort(data.begin(), data.end());
			int sum = 0;

			for (unsigned int m = 0; m < dataset.size(); m++)
			{
				if (dataset[m].size() < data.size())
					continue;
				bool flag = true;
				for (unsigned int n = 0; n < data.size(); n++){
					unsigned int p = 0;
					for (; p < dataset[m].size(); p++){
						if (dataset[m][p] == data[n])
							break;
					}
					if (p == dataset[m].size())
					{
						flag = false;
						break;
					}
				}
				if (flag == true)
					sum++;
			}
			if (sum >= SupportCount)
				layerpair.insert(make_pair(data, sum));
		}
		freqSet.push_back(layerpair);
	}

	cout << endl << "     共计 " << sum << " 项频繁项集" << endl;

};

