#include <stdio.h>
typedef void *QUEUE[2];

#define QUEUE_NEXT(q)	(*(QUEUE **) &((*(q))[0]))
#define QUEUE_PREV(q)	(*(QUEUE **) &((*(q))[1]))

void fnThree()
{
	int a = 3;
	int b = 1;
	(a > b)?(a=2):(a=0);
	printf("a = %d\n", a);
}

int main()
{
	int n = 1;
	printf("I am testing...");
	printf("%d, %d\n", n++, n++);
	void *q[2];
	printf("%x, %x, %x\n", q, &(q[0]), &(q[1]));
	printf("%x, %x, %x\n", q, &QUEUE_NEXT(&q), &QUEUE_PREV(&q));
	printf("%x, %x, %x\n", q, &((*(q))[0]), &((*(q))[1]) );

	printf("test fnThree...");
	fnThree();
	printf("Test : %.1f\n", 0.382525);
	
	getchar();
	return 0;
}
