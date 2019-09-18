//////////////////////////////////////////////////////////////////////////
// std::is_same compile error
// stackoverflow: https://stackoverflow.com/questions/54320902/overloading-operator-cannot-convert-object-to-base-type-in-assignment#54322968
/* error info:
f:\study\c_c++\vs\projects\c++study\c++study\std_is_same.h(58): warning C4244: '=' : conversion from 'int' to 'float', possible loss of data
1>f:\study\c_c++\vs\projects\c++study\c++study\std_is_same.h(31): error C2440: '=' : cannot convert from 'Complex' to 'double'
1>          No user-defined-conversion operator available that can perform this conversion, or the operator cannot be called
1>          f:\study\c_c++\vs\projects\c++study\c++study\std_is_same.cpp(13) : see reference to function template instantiation 'Complex operator *<Complex,Complex>(T,D)' being compiled
1>          with
1>          [
1>              T=Complex,
1>              D=Complex
1>          ]
1>f:\study\c_c++\vs\projects\c++study\c++study\std_is_same.h(41): error C2440: '=' : cannot convert from 'Complex' to 'double'
1>          No user-defined-conversion operator available that can perform this conversion, or the operator cannot be called
1>f:\study\c_c++\vs\projects\c++study\c++study\std_is_same.h(45): warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
1>f:\study\c_c++\vs\projects\c++study\c++study\std_is_same.h(46): warning C4244: '=' : conversion from 'double' to 'float', possible loss of data
1>f:\study\c_c++\vs\projects\c++study\c++study\std_is_same.h(36): error C2228: left of '.real_part' must have class/struct/union
1>          type is 'int'
1>f:\study\c_c++\vs\projects\c++study\c++study\std_is_same.h(37): error C2228: left of '.imaginary_part' must have class/struct/union
1>          type is 'int'
*/
//////////////////////////////////////////////////////////////////////////
#pragma once
#include <type_traits>
#include <cstdint>

class Complex {
public:
	Complex();

	template<typename T>
	Complex(T real, T imaginary);

	template<typename T, typename D>
	friend Complex operator * (T lhs, D rhs)
	{
		double real_a;
		double real_b;
		double imaginary_a;
		double imaginary_b;

		//if (std::is_same<T, Complex>::value)//if lhs is a Complex
		//{
		//	real_a = lhs.real_part;
		//	imaginary_a = lhs.imaginary_part;
		//}
		//else //base type, some sort of number
		//{
		//	real_a = lhs;
		//	imaginary_a = 0;
		//}
		//if (std::is_same<D, Complex>::value)//if rhs is a Complex
		//{
		//	real_b = rhs.real_part;
		//	imaginary_b = rhs.imaginary_part;
		//}
		//else //base type, some sort of number
		//{
		//	real_b = rhs;
		//	imaginary_b = 0;
		//}
		Complex result;
		result.real_part = (real_b*real_a - imaginary_b*imaginary_a);
		result.imaginary_part = (real_b*imaginary_a + imaginary_b*real_a);
		return result;
	}
private:
	float real_part;
	float imaginary_part;
};

template<typename T>
Complex::Complex(T real, T imaginary)
{
	real_part = real;
	imaginary_part = imaginary;
}

void StdIsSameTest();