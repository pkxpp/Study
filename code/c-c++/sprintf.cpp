#include <stdio.h>
#include <string.h>

int main()
{
    char buf[20];
    char *pstr = "this is a test string aaaaaaaaaaaa";

    memset(buf, 0, sizeof(buf));
    // windows use _snprintf function.
    // Notice: return value!
    int len = snprintf(buf, sizeof(buf)-1, "%s", pstr);

    printf("buf = %s, strlen(buf) = %d, len = %d\n", buf, strlen(buf), len);

    return 0;

}
