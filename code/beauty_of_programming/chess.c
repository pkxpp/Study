#include <stdio.h>

#define MAX_BITS 4
#define FULLMASK 255
#define LMASK (FULLMASK << MAX_BITS)
#define RMASK (FULLMASK >> MAX_BITS)
#define RSET(b, n) (b = ((b & LMASK) | (n)))
#define LSET(b, n) (b = ((b & RMASK) | ((n) << MAX_BITS)))
#define RGET(b) (b & RMASK)
#define LGET(b) ((b & LMASK) >> MAX_BITS)
#define GRIDW 3
 
void calcuate()
{
	unsigned char b;
	for(LSET(b, 1); LGET(b) <= GRIDW * GRIDW; LSET(b, (LGET(b)+1)))
		for(RSET(b, 1); RGET(b) <= GRIDW * GRIDW; RSET(b, (RGET(b)+1)))
			if (LGET(b) % GRIDW != RGET(b) % GRIDW){
				printf("A: %d -- B: %d\n", LGET(b), RGET(b));
			}
}

void calucate1()
{
	struct {
		unsigned char a:4;
		unsigned char b:4;
	}i;

	for (i.a = 1; i.a <= 9; i.a++)
		for (i.b = 1; i.b <=9; i.b++)
			if (i.a %3 != i.b %3)
				printf("A: %d -- B: %d\n", i.a, i.b);
}

int main()
{
	/* self code
	const char ROW[] = {'8', '9', 'A'};
	const char COL[] = {'e', 'f', 'g'};
	int i = 1;
	//self
	for (i = 1; i < 256; i++)
	{
		int rowA = i & 0xc0 > 6;
		int colA = i & 0x30 > 4;
		int rowB = i & 0x0c > 2;
		int colB = i & 0x03;
		printf("test: %d, %d, %d, %d\n", rowA, colA, rowB, colB);
		if (rowA != rowB){
		//	printf("%d = {%c%c vs %c%c}", i, ROW[rowA], COL[colA], ROW[rowB], COL[colB]);
		}
	}*/
	//calcuate();
	//
	// new function
	calucate1();
	return 0;
}
