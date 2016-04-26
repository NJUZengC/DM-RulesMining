#include"Apriori.h"
#include <ctime>
#include<iostream>
#include<string>
#include<fstream>

bool SortVector(const string& s1, const string& s2)
{
	return (s1.compare(s2) < 0);//升序排列  
}

Apriori::Apriori(){
	minsupCount = 200;
	support = 0;
	conf = 0;
	lift = 1;
};

Apriori::Apriori(int minsupportCount, double sup, double confi, double lif){
	minsupCount = minsupportCount;
	support = sup;
	conf = confi;
	lift = lif;
};

void Apriori::dataMining(){

	getFreItems();
	cout << endl;

	for (unsigned int i = 1; i < frequentSet.size(); i++)
	{
		map<vector<string>, int> item = frequentSet[i];
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

					int precount = findcount(pre, frequentSet[pre.size() - 1]);
					int thencount = findcount(then, frequentSet[then.size() - 1]);

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

								cout << "   SUP : " << sup << "  CNF : " << confidence << "  LIF : " << lifts<< endl;
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

void Apriori::printFrequentSets(string filename){
	ofstream output(filename);
	if (!output){
		cout << "输出文件未打开" << endl;
		return;
	}
	for (unsigned int j = 0; j < frequentSet.size(); j++){
		map<vector<string>, int>::iterator it1 = frequentSet[j].begin();
		for (; it1 != frequentSet[j].end(); it1++){
			for (unsigned int i = 0; i < it1->first.size(); i++)
				output << it1->first[i] << " ";

			output << it1->second << endl;
		}
	}
	output.close();
}

bool Apriori::containRule(RULE &r){
	for (unsigned int i = 0; i < strongRule.size(); i++)
	{
		vector<string> rulepre = strongRule[i].premise;
		vector<string> ruleres = strongRule[i].result;
		if ((rulepre.size() != r.premise.size()) || (ruleres.size() != r.result.size()))
			continue;
		sort(rulepre.begin(), rulepre.end());
		sort(r.premise.begin(), r.premise.end());
		
		bool flag = true;
		for (unsigned int j = 0; j < rulepre.size();j++)
			if (rulepre[j] != r.premise[j])
			{
				flag = false;
				break;
			}
		if (flag == false)
			continue;


		sort(ruleres.begin(), ruleres.end());
		sort(r.result.begin(), r.result.end());

		for (unsigned int j = 0; j < ruleres.size();j++)
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

int Apriori::findcount(vector<string> &emerge, map<vector<string>, int> &prevL){
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
}

bool Apriori::find( map<vector<string>, int> &mapmow,  vector<string> &s){
	for (map<vector<string>, int>::iterator it=mapmow.begin(); it != mapmow.end(); it++){
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

void Apriori::combine(vector<int> arr, int start, vector<int> result, int count, const int NUM, vector<vector<int>> &combination){
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


void Apriori::getFreItems(){
	bool finish = false;
	map<vector<string>, int> L = find_frequent_1_items();
	frequentSet.push_back(L);
	int index = 1;
	int sum = L.size();
	cout << "     " << index << "频繁项集 ：" << L.size() << "\tFROM\t" << totalItemSize << endl;



	while (frequentSet[frequentSet.size()-1].size()>0)
	{

		map<vector<string>, int> Ck = apriori_gen(frequentSet[frequentSet.size() - 1]);

		int lastNum = Ck.size();
		bool flag;
		for (unsigned int i = 0; i < dataset.size(); i++)
		{
			for (map<vector<string>, int>::iterator it1 = Ck.begin(); it1 != Ck.end(); it1++){
				flag = true;
				/*if (AffairContainsItem(dataset[i], it1->first))
				{
					it1->second = it1->second + 1;
				}*/
				for (unsigned int k = 0; k < it1->first.size(); k++) {
					if (std::find(dataset[i].begin(), dataset[i].end(), it1->first[k]) == dataset[i].end()) {
						flag = false;
						break;
					}
				}
				if (flag) {
					it1->second++;
				}
			}
		}


		for (map<vector<string>, int>::iterator it1 = Ck.begin(); it1 != Ck.end();){
			if (it1->second < minsupCount)
			{
				
				Ck.erase(it1++);
			}
			else
			{
				
				it1++;
			}

		}

		map<vector<string>, int> Lk(Ck);
		index++;
		cout << "     " << index << "频繁项集 ：" << Lk.size() << "\tFROM\t" << lastNum << endl;
		sum += Lk.size();
		frequentSet.push_back(Lk);



	}
	cout << endl << "     共计 " << sum << " 项频繁项集" << endl;
	
};

map<vector<string>, int> Apriori::apriori_gen(map<vector<string>, int> &prevL){
	map<vector<string>, int> nextC;
	
	for (map<vector<string>, int>::iterator it1 = prevL.begin(); it1 != prevL.end(); it1++)
		for (map<vector<string>, int>::iterator it2 = it1; it2 != prevL.end(); it2++)
		{
			if (it1 != it2){
				bool flag = false;
				for (unsigned int i = 0; i < it1->first.size()-1; i++)
				{
					if (it1->first[i].compare(it2->first[i]) != 0)
					{
						flag = true;
						break;
					}
				}
				if (!flag){
					if (it1->first[it1->first.size() - 1].compare(it2->first[it2->first.size() - 1]) >= 0)
					{
						continue;
					}
					vector<string> emerge(it1->first);
					emerge.erase(emerge.begin()+it1->first.size() - 1);
                    emerge.push_back(it1->first[it1->first.size() - 1]);
				    emerge.push_back(it2->first[it2->first.size() - 1]);
					
					if (!has_infrequent_subset(emerge, prevL)){//!has_infrequent_subset(emerge, prevL)
						
						bool flag = find(nextC, emerge);
						if (!flag)
						{
							nextC.insert(pair<vector<string>, int>(emerge, 0));			
						}
					}
				}
			}
		}

	return nextC;
};

bool Apriori::has_infrequent_subset(vector<string> &emerge, map<vector<string>, int> &prevL)
{
	for (unsigned int i = 0; i < emerge.size(); i++)
	{
		vector<string> temp(emerge);
		temp.erase(temp.begin() + i);
		/*map<vector<string>, int, ptrCmp>::iterator it = prevL.begin();
		it = prevL.find(temp);*/
		bool flag = find(prevL, temp);
		if (!flag)
			return true;
	}
	return false;
};


map<vector<string>, int> Apriori::find_frequent_1_items(){
	map<vector<string>, int> frequent;
	for (unsigned int i = 0; i < dataset.size(); i++)
	{
		
		
		for (unsigned int j = 0; j < dataset[i].size(); j++)
		{
			vector<string> now;
			now.push_back(dataset[i][j]);
			bool nowflag = find(frequent, now);
			if (!nowflag)
				frequent.insert(pair<vector<string>, int>(now, 1));
			else
				frequent[now] = frequent[now] + 1;
		}
		
	}
	totalItemSize = frequent.size();
	map<vector<string>, int>::iterator iti = frequent.begin();
	for (; iti != frequent.end(); )
	{
		if (iti->second < minsupCount)
		{
			frequent.erase(iti++);
		}
		else
		{
			iti++;
		}
	}
	return frequent;
};