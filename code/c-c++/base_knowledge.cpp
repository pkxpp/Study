//
// time		: create @ 2014/09/20
// description	: some base knowledge about c++
// 
#include <iostream>

using namespace std;
//////////////////////////////////////////////////////////////
// 1. Construct Function about class
class CA{
public:
	CA(int a, int b)
	{
		cout << "CA construct ... " << endl;
		m_a = a;
		m_b = b;
	}
	void print_number(){
		cout << m_a << ", " << m_b << endl;
	}
	
private:
	int m_a;
	int m_b;
};

class CB : public CA
{
public:
	CB(int a, int b)
	//:CA(b, a)
	{
		cout << "CB construct ... " << endl;
	}
};

///////////////////////////////////////////////////////////
int main()
{
	CB *b = new CB(1, 3);
	b->print_number();
	
	return 0;
}
