#include <stdio.h>
#include <string.h>

#define PRINT_D(intValue)     printf(#intValue" is %d\n", (intValue))
#define OFFSET(struct,member)  ((char *)&((struct *)0)->member - (char *)0)

//#pragma pack(1)

typedef struct
{
	char    sex;
	short   score;
	char	a;
	int     age;
}student;

int main()
{
	PRINT_D(sizeof(student));
	PRINT_D(OFFSET(student,sex));
	PRINT_D(OFFSET(student,score));
	PRINT_D(OFFSET(student,a));
	PRINT_D(OFFSET(student,age));

	getchar();
	return 0;
}