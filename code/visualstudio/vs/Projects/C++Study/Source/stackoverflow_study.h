//////////////////////////////////////////////////////////////////////////
// file		: stackoverflow.h
// author	: pk
// time		: 2019/11/19
// desc		: some content can't class puted here
//////////////////////////////////////////////////////////////////////////
#include <iostream>
using namespace std;

// https://stackoverflow.com/questions/56420390/converting-a-pointer-to-x-to-an-array-of-x
void convert_point_to_array();
/*summary
 * in vs2015 it's ok.And the question is happened in MISRA C++
 */


//////////////////////////////////////////////////////////////////////////
// https://www.zhihu.com/question/356558919/answer/900098873

namespace lf_nam
{

	// template <typename T>
	// class Access;

	template <typename T>
	class Data
	{
	public:
		friend class Pal;
		template <typename X> friend class Access;
	private:
		T a;
	};


	class Pal
	{
	public:
		void fun() { Data<int> d; }
	};


	template <typename T>
	class Access
	{
	};

}

using lf_nam::Access;

//int main(int argc, char const *argv[])
//{
//	Access<double> a;
//	return 0;
//}

/*summary
* in vs2015 it's ok.And the question is happened in tdm-g++ 5.1.0
*/

//////////////////////////////////////////////////////////////////////////
// 2. https://stackoverflow.com/questions/61072942/how-does-the-comparator-function-work-in-priority-queue-c-stl
void compare_self_define();

class Student
{
public:
	string a;
	int age;
	Student(string a, int age)
	{
		this->a = a;
		this->age = age;
	}
};

