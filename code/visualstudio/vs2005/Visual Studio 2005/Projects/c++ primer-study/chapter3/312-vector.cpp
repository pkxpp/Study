#include <iostream>
#include <assert.h>
#include <string>
#include <vector>

using namespace std;

/**
 * bool is_equal(const int *ia, int ia_size, const vector<int> &ivec)就因为有个const在函数中有错误
 */
bool is_equal(const int *ia, int ia_size, vector<int> &ivec);
int main(int arg, char **argv)
{
	int ia[] = { 5, 4, 8};
	vector<int> ivec;
	int itemp = 0;
	for(int i = 0; i < 3; i++)
	{
		cin >> itemp;
		ivec.push_back(itemp);
	}
	if(is_equal(ia, sizeof(ia), ivec))
		cout << "Match" << endl;
	else
		cout << "Not Match" << endl;

//	sleep(1000);
	getchar();
	getchar();
	getchar();
}

bool is_equal(const int *ia, int ia_size, vector<int> &ivec)
{
	int i = 0;
	if(ia_size == 0 && ivec.empty()) return true;
//	Assert(ia_size > 0 && !ivec.is_empty());
	int size = (ia_size < ivec.size())?ia_size:ivec.size();
	vector<int>::iterator it = ivec.begin();
	for(i = 0 ; i < ia_size && it != ivec.end(); i++, ++it)
	{
		if(ia[i] != *it)
			break;
	}

	if(i < ia_size && it < ivec.end()) return false;
	return true;
}