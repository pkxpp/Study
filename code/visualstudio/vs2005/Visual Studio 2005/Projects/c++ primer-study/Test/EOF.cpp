/**
 * test EOF
 */

#include <cstring>
#include <string>
#include <iostream>

using namespace std;

int main()
{
	string word;
	while(cin >> word)
		cout << word << endl;

//	Sleep(10000);
	printf("The end\n");
	getchar();
	getchar();
	getchar();
	return 0;
}