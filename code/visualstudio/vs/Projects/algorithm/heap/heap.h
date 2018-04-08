//////////////////////////////////////////////////////////////////////////
// desc: heap study
// author: page(pk)
// time	: 2018/02/09 2018-02-09 14:30:40
//////////////////////////////////////////////////////////////////////////

const int MAX_HEAP = 64;

class MyHeap
{
	MyHeap()
	{
		m_nCurIndex = -1;
	}
public:
	void push(int nValue);
	int pop();

	void down();
	void up(int nIndex);

	int size() {
		(m_nCurIndex >= 0) ? (m_nCurIndex + 1) : 0;
	}

private:
	int m_nCurIndex;
	int _storage[MAX_HEAP];
};