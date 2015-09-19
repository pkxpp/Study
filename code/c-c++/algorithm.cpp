#include <iostream>
#include <map>
#include <time.h>
using namespace std;

int main()
{
	time_t start = time(NULL);
	clock_t s = clock();
	const int MAX = 100000;
	int a[MAX] = {};
	int b[MAX] = {};
	int c[MAX] = {};
	// init
	for (int i = 0; i < MAX; i++)
	{
		a[i] = i;
		b[i] = i;
	} 

	// reconstruct b
	map<int, bool> hash;
	for (int i = 0; i < MAX; i++)
	{
		hash.insert(pair<int, bool>(b[i], true));
	}

	// compare
	for (int i =0; i < MAX; i++)
	{
		int n = a[i];
		if (hash[n])
			c[i] = a[i];
	}
	time_t end = time(NULL);
	clock_t e = clock();
	cout << "cost time: " << end - start << ", " << (e - s) << ", " << CLOCKS_PER_SEC << endl;
	return 1;
}
