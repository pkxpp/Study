#include <iostream>

using namespace std;

int main()
{
	enum class Options {None, One, All};
	Options o = Options::All;
	//cout << "enum class: " << o << endl;
	return 0;
}

/////////////////////////////////////////////////
/*
 * g++ is in /usr/local/bin/g++ 
 * compile: /usr/local/bin/g++ -std=c++11
 */
/////////////////////////////////////////////////
