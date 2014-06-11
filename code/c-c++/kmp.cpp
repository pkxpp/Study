#include <iostream>

int next[20];

void Next(char str[], int len)
{
	next[0] = -1;
	for(int j = 1; j < len; j++)
	{
		int i = next[j-1];
		while(str[j] != str[i+1] && i >= 0)
			i = next[i];
		if(str[j] == str[i+1])
			next[j] = i + 1;
		else
			next[j] = -1;
	}
}

int kmp(char des[], int len1, char pat[], int len2)
{
	Next(pat, len2);
	int p = 0, s = 0;
	while(p < len2 && s < len1)
	{
		if(pat[p] == des[s])
		{
			p++;s++;
		}
		else
		{	
			if( p == 0)
				s++;
			else
				p = next[p-1]+1;
		}
	}
	if(p < len2)
		return -1;
	return s-len2;
}

void GetNext(char s[] ,int len, int   next []){
     int  i  =   0 ;
     int  k  =   - 1 ;
     
     std::cout << "len: " << len << std::endl;
      next [ 0 ]  =   - 1 ;
     while (i  <  len - 1 ){
         if (k ==- 1   ||  s[i]  ==  s[k]){
            i ++ ;
            k ++ ;
             next [i]  =  k;
        } else {
            k  =   next [k];
        }
    }
    std::cout << "i: " << i << std::endl;
}

void GetNextval(char s[] , int len, int  nextval[]){
    int  i  =   0 , k  =   - 1 ;
    std::cout << "len: " << len << std::endl;
    nextval[ 0 ]  =   - 1 ;
     while (i  <  len - 1 ){
         if (k ==- 1   ||  s[i]  ==  s[k]){
            i ++ ;
            k ++ ;
             if (s[i]  !=  s[k]){
                nextval[i]  =  k;
            } else    nextval[i]  =   nextval[k];            
        } else {
            k  =  nextval[k];
        }
    }
}

int main()
{
	char dest[] = "DABABC";
	//char mode[] = "ABAC";
	//char mode[] = "abdabcde";	//-1,0,0,0,1,2,0,0
	char mode[] = "abcabaa";	//-1,0,0,0,1,2,0,0
	//char mode[] = "aaaadd";
	int pos = kmp(dest, sizeof(dest), mode, sizeof(mode));
	std::cout << pos << std::endl;
	int next1[10] = {-10};
	GetNext(mode, sizeof(mode)/sizeof(mode[0]), next1);
	std::cout << "next: " << std::endl;
	for(int i = 0; i < sizeof(next1)/sizeof(next1[0]); i++)
		std::cout << i << ": " << next1[i] << std::endl;

	int nextval[10];
	GetNextval(mode, sizeof(mode)/sizeof(mode[0]), nextval);
	std::cout << "nextval: " << std::endl;
	for(int i = 0; i < sizeof(nextval)/sizeof(nextval[0]); i++)
		std::cout << i << ": " << nextval[i] << std::endl;

	return 1;
}
