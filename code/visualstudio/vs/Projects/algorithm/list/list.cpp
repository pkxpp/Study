//////////////////////////////////////////////////////////////////////////
// desc: list study
// author: page(pk)
// time	: 2018/02/8
//////////////////////////////////////////////////////////////////////////
#include "list.h"
#include <iostream>
using namespace std;

MyListStudy* MyListStudy::m_pInstance = nullptr;

ListNode* MyListStudy::Reverse(ListNode* h)
{
	ListNode* pCurHead = h;
	if (h->next == nullptr)
		return h;

	
	ListNode* pNewHead = Reverse(h->next);
	h->next->next = h;
	h->next = nullptr;

	return pNewHead;
}

void MyListStudy::Print(ListNode* h)
{
	ListNode* tmp = h;
	while (tmp)
	{
		cout << tmp->nData << " ";
		tmp = tmp->next;
	}
	cout << endl;
}

MyListStudy* MyListStudy::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyListStudy;
	}
	return m_pInstance;
}