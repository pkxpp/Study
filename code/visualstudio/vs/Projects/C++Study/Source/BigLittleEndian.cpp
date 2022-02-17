/* test big or little endian?
 *
 */
#include <stdio.h>

int CheckCPU()
{
	{
		union w
		{
			int a;
			char b;
		}c;
		c.a = 1;
		return (c.b == 1);
	}
}


enum IPACK_FILE_SHELL_PARAM
{
	PACK_FILE_SHELL_MAX_SUPPORT_PAK_NUM = 24,			//最多同时支持的打包文件的数目
	PACK_FILE_SHELL_MAX_SUPPORT_ELEM_FILE_NUM = 200000,	//一个打包文件中最多可以包含子文件的个数
	IPACK_FILE_SHELL_OPTION_SPR_SPLIT_FRAME_BALANCE_MIN = 100,
	IPACK_FILE_SHELL_OPTION_SPR_SPLIT_FRAME_BALANCE_MAX = 1000,
	IPACK_FILE_SIGNATURE_FLAG = 0x4b434150,				//'PACK'，打包文件符。
	IPACK_FILE_SHELL_OPTION_SPR_SPLIT_FRAME_BALANCE_DEF = 131072,		//128K，默认为spr文件大于128K则分帧压缩
								//通过SetOption(IPACK_FILE_SHELL_OPTION_SPR_SPLIT_FRAME_BALANCE..)调整此设定
		
};

/*
int main()
{
	if(CheckCPU())
		printf("Little Endian\n");
	else
		printf("Big Endian\n");

	char pack[4];
	*(int*)pack = IPACK_FILE_SIGNATURE_FLAG;
	getchar();
}
*/
