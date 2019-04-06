#include "std_is_same.h"

Complex::Complex()
{
	real_part = 0.0;
	imaginary_part = 0.0;
}

void StdIsSameTest()
{
	Complex a(4.0, 8.0);
	Complex b(8, 16);
	auto prod = a*b;
	auto prod2 = a * 2;
}