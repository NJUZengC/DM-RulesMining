#include<iostream>
//#include"DataSet.h"
#include"Apriori.h"
#include"FPGrowth.h"
#include"DummyBaseline.h"
#include<ctime>
#include<fstream>
#include<string>

string parentfile = "UNIX_usage\\";
string datafile[9] = { "USER0", "USER1", "USER2", "USER3", "USER4", "USER5", "USER6", "USER7", "USER8" };
string childfile = "sanitized_all.981115184025";
int main()
{
	

	while (1){
		int choose = 1;
		do{
			cout << endl << "*************************************************" << endl;
			cout << "       1.Apriori 算法" << endl;
			cout << "       2.FP-Growth 算法" << endl;
			cout << "       3.穷尽算法" << endl;
			cout << "       4.退出" << endl;


			cin >> choose;
		} while (choose < 1 || choose>4);

		if (choose == 4)
			return 0;

		int datatype = 1;
		do{
			cout << endl << "       1.Groceries.csv" << endl;
			cout << "       2.UNIX 数据集" << endl;
			cin >> datatype;
		} while (datatype < 1 || datatype>2);
		
		
		int min_support;
		double supportrate;
		double confidence;
		double lift;
		cout << endl << "       请输入最小支持度阈值   最小支持度比例  最小置信度比例  最小提升度：\n";
		cin >> min_support;
		cin >> supportrate >> confidence>>lift;
		
		

		clock_t start1 = clock();

		switch (choose){
		case 3: {
			if (datatype == 1)
			{
				cout << endl << "     --------------Groceries.csv-----------" << endl;
				DummyBaseline db(min_support, supportrate, confidence,lift);
				db.loadFile("Groceries.csv");
				db.dataMining();
				cout << "     --------------Groceries.csv-----------" << endl;
			}
			else if (datatype == 2){
				for (int i = 0; i < 9; i++)
				{
					cout << endl;
					string filename = parentfile + datafile[i] + "\\" + childfile;
					cout << "     --------------" << datafile[i] << "-----------" << endl;
					DummyBaseline db(min_support, supportrate, confidence,lift);
					db.loadLinux(filename);
					db.dataMining();
					cout << endl;
				}
			}
			
			
			break;

		}

		case 1:{
			
			if (datatype == 1)
			{
				cout << endl<<"     --------------Groceries.csv-----------" << endl;
				Apriori dm(min_support, supportrate, confidence,lift);
				dm.loadFile("Groceries.csv");
				//dm.getFreItems();
				dm.dataMining();
				cout << "     --------------Groceries.csv-----------" << endl;
			}
			else if(datatype==2){
				for (int i = 0; i < 9; i++)
				{
					cout << endl;
					string filename = parentfile + datafile[i] + "\\" + childfile;
					cout << "     --------------" << datafile[i] << "-----------" << endl;
					Apriori dm(min_support, supportrate, confidence,lift);
					dm.loadLinux(filename);
					dm.dataMining();
					cout << endl;
				} 
			}
		
			break;
		};

		case 2:{
			if (datatype == 1)
			{
				cout <<endl<< "     --------------Groceries.csv-----------" << endl;
				FPGrowth ds(min_support, supportrate, confidence,lift);
				ds.loadFile("Groceries.csv");
				ds.DataMining();
				cout << "     --------------Groceries.csv-----------" << endl;
			}
			else if (datatype == 2)
			{
				for (int i = 0; i < 9; i++)
				{
					cout << endl;
					string filename = parentfile + datafile[i] + "\\" + childfile;
					cout << "     --------------" << datafile[i] << "-----------" << endl;
					FPGrowth ds(min_support, supportrate, confidence,lift);
					ds.loadLinux(filename);
					ds.DataMining();
					cout << endl;
				}
			}
			break;

		};

		}
		
		

		
		clock_t end1 = clock();

		cout <<endl<< "     Running time = " << static_cast<double>(end1 - start1) / CLOCKS_PER_SEC << "s" << endl;//输出运行时间
	}
	return 0;
}