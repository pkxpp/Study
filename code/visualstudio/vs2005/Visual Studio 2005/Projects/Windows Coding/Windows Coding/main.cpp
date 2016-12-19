#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string>

#include "KCFileFind.h"
#include "SprConverter.h"

using namespace std;

int ModifySpr(LPCTSTR lpFileName, const FSprHead &SprHead);
int main(int args, char **argv)
{
	/*
	WIN32_FIND_DATA FindFileData;
	HANDLE hFile;
	hFile = FindFirstFile(".\\*", &FindFileData);// 为什么使用".\\"为什么不行，还需要加个*通配符？
	do
	{
		printf("%s\n", FindFileData.cFileName);
	}while(FindNextFile(hFile, &FindFileData));

	FindClose(hFile);
	*/
	WIN32_FIND_DATA FindData;
	KCFileFind cFileFind;
	string strFilter;	//能够匹配的所有文件读入

	unsigned int count = 0;
	unsigned int nSuccess = 0;
	unsigned int nFailed = 0;
	FSprHead SprHead; 
	memset(&SprHead, 0, sizeof(SprHead));

	int sig = 0;
	while(1)
	{
		cout << "***************工具菜单***************" << endl;
		cout << "1.修改帧间隔" << endl;
		cout << "2.暂无" << endl;
		cout << "3. 退出" << endl;
		cin >> sig;
		getchar();
		switch(sig)
		{
			case 1:
				cout << "请输入文件名【*.spr】：" << endl;
				cin >> strFilter;
				cout << "你的输入为：" << strFilter << endl;
				getchar();
				cout << "请输入修改的间隔为：" << endl;
				cin >> SprHead.Interval;
				getchar();
				cout << "你输入的间隔为：" << SprHead.Interval << endl;
				cFileFind.First(strFilter.c_str());
				while(cFileFind.IsExist())
				{
					count++;
					printf("\n***%d***\n", count);
					FindData = cFileFind.GetCurrentFile();
					cout << FindData.cFileName << endl;
					if(ModifySpr(FindData.cFileName, SprHead))
					{
						printf("Failed\n");
						nFailed++;
					}
					else
					{
						printf("Ok\n");
						nSuccess++;
					}
					cFileFind.Next();
				}

				printf("\n总文件数：%d\n", count);
				cout << "成功数：" << nSuccess << " || 失败数：" << nFailed << endl;
				 break;
			case 2:
				 break;
			case 3:
				exit(1);
				break;
		}
	}

	getchar();
	getchar();
//	Sleep(10000);
}

/**
 * 修改文件名为礼品FileName的头部
 * 注：在使用该函数之前需对文件名等作检查
 */
int ModifySpr(LPCTSTR lpFileName, const FSprHead &SprHead)
{
	//读取头文件信息
	FILE *pf = NULL;
	FSprHead *pSprHead;	// 存储读取的Spr文件头
	pf = fopen(lpFileName, "rb+");
	if(NULL == pf)
	{
		printf("Read File error!\n");
		return -1;
	}
	

	// 读取spr文件内容
	DWORD uSprDataSize = filelength(fileno(pf));
	BYTE *pSprData = new BYTE[uSprDataSize];
	if (pSprData == NULL)
	{
		fclose(pf);
		printf("Malloc Failed!\n");
		return -1;
	}
	if (fread(pSprData, uSprDataSize, 1, pf) == 1)
	{
//		fclose(pf);
		pSprHead = (FSprHead*)pSprData;
		if ((LPBYTE)pSprHead + sizeof(FSprHead) > (LPBYTE)pSprData + uSprDataSize)
		{
			printf("Error\n");
			return -1;
		}
		if (*(int*)&pSprHead->Comment[0] != SPR_COMMENT_FLAG || !pSprHead->Frames || pSprHead->FrameFormat != enumFF_LinkPng)
		{
			return -1;
		}
	}
	// 相关信息
	printf("帧数：%d, 帧间隔：%d\n", pSprHead->Frames, pSprHead->Interval);

	/**
	 * 修改信息
	 */
	/// 文件指针定位
	unsigned int offset = sizeof(SprHead.Comment)+sizeof(SprHead.Height)+sizeof(SprHead.Width)+sizeof(SprHead.CenterX)+sizeof(SprHead.CenterY)+
		sizeof(SprHead.Colors)+sizeof(SprHead.Frames)+sizeof(SprHead.Directions);
	fseek(pf, offset, SEEK_SET);

	///写入新的数值
	fwrite(&SprHead.Interval, sizeof(SprHead.Interval), 1, pf);
	rewind(pf);

	return 0;// 成功
}