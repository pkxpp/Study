#include <stdio.h>

typedef struct _llink{
	int value;
	struct _llink *next;
}slink, *link;

link newnode(int nValue){
	link pNode = (link)malloc(sizeof(slink));
	pNode->value = nValue;
	pNode->next = NULL;
} 

link deletenode(link head, link ptr){
	link pre = NULL;
	if (ptr == head)
		return head->next;
	else
	{
		pre = head;
		while(pre->next != ptr)
			pre = pre->next;

		if (ptr->next == NULL)
			pre->next = NULL;
		else
			pre->next = ptr->next;
	}
	return head;
}

link delnode(link head, link ptr){
	link pre = NULL;
	if (ptr == head)
	{
		link p = head->next;
		head->next = NULL;
		return p;
	}
	else
	{
		pre = head;
		while(pre->next != NULL)
		{
			if (pre->next == ptr)
			{
				pre->next = ptr->next;
				break;
			}
			pre = pre->next;
		}
	}
	return head;
}

llink deletenode(llink head,llink ptr)
{
   llink previous;

   if(ptr == head){
	llink p = head->next;
	head->next = NULL;              
       return p;
    else
    {
        previous = head;
        while(previous -> next != NULL)a{
		if (previous->next == ptr)
			break;	 
            	previous = previous->next;
	}
            
        if(ptr->next == ptr)            
            previous->next = ptr->next;
   }
   return head;
} 

llink deletenode(llink head,llink ptr)
{
   llink previous;

   if(ptr == head)              
       return head->next;
    else
    {
        previous = head;
        while(previous -> next != ptr) 
            previous = previous->next;
            
        if(previous->next == ptr)            
             previous->next = ptr->next;
   }
   return head;
} 
