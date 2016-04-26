
#include<iostream>
#include<fstream>
#include"FPGrowth.h"
#include<string>

FPGrowth::FPGrowth(){
	minSupportCount = 200;
	support = 0;
	conf = 0;
	lift = 1;
};

FPGrowth::FPGrowth(int minsupportcount, double sup, double confi, double lif){
	minSupportCount = minsupportcount;
	support = sup;
	conf = confi;
	lift = lif;
};

void FPGrowth::printFrequentSets(string filename){
	ofstream outputq(filename);
	if (!outputq){
		cout << "输出文件未打开" << endl;
		return ;
	}
	map<vector<string>, int>::iterator it = totalSet.begin();
	for (; it != totalSet.end(); it++)
	{
		for (unsigned int i = 0; i < it->first.size(); i++)
			outputq << it->first[i] << "  ";
		outputq << it->second << endl;
	}
	outputq.close();
}

bool sortItemList(const DataItem& di1, const DataItem& di2){
	return di1.count > di2.count;
};

bool FPGrowth::containRule(RULE &r){
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

bool FPGrowth::cmpItems(const string& s1, const string &s2, const vector<DataItem> &itemlist){
	if (s1.compare(s2) == 0)
		return false;
	for (unsigned int i = 0; i < itemlist.size(); i++)
	{
		if (itemlist[i].name.compare(s1) == 0)
			return true;
		if (itemlist[i].name.compare(s2) == 0)
			return false;
	}
	return false;
};


void FPGrowth::DataMining(){
	doGetFre();
	cout << endl;
	    map<vector<string>, int> item = totalSet;
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
				Combine(arr, 0, result, j, j, comb);

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

					int precount = getcount(pre, totalSet);
					int thencount = getcount(then, totalSet);

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

								cout << "   SUP : " << sup << "  CNF : " << confidence << "  LIF : "<<lifts << endl;
							}
						}
					}

				}

			}
		}
		
		cout << endl << "     " << "共挖掘出强关联规则 ：" << strongRule.size() << " 条" << endl;
		cout << "     " << "由于篇幅缘故只显示前 10 条 " << endl;
};

void FPGrowth::Combine(vector<int> arr, int start, vector<int> result, int count, const int NUM, vector<vector<int>> &combination){
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
			Combine(arr, i + 1, result, count - 1, NUM, combination);
	}
};

int FPGrowth::getcount(vector<string> &emerge, map<vector<string>, int> &prevL){
	for (map<vector<string>, int>::iterator it = prevL.begin(); it != prevL.end(); it++){
		vector<string> m = it->first;
		if (m.size() != emerge.size())
			continue;
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

void FPGrowth::doGetFre(){
	vector<DataItem> itemarray = createItemList(dataset);
	
	for (unsigned int i = 0; i < itemarray.size(); i++)
	{
		vector<string> t;
		t.push_back(itemarray[i].name);

		totalSet.insert(make_pair(t, itemarray[i].count));
	}
	
	
	vector<string> temp;
	getFrequentSets(dataset, temp);

	cout << endl << "     共计 " << totalSet.size() << " 项频繁项集" << endl;
}

void FPGrowth::getFrequentSets(vector<vector<string>> ds, vector<string> prePattern){
	vector<DataItem> itemarray = createItemList(ds) ;//生成项头表
	if (itemarray.size() == 0)
		return;
	FPTreeNode *root =createFPTree(ds,itemarray);//形成FP树
	if (root->children.size() == 0)
		return;

	if (prePattern.size() != 0){//由前缀模式与项头表组合生成频繁模式
		for (unsigned int i = 0; i < itemarray.size(); i++){
			vector<string> s;
			s.push_back(itemarray[i].name);
			for (unsigned int j = 0; j < prePattern.size(); j++)
			{
				s.push_back(prePattern[j]);
			}
			sort(s.begin(), s.end());
			totalSet.insert(make_pair(s, itemarray[i].count));
		}
	}
	for (int i = itemarray.size() - 1; i >= 0; i--)//从项头表末尾开始挖掘
	{
		vector<string> newPattern;//形成新的模式
		newPattern.push_back(itemarray[i].name);
		for (unsigned int m = 0; m < prePattern.size(); m++)
			newPattern.push_back(prePattern[m]);
		vector<vector<string>> newds;//新建新数据集
		FPTreeNode *head = itemarray[i].head->nextBrother;//遍历同名兄弟
		while (head != NULL){
			vector<string> affair;
			int count = head->count;//事务计数为当前节点计数
			FPTreeNode *up = head->parent;
			while (up != root)//从节点向上获取整条事务链上的项
			{
				affair.push_back(up->name);
				up = up->parent;
			}
			while (count > 0)//按照计数个数将事务加入新的数据集
			{
				newds.push_back(affair);
				count--;
			}
			head = head->nextBrother;
		}
		getFrequentSets(newds, newPattern);//对新的数据集生成频繁项集
	}

};

vector<DataItem> FPGrowth::createItemList(const vector<vector<string>> &ds){
	vector<DataItem> dis;
	for (unsigned int i = 0; i < ds.size(); i++)
	{
		for (unsigned int j = 0; j < ds[i].size(); j++)
		{
			bool flag = true;
			for (unsigned int k = 0; k < dis.size(); k++)
			{
				if (dis[k].name.compare(ds[i][j]) == 0)
				{
					dis[k].count++;
					flag = false;
					break;
				}
			}
			if (flag)
			{
				DataItem di;
				di.count = 1;
				di.name=(ds[i][j]);
				di.head = new FPTreeNode;
				di.head->nextBrother = NULL;
				dis.push_back(di);
			}
		}
	}
	sort(dis.begin(),dis.end(),sortItemList);
	for (vector<DataItem>::iterator it = dis.begin(); it != dis.end();)
	{
		if (it->count < minSupportCount){
			it = dis.erase(it);
		}
		else{
			it++;
		}
	}
	return dis;
};


void FPGrowth::insert_tree(const string&p, vector<string> &P, FPTreeNode *T, const vector<DataItem> &itemlist){
	FPTreeNode *child = T->insertChild(p);
	FPTreeNode *newchild=new FPTreeNode(p);
	bool flag = true;
	if (child== NULL)
	{
		flag = false;
		newchild->count = 1;
		newchild->parent = T;
		T->children.push_back(newchild);
		for (unsigned int i = 0; i < itemlist.size(); i++)
		{
			if (itemlist[i].name.compare(p) == 0)
			{
				FPTreeNode  *brother = itemlist[i].head;
				while (brother->nextBrother != NULL)
					brother = brother->nextBrother;
				brother->nextBrother = newchild;
				break;
			}
		}
	}
	if (P.size() > 0)
	{
		string s = P[0];
		vector<string> temp(P);
		temp.erase(temp.begin());
		if (flag)
		insert_tree(s, temp, child,itemlist);
		else
			insert_tree(s, temp, newchild,itemlist);
	}
}

FPTreeNode * FPGrowth::createFPTree(const vector<vector<string>> &data, const vector<DataItem> &itemlist){
	FPTreeNode *root = new FPTreeNode();
	for (unsigned int i = 0; i < data.size(); i++)
	{
		vector<string> datas(data[i]);
		for (unsigned j = 0; j < datas.size(); j++)
			for (unsigned k = j; k < datas.size();k++){
				if (!cmpItems(datas[j], datas[k],itemlist))
				{
					string temp = datas[j];
					datas[j] = datas[k];
					datas[k] = temp;
				}
			}
		if (datas.size() != 0){
			
			string s = datas[0];
			datas.erase(datas.begin());
			insert_tree(s, datas, root, itemlist);
		}
		
		
	}
	
	return root;
};