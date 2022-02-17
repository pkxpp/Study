#include "stl_study.h"
#include <iostream>
using namespace std;

void VECTOR_TEST::PrintSizeAndCapacity()
{
	cout << "max size: " << m_vecTest.max_size()
		<< "\tvector size: " << m_vecTest.size()
		<< "\tvector capacity: " << m_vecTest.capacity() << endl;

	if (m_vecTest.size() > 0)
		cout << "address: " << &m_vecTest[0] << endl;
}

void VECTOR_TEST::TestSizeAndCapacity()
{
	PrintSizeAndCapacity();

	m_vecTest.reserve(10);
	PrintSizeAndCapacity();

	m_vecTest.resize(5);
	PrintSizeAndCapacity();

	m_vecTest.resize(10);
	PrintSizeAndCapacity();

	m_vecTest.push_back(1);
	PrintSizeAndCapacity();

	m_vecTest.resize(5);
	PrintSizeAndCapacity();

	m_vecTest.reserve(5);
	PrintSizeAndCapacity();
}

void testSTLStudy()
{
	VECTOR_TEST vectorTest;
	vectorTest.TestSizeAndCapacity();
}