// regex_study.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <regex>
#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////////
// 1. match
void RegexMatch()
{
	string strName1("CA_Props_Shopboard_01test01_lod1");	
	string strName2("CA_Props_Shopboard_01test01_lod1");
	vector<string> vecStr;
	vecStr.push_back("CA_Props_Shopboard_01test01_lod1");
	vecStr.push_back("CA_Props_Shopboard_01test01_C");
	vecStr.push_back("test_lod0");

	regex reg("(.*)_lod(\\d*)");
	smatch matchResult;
	for (int i = 0; i < vecStr.size(); ++i)
	{
		string strName = vecStr[i];
		if (regex_match(strName, matchResult, reg))
		{
			cout << "[" << i << "] = ";
			for (size_t i = 0; i < matchResult.size(); ++i)
			{
				//cout << "\t" << matchResult[i] << " ";
				string str = matchResult[i];
				cout << "\t" << str << " " << endl;;
			}
		}
	}
}

int main()
{
	// 1.match
	RegexMatch();

	getchar();
    return 0;
}

