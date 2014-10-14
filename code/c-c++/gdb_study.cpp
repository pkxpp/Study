#include <stdexcept>
#include <iostream>

using namespace std;

class ABC
{
public:
	virtual ~ABC(){}
	int i;
	int j;
};

void f()
{
	for(int i = 0; i < 1000; ++i)
	{
		ABC *p = new ABC;
	}
	throw std::bad_alloc();
}

int main()
{
	f();
	return 0;
}
