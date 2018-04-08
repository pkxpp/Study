/**
 * test File
 */

#include <cstring>
#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main()
{
	FILE *fp;
	if((fp = fopen("TestFile.txt", "rb+")) == NULL)
		printf("read file error\n");
	else
		printf("OK");
	FILE *fpTemp = fp;

	fseek(fpTemp, 3, SEEK_SET);
	char buf[1024] = {0};
//	while(!feof(fpTemp))
//	{
		fread(buf, 1, 1, fp);
		printf("buf: %s\n", buf);
		fread(buf, 1, 1, fpTemp);
		printf("buf: %s\n", buf);
		fwrite(buf, 1, 1, fp);
//	}
	getchar();
	getchar();
	fclose(fp);
	return 0;
}