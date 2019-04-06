#pragma once
#include <iostream>
#include <windows.h>
#include <fstream>

// 定义64位整形
typedef __int64 int64_t;

// 获取系统的当前时间，单位微秒(us)
int64_t GetSysTimeMicros()
{
	// 从1601年1月1日0:0:0:000到1970年1月1日0:0:0:000的时间(单位100ns)
#define EPOCHFILETIME   (116444736000000000UL)
	FILETIME ft;
	LARGE_INTEGER li;
	int64_t tt = 0;
	GetSystemTimeAsFileTime(&ft);
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;
	// 从1970年1月1日0:0:0:000到现在的微秒数(UTC时间)
	tt = (li.QuadPart - EPOCHFILETIME) / 10;
	return tt;
}

// 计算1到n之间所有整数的和
int64_t CalculateSum(int n)
{
	if (n < 0)
	{
		return -1;
	}

	/*int64_t sum = 0;
	for (int i = 0; i < n; i++)
	{
		sum += i;
	}
	return sum;*/
	// 优化
	return (n * (1 + n)) >> 1;
}

// 判断整数n是否为质数
//bool IsPrime(int n)
//{
//	if (n < 2)
//	{
//		return false;
//	}
//
//	for (int i = 2; i < n; i++)
//	{
//		if (n %i == 0)
//		{
//			return false;
//		}
//	}
//
//	return true;
//}
// 判断整数n是否为质数（优化）
bool IsPrime(int n)
{
	if (n < 2)
	{
		return false;
	}

	if (n == 2)
	{
		return true;
	}

	//把2的倍数剔除掉
	if (n % 2 == 0)
	{
		return false;
	}

	// 其实不能被小于n的根以下的数整除，就是一个质数
	for (int i = 3; i*i <= n; i += 2)
	{
		if (n % i == 0)
		{
			return false;
		}
	}

	return true;
}


void PrintPrimeSum()
{
	int64_t startTime = GetSysTimeMicros();
	std::ofstream outfile;
	outfile.open("D:\\Test\\PrimeSum.dat", std::ios::out | std::ios::app);
	int count = 0;
	int64_t sum = 0;
	for (int i = 10000; i <= 100000; i++)
	{
		if (IsPrime(i))
		{
			sum = CalculateSum(i);
			//std::cout << sum << "\t";
			outfile << sum << "\t";
			count++;
			if (count % 10 == 0)
			{
				//std::cout << std::endl;
				outfile << std::endl;
			}
		}
	}
	outfile.close();
	int64_t usedTime = GetSysTimeMicros() - startTime;
	int second = usedTime / 1000000;
	int64_t temp = usedTime % 1000000;
	int millise = temp / 1000;
	int micros = temp % 1000;
	std::cout << "执行时间:" << second << "s " << millise << "' " << micros << "''" << std::endl;
}