#include <iostream>
#include "mytree.h"

using namespace std;

CTree::CTree():
m_pHead(NULL)
{}

CTree::~CTree(){
	FreeTree();
}
	
tagTreeNode* CTree::_Insert(tagTreeNode *treep, tagTreeNode *newp)
{
	int cmp;
	if(treep == NULL)
		return newp;
	cmp = strcmp(newp->name, treep->name);
	cout << "treep: " << treep << ", " << treep->name << ", " << endl
		<< "newp: " << newp
		<< newp->name << ", " << cmp << endl;
	if (cmp == 0)
		cout << "insert: duplicate entry " << newp->name << " ignored" << endl;
	else if (cmp < 0 )
		treep->left = _Insert(treep->left, newp);
	else
		treep->right = _Insert(treep->right, newp);
	return treep;
}

tagTreeNode* CTree::_LookUp(tagTreeNode *treep, char *name)
{
	int cmp;
	if (treep == NULL)
		return NULL;
	cmp = strcmp(name, treep->name);
	if (cmp == 0)
		return treep;
	else if (cmp < 0)
		return _LookUp(treep->left, name);
	else
		return _LookUp(treep->right, name);
}


tagTreeNode* CTree::LookUp(char *name)
{
	return _LookUp(m_pHead, name);
}


tagTreeNode* CTree::Insert(tagTreeNode *newp)
{
	cout << "Insert: " 
		<< "m_pHead: " << m_pHead << ", " << endl
		<< "newp: " << newp << endl;
	return m_pHead = _Insert(m_pHead, newp);
}


void CTree::ApplyPostOrder(void(*fn)(tagTreeNode*, void*), void *arg)
{
	_ApplyPostOrder(m_pHead, fn, arg);
}

tagTreeNode* CTree::GetTree()
{
	return m_pHead;
}

void CTree::_ApplyPostOrder(tagTreeNode* treep, void(*fn)(tagTreeNode*, void*), void *arg)
{
	cout << "_ApplyPostOrder: " << treep << endl;
	if(treep == NULL)
		return;
	_ApplyPostOrder(treep->left, fn, arg);
	_ApplyPostOrder(treep->right, fn, arg);
	(*fn)(treep, arg);
}

void CTree::FreeTree()
{}

//test
void CTree::_PrintNode(tagTreeNode* treep, void *arg)
{
	char *fmt = (char*)arg;
	printf(fmt, treep->name, treep->value);
}
void __PrintNode(tagTreeNode* treep, void *arg)
{
	char *fmt = (char*)arg;
	printf(fmt, treep->name, treep->value);
}
	
void CTree::PrintNode()
{
	_ApplyPostOrder(m_pHead, __PrintNode, (void*)"%s, %d\n");
}


tagTreeNode* CTree::NewNode(char *name, int value){
	tagTreeNode *newp;
	newp = new tagTreeNode();//(Nameval*)emalloc(sizeof(Nameval));
	//newp->name = name;
	strncpy(newp->name, name, sizeof(newp->name));
	newp->value = value;
	newp->left= NULL;
	newp->right = NULL;
	return newp;
}
