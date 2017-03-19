#include<iostream>
#include<vector>

using namespace std;

int main()
{
	// struct
	typedef struct tagTestStruct {
		int a;
		char s[32];
	}TestStruct;
	TestStruct st = {3,  "123"};
	TestStruct st2 = st;

	cout << "struct 1: " << st.a << ", " << st.s << endl;
	cout << "struct 2: " << st2.a << ", " << st2.s << endl;

	// vector
	vector<int> vec(2, 5);
	vector<int> vec2 = vec;
	vec2[0] = 3;

	cout << "vec 1: ";
	for(vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
		cout << *it << ",";
	cout << endl;
	
	cout << "vec 2: ";
	for(vector<int>::iterator it = vec2.begin(); it != vec2.end(); ++it)
		cout << *it << ",";
	cout << endl;

	return 0;
}
