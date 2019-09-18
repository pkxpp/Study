// ***************************************************************
//  FileName	: $FILE_BASE$.h 
//  Creator 	: page(pk)  
//  Date		: $MONTH_02$/$DAY_02$/$YEAR$ $HOUR$:$MINUTE$:$SECOND$
//  Comment	: 
// ***************************************************************
#include <stdio.h>
#include <string.h>

char* strchr_trad(char *p, char c)
{
	if( NULL == p)
		return NULL;

	while( '\0' != *p)
	{
		if( (unsigned char)*p >= 0x81 && (unsigned char)*p <= 0xfe )/* big5 code */
		{
			p += 2;
			continue;
		}
		if( *p == c )
			return p;
		p++;
	}
}

int
is_gbk_chinese(unsigned char cFirst, unsigned char cSecond){
	if (cFirst >= 0xb0 && cFirst <= 0xf7 &&		
	    cSecond >= 0xa1 && cSecond <= 0xfe)
		return 1;
	return 0;
}

int main()
{
	char *str = "36	Га礚и	<color=Blue2>虫┷	<color=White>糤㎝㏄瞅ねよ翴计 <color=Blue1>[=state_1]%+[=state_0] <color=White>翴	<color=White>ず <color=Red1>[=cost_stamina_percent]% <enter><color=White>糤㎝㏄瞅ねよ翴计 <color=Blue1>[=state_1]%+[=state_0] <color=White>翴<enter><color=White>尿丁 <color=Green1>[=skill_display_persist_time] <color=White><enter>		100			坐忘无我			武当\无我心法_a_p.3e	";
	char *p = strchr_trad(str, '[');
	printf("1: %s\n", p);
	p = NULL;
	p = strchr(str, '[');
	printf("2: %s\n", p);
}