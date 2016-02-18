#include <stdio.h>

/* print array */
void print(int a[], int n)
{
	int i;
	for(i = 0; i < n; i++)
		printf("%d ", a[i]);
	printf("\n"); 
}

/* swap function */
void swap(int *a, int *b)
{	
	int temp = *a;
	*a = *b;
	*b = temp;
}

/* quick sort
 * a: array 
 * start: the start index of array
 * end: the out index of array(except end)
 */ 
void quick_sort(int a[], int start, int end)
{
	int i = 0;	// for circle
	int pivot = start;
	int nFind = 0;
	if (start >= end)
		return;
	for(i = start+1; i < end; i++)
	{
		if (a[i] < a[pivot])
		{
			//swap(a+i, a+pivot);
			//swap(&a[i], &a[pivot]);
			nFind = 1;
			pivot++;
		}	
		
		if (nFind == 1 && i != pivot)
			swap(&a[i], &a[pivot]);
		nFind = 0;
	}
	
	if (pivot != start)
		swap(&a[pivot], &a[start]);
	print(a, end);
	// recursive
	quick_sort(a, start, pivot);
	quick_sort(a, pivot+1, end);
}

int partition(int a[], int start, int end)
{
	int pivot = a[start];
	while( start < end){
		while(a[end] >= pivot && start < end) end--;
		a[start] = a[end];
		while(a[start] <= pivot && start < end) start++;
		a[end] = a[start];
	}
	a[start] = pivot;

	return start;
}

void quick_sort2(int a[], int start, int end)
{
	if (start < end)
	{
		int pivot = partition(a, start, end);
		quick_sort2(a, start, pivot-1);
		quick_sort2(a, pivot+1, end);
	}
}

/* main */ 
int main()
{
	int a[] = {5, 4, 7, 3, 2, 9, 1, 8};
	int len = sizeof(a)/sizeof(a[0]);
	printf("sort befaore: \n");
	print(a, len);

	quick_sort2(a, 0, len-1);
	printf("sort after: \n");
	print(a, len);

	return 0;
}
