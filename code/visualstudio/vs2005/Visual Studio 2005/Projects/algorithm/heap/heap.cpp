//////////////////////////////////////////////////////////////////////////
// desc: list study
// author: page(pk)
// time	: 2018/02/8 2018-02-09 14:30:44
//////////////////////////////////////////////////////////////////////////
#include "heap.h"
#include <iostream>
using namespace std;

void MyHeap::push(int nValue)
{
	if (m_nCurIndex < 0)
		m_nCurIndex = 0;
	else
		++m_nCurIndex;

	_storage[m_nCurIndex] = nValue;

	up(m_nCurIndex);
}

void MyHeap::up(int nIndex)
{
	int nValue = _storage[nIndex];
	int nParent = (nIndex - 1) / 2;
	while (nParent >= 0 && _storage[nParent] < nValue)
	{
		_storage[nIndex] = _storage[nParent];
		_storage[nParent] = nValue;
		if (nParent == 0)
			break;
		nParent = (nParent - 1) / 2;
	}
}

int MyHeap::pop()
{
	// empty pop what?
	if (m_nCurIndex < 0)
		return 0;

	int nValue = _storage[0];
	down();
	--m_nCurIndex;
	return nValue;
}

void MyHeap::down()
{
	int nIndex = 0;
	int nLeftChildIndex = nIndex * 2 + 1;
	int nRightChildIndex = nIndex * 2 + 2;
	int nChildIndex = nLeftChildIndex;
	while (nIndex <= m_nCurIndex  ) {
		if (nLeftChildIndex > m_nCurIndex && nRightChildIndex > m_nCurIndex)
			break;

		if (nLeftChildIndex <= m_nCurIndex && nRightChildIndex <= m_nCurIndex)
			nChildIndex = (_storage[nLeftChildIndex] > _storage[nRightChildIndex]) ? nLeftChildIndex : nRightChildIndex;
		else if (nLeftChildIndex <= m_nCurIndex)
			nChildIndex = nLeftChildIndex;
		else
			nChildIndex = nRightChildIndex;
		
		_storage[nIndex] = _storage[nChildIndex];
		nIndex = nChildIndex;
	}
}