#include <iostream>
#include <vector>
#include <time.h>
#include <stdarg.h>

using namespace std;


typedef struct _tagLYGoods
{
	int nValue;
	int nWeight;
}LYGoods;

int LYMax(int a, int b)
{
	return ((a > b) ? a : b);
}

int LYPrintPack(int **c, LYGoods *A, int nItem, int nSize, std::vector<int> &vecRet)
{
	int *Ret = new int[nItem];
	for (int i = 0; i < nItem; i++)
		Ret[i] = 0;
	for (int i = nItem; i > 0; i--)
	{
		if (c[i][nSize] > c[i - 1][nSize])
		{
			Ret[i - 1] = 1;
			nSize -= A[i - 1].nWeight;
		}
	}
	cout << "PrintPack: " << endl;
	for (int j = 0; j < nItem; j++)
	{
		if (Ret[j] == 0)
			vecRet.push_back(j);
		cout << Ret[j] << ", " << A[j].nWeight << ";";
	}
	cout << endl;

	delete[] Ret;
	return true;
}

int LYPack(LYGoods *A, int nItem, int nSize, std::vector<int> &vecRet)
{
	//
	//int (*MN)[1024];
	printf_s("[Pack]nItem:%d, size:%d\n", nItem, nSize);
	for (int nI = 0; nI < nItem; nI++)
		printf_s("[Pack]%d, %d\n", A[nI].nValue, A[nI].nWeight);

	int **MN = new int*[nItem + 1]();//定义一维动态数组
	for (int i = 0; i<nItem + 1; i++)//用循环令一维数组变成二维
	{
		MN[i] = new int[nSize + 1];
	}
	int nMax = 0;
	for (int i = 0; i < nItem; i++)
	for (int j = 0; j < nSize; j++)
		MN[i][j] = 0;
	for (int i = 1; i <= nItem; i++)
	{
		for (int j = 1; j <= nSize; j++)
		{
			if (j < A[i - 1].nWeight)
			{
				MN[i][j] = MN[i - 1][j];
			}
			else
			{
				MN[i][j] = LYMax(MN[i - 1][j], MN[i - 1][j - A[i - 1].nWeight] + A[i - 1].nValue);
			}
		}
	}

	//cout << "Result: " << endl;
	//for (int i = 0; i < nItem; i++)
	//{
	//	for (int j = 0; j < nSize; j++)
	//		cout << MN[i][j] << ", ";
	//	cout << endl;
	//}
	//cout << "End..." << endl;

	LYPrintPack(MN, A, nItem, nSize, vecRet);

	int nRet = MN[nItem][nSize];

	for (int i = 0; i<nItem + 1; i++)
	{
		delete[] MN[i];
	}
	return nRet;
}

void my_printf(char *szFormat, ...)
{
	char buf[1024] = {0};
	va_list args;
	va_start(args, szFormat);
	vsprintf_s(buf, szFormat, args);
	va_end(args);
	printf(buf);
}

#define my_printf1 printf 

int main()
{
	printf("test:%s\n", "hello world");
	my_printf("test:%s\n", "hello world");
	my_printf1("test:%s\n", "hello world");
	const int GOODS_NUM = 5;
	LYGoods *A = new LYGoods[GOODS_NUM];
	int arrValues[GOODS_NUM] = {2, 11, 15, 20, 32};
	int arrWeight[GOODS_NUM] = {2, 11, 15, 20, 32};
	//例子
	//int arrValues[GOODS_NUM] = {1, 4, 3, 6, 8};
	//int arrWeight[GOODS_NUM] = {2, 3, 4, 5, 6};
	// 我的需求:子集和>x的最小值
	//int arrNeed[GOODS_NUM] = {2, 5, 11, 15, 20};
	//int arrValues[GOODS_NUM] = {2, 5, 11, 15, 20};
	//int arrNeed[GOODS_NUM] = { 2, 11, 15, 20, 25, 29 };
	//int arrValues[GOODS_NUM] = { 12, 13 };
	//int arrWeight[GOODS_NUM] = { 12, 13 };

	int nTarget = 31;
	int nSum = 0;
	for (int i = 0; i < GOODS_NUM; i++)
	{
		nSum += arrWeight[i];
	}
	nTarget = nSum - nTarget;
	cout << "nSum: " << nSum << ", nTarget: " << nTarget << endl;


	for (int i = 0; i < GOODS_NUM; i++)
	{
		A[i].nWeight = arrWeight[i];
		A[i].nValue = arrValues[i];
	}
	std::vector<int> verRet;
	int nRet = LYPack(A, GOODS_NUM, nTarget, verRet);
	//cout << "nRet: " << nRet << endl;

	time_t tNow = time(NULL);
	char szTime[256] = { 0 };
	cout << ctime_s(szTime, 256, &tNow) << endl;
	cout << szTime << endl;
	/*
	//压力测试
	const int nCircl = 10;
	const int C_ITMES = 10;
	const int C_SIZE = 100;
	srand((unsigned int)time(NULL));

	time_t tStart = time(NULL);
	for (int i = 0; i < nCircl; i++)
	{
		int nItemsR = rand() % C_ITMES + 1;
		LYGoods *pA = new LYGoods[nItemsR];
		cout << "元数据: ";
		for (int j = 0; j < nItemsR; j++)
		{
			int nSizeR = rand() % C_SIZE + 1;
			pA[j].nValue = nSizeR;
			pA[j].nWeight = nSizeR;
			cout << nSizeR << ",";
		}

		int nSum = 0;
		for (int k = 0; k < nItemsR; k++)
		{
			nSum += pA[k].nWeight;
		}

		int nTargetR = rand() % nSum + 1;
		cout << ", 目标：" << nTargetR << endl;
		nTargetR = nSum - nTargetR;

		nRet = LYPack(pA, nItemsR, nTargetR);
		//cout << "nRet: " << nRet << endl;
	}
	time_t tEnd = time(NULL);
	cout << "耗时：" << difftime(tEnd, tStart) << "秒" << endl;
	*/
	delete[] A;
	getchar();
	return true;
}