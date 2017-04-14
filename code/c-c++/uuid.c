#include <stdio.h>
#include <uuid/uuid.h>

void uuid2string(const uuid_t uu, char* const str) // 把uuid转成字符串。
{
    int i;
    for(i=0;i<16;i++)
        sprintf(str+i*2, "%02X",uu[i]);
    return;
}

void main()
{
    int i;
    char buf[200];
    uuid_t uu;

    uuid_generate(uu);

    printf("{");
    for(i=0;i<15;i++)
        printf("%02X-",uu[i]);
    printf("%02X}\n",uu[15]);

    uuid2string(uu, buf);
    printf("%s\n",buf);

    return;
}
