// ***************************************************************
//  Copyright(c) Kingsoft
//  FileName	: StringStudy.h 
//  Creator 	: page(pk)  
//  Date		: 05/10/2018 17:56:13
//  Comment	: 
// ***************************************************************
#include <string>
#include <fstream>
#include <iostream>

using namespace std;
const int MAX_PATH_LEN = 256;
const char* MRN_EXT = ".mrn";

bool IsInvalidLine(string str)
{
	size_t nPos = str.find("...");
	if (nPos != 0)
		return true;

	nPos = str.find(MRN_EXT);
	if (nPos == std::string::npos)
		return true;

	return false;
}

string ParseMrn(string str)
{
	size_t nPosStart = str.find("//");
	size_t nPosEnd = str.find(MRN_EXT);\
	if (nPosEnd <= nPosStart)
		return "";

	string strResult = str.substr(nPosStart, (nPosEnd - nPosStart) + string(MRN_EXT).length());
	return strResult;
}

bool ParseMrnFileFromChangeList(const char* pszFileName, char* szOutput)
{
	if (!szOutput)
		return false;
	ifstream fInput;
	string strOutput = "";
	string str;
	fInput.open(pszFileName, ifstream::in);  
	if (fInput.fail())
	{
		cout << "[Error]Open file failed." << endl;
		return false;
	}
	while(!fInput.eof())  
	{  
		getline(fInput, str);
		
		if (IsInvalidLine(str))
			continue;

		// use the space to connect mrn, because of the compile tool uses it.
		strOutput += " " + ParseMrn(str);
	}

	strcpy(szOutput, strOutput.c_str());
	fInput.close();

	return true;
}


void StringStudy()
{
	char szMrn[2048];
	ParseMrnFileFromChangeList("..\\C++Study\\ChangeListDepotPath.txt", szMrn);

	cout << "Result = " << endl;
	cout << szMrn << endl;
}