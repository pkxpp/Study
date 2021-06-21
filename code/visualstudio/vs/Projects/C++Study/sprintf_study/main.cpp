#include <stdio.h>
#include <string.h>
int main()
{
	char buf[20];
	char *pstr = "this is a test string";

	memset(buf, 0, sizeof(buf));

	int nLen = _snprintf(buf, sizeof(buf)-1, "%s", pstr);

	printf("buf = %s, strlen(buf) = %d, nLen = %d\n", buf, strlen(buf), nLen);

	getchar();
	return 0;

}