//file:6-7-1.cpp of stl sources

#include <algorithm>
#include <vector>
#include <functional>
#include <iostream>

using namespace std;

template <class T>
struct display{
	void operator()(const T& x) const
	{
		cout << x << ' ';
	}
};

class even_by_two{
public:
	int operator()() const{
		return _x += 2;
	}

private:
	static int _x;
};
