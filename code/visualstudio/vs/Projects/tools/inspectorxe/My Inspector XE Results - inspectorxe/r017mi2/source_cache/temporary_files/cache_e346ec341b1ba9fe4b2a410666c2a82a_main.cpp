/* This is memory growth test file ¨C mem_growth.c */
#include <stdio.h>
//#include <stdlib.h>
#include <time.h>
#include <Windows.h>


#define MAX_NUM 120 /* total 120 times to allocate memory, elapsed time 600s */





int main()
{


	char *str[MAX_NUM];


	int i;





	for (i=0; i<MAX_NUM; i++) {


		str[i] = (char *) malloc (16*sizeof(char));


		Sleep(5000); /* wait 5 seconds, each time */


	}


	return 1;

}


