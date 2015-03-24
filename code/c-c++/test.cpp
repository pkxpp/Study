#include <stdio.h>

int main(){
    char * Monster_run = "monster_run";
    int runNum = 6;
    for( unsigned int i=1;i<=runNum;i++)  
	{  
		char szName[100] = {0};  
		sprintf(szName,"%s%d.png",Monster_run,i);  
		printf("%s\n", szName);  
	}  
    return 0;
}
