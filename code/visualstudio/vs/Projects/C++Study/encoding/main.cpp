#include <iostream>
#include <iomanip>
using namespace std;

#define NELEMS(x)		(sizeof((x))/(sizeof(((x)[0]))))

int main()
{
	system("chcp 65001 && cls");
	//const char* szChinese = "特效";
	//cout << hex << szChinese << endl;

	const char* szChinese = "\xCC\xD8";
	cout << szChinese << endl;

	char szOut[1024] = {0};
	int nSize1 = sizeof(szChinese);
	int nSize2 = sizeof(szChinese[0]);
	int nLen = NELEMS(szChinese);
	for (int i = 0; i < nLen; ++i)
	{
		cout << hex << setfill(' ') << (unsigned short)szChinese[i] << endl;
		//printf("%x", szChinese[i]);
		szOut[i] = tolower(szChinese[i]);
		cout << hex << setfill(' ') << (unsigned short)szOut[i] << endl;
	}

	cout << hex << szOut << endl;
	getchar();
	return 0;
}

// 小结：tolower应该是需要系统在unicode环境下才有用，第一句system("chcp 65001 && cls");并木有生效
