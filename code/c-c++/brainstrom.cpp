#include <iostream>
#include <ctime>

using namespace std;
// 1. test page for
void StudyPage()
{
	const int MAX_ROW = 1000;
	const int MAX_COL = 1024;
	int a[MAX_ROW][MAX_COL];
	// time 1
	time_t t1 = time(NULL);
	clock_t c1 = clock();
	for ( int i = 0; i < MAX_ROW; ++i){
		for (int j = 0; j < MAX_COL; ++j){
			//cout << i << ", " << j << endl;
			a[i][j] = 1;
		}
	}
	time_t t2 = time(NULL);
	clock_t c2 = clock();
	for (int i = 0; i < MAX_COL; ++i)
		for (int j = 0; j < MAX_ROW; ++j)
			a[j][i] = 1;
	time_t t3 = time(NULL);
	clock_t c3 = clock();
	cout << "time cost: " << difftime(t2, t1) << " vs " << difftime(t3, t2) << endl;
	cout << "clock cost: " << (c2 - c1) << " vs " << c3 - c2 << endl;
}

int main()
{
	//time_t tStart = time(NULL);

	/////////////////////////////////
	// 1. test page for
	StudyPage();
	//////////////////////////////////
	
	//time_t tEnd = time(NULL);
	//cout << "time cost: " << difftime(tEnd, tStart) << endl;

	return 0;
}
