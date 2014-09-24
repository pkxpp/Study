#include <iostream>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

using namespace std;

typedef struct Nameval Nameval;
struct Nameval{
	char *name;
	int value;
	Nameval *next;	/*list*/
};

///////////////////////////////////////
Nameval *NewItem(char *name, int value){
	Nameval *newp;
	newp = new Nameval();//(Nameval*)emalloc(sizeof(Nameval));
	newp->name = name;
	newp->value = value;
	newp->next = NULL;
	return newp;
}

// addfront
Nameval *AddFront(Nameval *listp, Nameval *newp)
{
	newp->next = listp;
	return newp;
}

// AddEnd
Nameval *AddEnd(Nameval *listp, Nameval *newp)
{
	Nameval *p;
	if (listp == NULL)
		return newp;
	for(p = listp; p->next != NULL; p = p->next)
		;
	p->next = newp;
	return listp;
}

//lookup
Nameval *LookUp(Nameval *listp, char *name)
{
	for(; listp != NULL; listp = listp->next)
		if(strcmp(name, listp->name) == 0)
			return listp;
	return NULL;	// no match
}

void Apply(Nameval *listp, void (*fn)(Nameval*, void*), void *arg)
{
	for(; listp != NULL; listp = listp->next)
		(*fn)(listp, arg);	//call the function
}

//print
void PrintNV(Nameval *p, void *arg)
{
	char *fmt;
	fmt = (char *)arg;
	printf(fmt, p->name, p->value);
}

void Inccounter(Nameval *p, void *arg)
{
	int *ip;
	//
	ip = (int*)arg;
	(*ip)++;
}

void FreeAll(Nameval *listp)
{	
	Nameval *next;
	for(;listp != NULL; listp = next){
		next = listp->next;
		free(listp);
	}
}

//delitem
Nameval *DelItem(Nameval *listp, char *name){
	Nameval *p, *prev;
	prev = NULL;
	for(p = listp; p != NULL; p = p->next)
	{
		if(strcmp(name, p->name) == 0 ){
			if(prev == NULL)
				listp = p->next;
			else
				prev->next = p->next;
			free(p);
		}
		prev = p;
		return listp;
	}
	printf("delitem: %s not in list", name);
	return NULL;
}
//practice
void _copy_list(Nameval *pList, void *arg){
	Nameval *pTemp = (Nameval*)arg;
	Nameval *pNew = NewItem(pList->name, pList->value);
	if (pTemp == NULL)
		pTemp = pNew;
	else
		pTemp->next = pNew;
}
// 不用Apply函数
// 新理解：copy要考虑之前的链表是否有内容，有的话要删除啊掉
Nameval *link_list_copy(Nameval *pListSrc, Nameval *pListDst){
	//pListDst = pListSrc;
	//Nameval *pTail = pListDst;
	//Apply(pListSrc, _copy_list, pTail);
	FreeAll(pListDst);//把原来的内容给删掉
	Nameval *p = pListSrc;
	Nameval *pD = pListDst;
	for(;p != NULL; p = p->next)
	{
	//	printf("%s: %d\n", p->name, p->value);	
		Nameval *pTemp = NewItem(p->name, p->value);
	//	printf("pTemp: %s, %d\n", pTemp->name, p->value);
		if (pD == NULL)
			pListDst = pD = pTemp;
		else{
			pD->next = pTemp;
			pD = pTemp;
		}
	}
	return pListDst;
}

//翻转-循环
void reverse1(Nameval **pList){
	Nameval *p = *pList;
	Nameval *pNext = p->next;
	Nameval *pPre = NULL;
	while(pNext!= NULL){
		p->next = pPre;
		pPre = p;
		p = pNext;
		pNext = pNext->next;
	}	
	p->next = pPre;
	*pList = p;
}
//翻转-递归
Nameval *reverse2(Nameval *pList, Nameval *pPre){
	/*Nameval *p = pList;
	if(p)
		printf("reverse2:%s, %d", p->name, p->value);
	if (pPre)
		printf("\t:%s, %d\n", pPre->name, pPre->value);
	else
		printf("\n");*/
	if (pList == NULL){
		return pPre;
	}
	Nameval *pHead = reverse2(pList->next, pList);
	pList->next = pPre;	
	return pHead;
}

////////////////////////////////////////
int main(){
	Nameval *nvlist = NULL;
        nvlist = AddFront(nvlist, NewItem("pk", 1));
	for(int i=0; i < 10; i++)
	{
		char szName[32] = {0};
		sprintf(szName, "pk_%d", i);
		nvlist = AddFront(nvlist, NewItem(szName, i));
	}
	printf("Before:\n");
	Apply(nvlist, PrintNV, (void *)"%s, %d\n");
	//reverse1(&nvlist);
	//printf("After1:\n");
	//Apply(nvlist, PrintNV, (void *)"%s, %d\n");
		
	Nameval *pHead = reverse2(nvlist, NULL);
	printf("After2:\n");
	Apply(pHead, PrintNV, (void *)"%s, %d\n");
	//int n = 0;
	//Apply(nvlist, Inccounter, &n);
	//printf("%d elements in nvlist\n", n);
	//Nameval *pNewList = NULL;
	//link_list_copy(nvlist, pNewList);
	//printf("111\n");
	//Apply(pNewList, PrintNV, (void*)"%s: %d\n");
	//pNewList = link_list_copy(nvlist, pNewList);
	//printf("222\n");
	//Apply(pNewList, PrintNV, (void*)"%s: %d\n");
	//pNewList = link_list_copy(nvlist, pNewList);
}

