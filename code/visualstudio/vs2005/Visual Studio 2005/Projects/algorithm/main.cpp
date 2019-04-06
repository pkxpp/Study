#include <iostream>
#include "list/list.h"

using namespace std;

int main()
{
	//////////////////////////////////////////////////////////////////////////
	/// list 
	// 1.reverse
	ListNode* h = nullptr;
	ListNode* tmp = h;
	for (int i = 1; i < 10; ++i)
	{
		ListNode* node = new ListNode;
		node->nData = i;

		if (i == 1) {
			h = tmp = node;
		}
		else {
			tmp->next = node;
			tmp = node;
		}
	}
	tmp->next = nullptr;

	MyListStudy::GetInstance()->Print(h);

	h = MyListStudy::GetInstance()->Reverse(h);

	MyListStudy::GetInstance()->Print(h);

	getchar();
	return 0;
}