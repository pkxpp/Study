#include <iostream>

using namespace std;


class X {};
class Y : public virtual X {};
class Z : public virtual X {};
class A : public Y, public Z{};
int main(){
	cout << "sizeof X = " << sizeof(X) << endl;
	cout << "sizeof Y = " << sizeof(Y) << endl;
	cout << "sizeof Z = " << sizeof(Z) << endl;
	cout << "sizeof A = " << sizeof(A) << endl;
		

	return 0;
}
