#include <iostream>

using namespace std;

typedef struct tagTreeNode{
	char 	name[32];
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
	void ApplyPostOrder(void(*fn)(tagTreeNode*, void*), void *arg);
	tagTreeNode *GetTree();
	void FreeTree();
	//Test
	void PrintNode();
	void _PrintNode(tagTreeNode* treep, void *arg);
	tagTreeNode* NewNode(char *name, int value);
private:
	tagTreeNode *_Insert(tagTreeNode *treep, tagTreeNode *newp);
	tagTreeNode *_LookUp(tagTreeNode *treep, char *name);
	void _ApplyPostOrder(tagTreeNode* treep, void(*fn)(tagTreeNode*, void*), void *arg);
	//void _PrintNode(tagTreeNode* treep, void *arg);
private:
	tagTreeNode *m_pHead;
};
