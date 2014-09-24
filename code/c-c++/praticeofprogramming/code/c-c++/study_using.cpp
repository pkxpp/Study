#include <iostream>

using namespace std;
namespace Exercise{
	int ivar = 0;
	double dvar = 0;
	const int limit = 1000;
}
int var = 0;
//position1
//using declaration
using Exercise::ivar;
using Exercise::dvar;
using Exercise::limit;

void manip(){
	//position2
	double dvar = 3.1416;
	int iobj = limit + 1;
	++ivar;
	++::ivar;
	cout << ivar << dvar << limit 
	     << iobj << ::ivar;
}

int main(){
	cout << "test..............." << endl;
	manip();
	return 0;
}
