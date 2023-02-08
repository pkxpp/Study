#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <iostream>
#include <windows.h>

using namespace std;

//referenc: [http://hi.baidu.com/yibansha0/item/7ed3021c87ddc8f286ad4e70]

int main() 
{
	/*
	FILE *myfile=fopen("CharsetExample-unicode-big-endian.txt","rb");
    char a[100];
    fgets(a,100,myfile);
    printf("字节数=%d\n",strlen(a));
    printf("16进制值=");
    for(int i=0;i<strlen(a);i++)
		printf("%hhx\t",a[i]);
    printf("\n字符串输出=%s\n",a);
	*/
	/*
	FILE *myfile;
    wchar_t *name=L"CharsetExample-unicode.txt";
    myfile=_wfopen(name,L"rb"); // "rt"
    fseek(myfile,0,0);
    wchar_t buffw[100];
    fgetws(buffw,100,myfile);
    printf("字节数=%d\n",wcslen(buffw));
    printf("16进制值=");
    for(int i=0;i<wcslen(buffw);i++)
		printf("%hhx\t",buffw[i]);
    //设置本地化信息，.936是简体中文的Code Page。wcout需要依靠它决定将宽字符按什么编码显示
    setlocale(LC_ALL, ".936");
    printf("\n字符串输出=");
    wcout<<buffw+1<<endl; 
	*/
	/*
	FILE *myfile;
    wchar_t *name=L"CharsetExample-utf8.txt";
    myfile=_wfopen(name,L"rb"); 
    fseek(myfile,0,0);
    wchar_t buffw[100];
    fgetws(buffw,100,myfile);
    printf("字节数=%d\n",wcslen(buffw));
    printf("16进制值=");
    for(int i=0;i<wcslen(buffw);i++)
		printf("%hhx\t",buffw[i]);
	//宽字符转换成多字节，第一个参数是Code Page，windows定义了若干Code Page，用来处理不同语言编码的。CP_ACP是ASCII编码，这里主要是求出转换后的字节数。Buffw+1主要是去掉头标记feff。UNICODEàASCII编码
	int len = WideCharToMultiByte( CP_ACP, 0, buffw+1, wcslen(buffw+1), NULL, NULL, NULL, NULL ); 
    LPSTR lpsz = new CHAR[len+1]; 
    //下面才是真正的转换。
	WideCharToMultiByte( CP_ACP, 0, buffw+1, wcslen(buffw+1), lpsz, len, NULL,NULL );
    printf("len = %d",len);
    lpsz[len]='\0';
    printf("\n输出 = %s\n",lpsz);
	*/

	// 4.2.3 - (1)error
	/*
	FILE *myfile;
    wchar_t *name=L"CharsetExample-utf8.txt";
    myfile=_wfopen(name,L"rb"); 
    fseek(myfile,3,0);
    wchar_t buffw[100];
    fgetws(buffw,100,myfile);
    printf("字节数=%d\n",wcslen(buffw));
    printf("16进制值=");
    for(int i=0;i<wcslen(buffw);i++)
		printf("%hhx\t",buffw[i]);
	*/


	// 4.2.3 - (2)right
	FILE *myfile;
    wchar_t *name=L"CharsetExample-utf8.txt";
    myfile=_wfopen(name,L"rb"); 
    fseek(myfile,3,0);
    char buff[100];
    //取得所有UTF-8字节
    fgets(buff,100,myfile);
    printf("字节数=%d\n",strlen(buff));
    printf("16进制值=");
    for(int i=0;i<strlen(buff);i++)
		printf("%hhx\t",buff[i]);

	// first utf-8 --> unicode
	int num=MultiByteToWideChar(CP_UTF8,NULL,buff,-1,NULL,NULL);
    wchar_t *buffw2=new wchar_t[num];
    MultiByteToWideChar(CP_UTF8,NULL,buff,-1,buffw2,num);

	//second: unicode --> 
	int len = WideCharToMultiByte( CP_ACP, 0, buffw2, num, NULL, NULL, NULL, NULL ); 
    LPSTR lpsz = new CHAR[len+1]; 
    WideCharToMultiByte( CP_ACP, 0, buffw2, num, lpsz, len, NULL, NULL );
    lpsz[len]='\0';
    printf("\n字符串输出=");
    printf("%s\n",lpsz);


	// 4.3
	// (1) mbstowcs
    char *ccname="我们";
    wchar_t * wwname=new wchar_t[2];
    //依赖于Locale而不是Code Page
    setlocale(LC_CTYPE,"chs");
    mbstowcs(wwname,ccname,2);
    wcout<<wwname<<endl;

	// (2) wcstombs
	wchar_t *wname=L"叶明";
    char * cname=new char[4];
    setlocale(LC_TIME,"chs");
    wcout<<wname<<endl;
    wcstombs(cname,wname,4);
    printf("%s\n",cname);


	getchar();
	return 0;
}