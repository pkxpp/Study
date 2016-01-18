#include <stdio.h>

int main()
{
	int count = 0;
	float x = 0.0f;
	float y = 0.0f;
	int bIsBirthday = 0;
	for (y = 1.5f; y > -1.1f; y-= 0.1f){
		for (x = -1.5f; x < 1.5f; x += 0.05f){
			float z = x * x + y * y - 1;
			float f = z * z * z - x * x * y * y * y;
			char c = f <= 0.0f ? ".:-=+*#%@"[(int)(f * -9.0f)] : ' ';
			// modify with birthday
			if (c == '@')
			{
				count += 1;
				bIsBirthday = 0;
				switch(count){
					case 4:
					case 11:
						c = ' ';
					break;
					//19 
					case 2:
						c = '1';
						bIsBirthday = 1;
					break;
					case 3:
						c = '9';
						bIsBirthday = 1;
					break;
					// 88
					case 5:
					case 6:
						c = '8';
						bIsBirthday = 1;
					break;
					// 11
					case 9:
					case 10:
						c = '1';
						bIsBirthday = 1;
					break;
					// 28
					case 12:
						c = '2';
						bIsBirthday = 1;
					break;
					case 13:
						c = '8';
						bIsBirthday = 1;
					break;
				}
				if (bIsBirthday)
					printf("\033[1;36m%c\033[0m", c);
				else
					printf("\033[35m%c\033[0m", c);
					//putchar(c);
			}else
			//putchar(c);
			printf("\033[35m%c\033[0m", c);
		}
		putchar('\n');
	}
	printf("             \033[1;36;45mHappy birthday to my dear QiuMin ^_^\033[0m\n");
	putchar('\n');
	putchar('\n');
	putchar('\n');
}
