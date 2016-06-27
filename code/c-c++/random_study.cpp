#include <iostream>
#include <stdlib.h>
// mod(2) has a problem.
using namespace std;
int main()
{
	srand(111);
	const int MAX_TURN = 10;
	for(int i = 0; i < MAX_TURN; ++i)
	{
		int nRand = rand();
		int nMod = nRand % 2;
		cout << "nRand = " << nRand << ",nMod = " << nMod << endl;
	}


	return 0;
}
