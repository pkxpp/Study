#include <iostream>

using namespace std;

typedef struct tagTreeNode{
	char 	*name;
	int	value;
	tagTreeNode	*left;
	tagTreeNode	*right;
}tagTreeNode;

class CTree
{
public:
	CTree();
	~CTree();
	
	tagTreeNode *Insert(tagTreeNode *newp);
	tagTreeNode *LookUp(char *name);
	void ApplyInorder(void(*fn)(tagTreeNode*, void*), void *arg);
	tagTreeNode *GetTree();
	void FreeTree();
	//Test
	void PrintNode(void *arg);
private:
	tagTreeNode *Insert(tagTreeNode *treep, tagTreeNode *newp);
	tagTreeNode *LookUp(tagTreeNode *treep, char *name);
private:
	tagTreeNode *m_pHead;
}
