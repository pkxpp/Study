#include <iostream>
#include <cmath>
using namespace std;
int swap(int &a, int &b)
{
	int nTemp = a;
	a = b;
	b = nTemp;
}

//
int quick_sort(int array[], int start, int end)
{
	if (start >= end) return 0;

	int nPoint = array[start];
	int nLast = start+1;

	for (int i = start+1; i < end; i++)
	{
		if (array[i] < nPoint)
		{	
			swap(array[i], array[nLast]);
			nLast++;
		}
	}
	swap(array[start], array[nLast-1]);
	quick_sort(array, start, nLast-1);
	quick_sort(array, nLast, end);
	return 1;
}

// 循环-快速排序
int quick_sort_onetime(int array[], int start, int end);
int quick_sort_for(int array[], int start, int end){
	int nPoint[32] = {0};
	int nPointTemp[32] = {0};
	int nNum = 0;
	int k = 0;	//for nPointTemp
	
	int nTimes = (int)(log(end)/log(2));
	printf("end, nTimes: %d, %d\n", end, nTimes);
	nNum = 1;
	k = 1;
	nPoint[0] = 0;
	for(int i = 0; i <= nTimes; i++)
	{
		k = 0;
		printf("nTimes:%d --- nNum:%d\n", i, nNum);
		//根据节点循环，第一次是一个
		for (int j = 0; j < nNum; j++)
		{
			printf("nPoint[%d]: %d\n", j, nPoint[j]);
			int nSubStart = nPoint[j];
			int nSubN = end - nSubStart;
			printf("nSubStart, nSubN: %d, %d\n", nSubStart, nSubN);
			if(j+1 < nNum)
				nSubN = nPoint[j+1]-nSubStart;
			int nTemp = quick_sort_onetime(array, nSubStart, nSubN);
			printf("nTemp:%d\n", nTemp);
			nPointTemp[k++] = nPoint[j];
			nPointTemp[k++] = nTemp+1;	//从后面一位开始
		}
		
		for(int i = 0; i < k; i++)
			nPoint[i] = nPointTemp[i];
		nNum = k;
	}
	return 1;
}

int quick_sort_onetime(int array[], int start, int end)
{
	if (end <= 1) return 0;

	int nPoint = array[start];
	int nLast = start+1;
	printf("before quick_sort_onetime...\n");
	for(int i1 = start; i1 < start+end; i1++)
		printf("%d\n", array[i1]);

	for (int i = start+1; i < start+end; i++)
	{
		if (array[i] < nPoint)
		{	
			swap(array[i], array[nLast]);
			nLast++;
		}
	}
	swap(array[start], array[nLast-1]);
	printf("after quick_sort_onetime...\n");
	for(int i2 = start; i2 < start+end; i2++)
		printf("%d\n", array[i2]);
	return nLast-1;
}

int test_log()
{
	double a;
	int b;
	a = log(100)/log(10);
	b = (int)a;
	int c = (int)(log(10)/log(2));
	//b = log(E);
	printf("%f, %f, %f, %d\n", log(100), log(10), log(2), c);
	return 1;
}
//main function
int main()
{
	//
	int a[] = {1,3,4,5,2};
	//quick_sort(a, 0, 5);
	test_log();
	quick_sort_for(a, 0, 5);
	for (int i = 0; i < 5; i++)
		cout << a[i] << endl;
	return 0;
}

