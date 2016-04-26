#include"DataSet.h"
#include<fstream>
#include<iostream>
#include<algorithm>
#include<string>
#include<math.h>

bool compare(const RULE &a, const RULE &b)
{
	
	unsigned int low = min(a.premise.size(), b.premise.size());
	for (unsigned int i = 0; i < low; i++)
	{
		if (a.premise[i] < b.premise[i])
			return true;
		else if (a.premise[i] > b.premise[i])
			return false;
	}
	if (a.premise.size() < b.premise.size())
		return true;
	else if (a.premise.size() > b.premise.size())
		return false;

	unsigned int lowint = min(a.result.size(), b.result.size());
	for (unsigned int i = 0; i < lowint; i++)
	{
		if (a.result[i] < b.result[i])
			return true;
		else if (a.result[i] > b.result[i])
			return false;
	}

	if (a.result.size() < b.result.size())
		return true;
	else if (a.result.size() > b.result.size())
		return false;

	return false;
}

DataSet::DataSet(){};

void DataSet::loadLinux(string filename){
	ifstream in(filename);
	if (!in){
		cout << "文件不存在，请验证后再进行操作！" << endl;
		return;
	}
	const string start = "**SOF**";
	const string end = "**EOF**";
	string data ;
	in >> data;

		while (data.compare(start)==0 && !in.eof())
		{
			vector<string> v;
			string endsig ;
			in >> endsig;
			while (endsig.compare(end)!=0)
			{
				if ((endsig.size() == 3) && (endsig[0] == '<')&&(endsig[2] == '>'));
				else if (find(v.begin(), v.end(), endsig) != v.end());
				else {
					v.push_back(endsig);
				}
				in >> endsig;
			}
			dataset.push_back(v);
			in >> data;
		}
		
	in.close();
	

}

void DataSet::loadFile(char* filename){
	FILE *input;
	fopen_s(&input, filename, "r");
	if (!input){
		cout << "文件不存在，请验证后再进行操作！" << endl;
		return;
	}
	char s='0';
	int im = 1;
	while (!feof(input))
	{
		while (s != '{'){
			s = fgetc(input);
			if (feof(input))
			{
				fclose(input);
				return;
			}
		}
		char *temp = new char[400];
		int index = 0;
		while (s != '}')
		{
			s = getc(input);
			temp[index++] = s;
		}
		temp[index-1] = '\0';
		im++;
		vector<string> line;
		char *delim = ",";
		char *next;
		line.push_back(strtok(temp, delim));
		while ((next = strtok(NULL, delim))){
			line.push_back(next);
		}
		dataset.push_back(line);

	}
	fclose(input);

};

void DataSet::printData(){
	for (unsigned int i = 0; i < dataset.size(); i++)
	{
		for (unsigned int j = 0; j < dataset[i].size(); j++)
		{
			string s = dataset[i][j];
			cout << s.c_str() << " ";
		}
		cout << endl;
	}

};

void DataSet::printRule(string filename){//打印规则
	ofstream output(filename);
	if (!output){
		cout << "未打开输出文件" << endl;
		return;
	}
	sort(strongRule.begin(), strongRule.end(),compare);
	for (unsigned int i = 0; i < strongRule.size(); i++)
	{
		for (unsigned int j = 0; j < strongRule[i].premise.size(); j++)
			output << strongRule[i].premise[j] << "\t";
		output << "\t---->\t";
		for (unsigned int j = 0; j < strongRule[i].result.size(); j++)
			output << strongRule[i].result[j] << "\t";
		output << "\tsupport :\t" << strongRule[i].support << "\tconfidence :\t" << strongRule[i].confidence << "\tlift :\t" << strongRule[i].lift << endl;
	}
	output.close();
};