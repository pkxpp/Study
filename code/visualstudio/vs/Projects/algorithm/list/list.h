//////////////////////////////////////////////////////////////////////////
// desc: list study
// author: page(pk)
// time	: 2018/02/8
//////////////////////////////////////////////////////////////////////////

struct tagListNode {
	struct tagListNode* next;
	int nData;
};
typedef struct tagListNode ListNode;

class MyListStudy
{
public:
	~MyListStudy() {
		if (m_pInstance)
			delete m_pInstance;
	}
	
	ListNode* Reverse(ListNode* h);
	void Print(ListNode* h);

	static MyListStudy* GetInstance();
private:
	MyListStudy() {};

private:
	static MyListStudy* m_pInstance;
};