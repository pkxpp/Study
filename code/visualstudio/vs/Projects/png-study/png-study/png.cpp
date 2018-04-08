#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <io.h>
#include <assert.h>

#define READLEN 32

int main(){
	FILE* fp = fopen("test.png","rb");
	if(!fp)
		return false;

	unsigned int n = 0;
	unsigned char data[READLEN];
	unsigned char *pos = data;
//	BYTE* data = new BYTE[READLEN];

	if(fread(data, READLEN, 1, fp) != 1) return false;
	
	for(int i = 0; i < 8; i++){
		printf("%d ", data[i]);
	}

	pos = data +16;
	printf("\n");
	for(int i = 0; i < 8; i++)
		printf("%d: %d, ", i, *(pos+i));

	pos = data + 16;
//	n = ((unsigned int)(*pos))<<24 + ((unsigned int)(*(pos+1)))<<16 + ((unsigned int)(*(pos+2)))<<8 + (unsigned int)(*(pos+3));
	n = (unsigned int)(*(pos+3)) + ((unsigned int)(*(pos+2)))<<8;
	printf("\nn = %d\n", n);

//	printf("\n");
//	int *num = (int *)(data + 16);
//	for(int i = 0; i < 2; i++){
//		printf("%d ", *num);
//	}

//	pos = data + 16;

	printf("\nint = %d\n", sizeof(unsigned int));
	getchar();
	return 0;
}