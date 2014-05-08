#include<iostream>

int main()
{
    int a = 0;
    //char ch[2] = {'|', '--', };
    while(1){
	std::cout << "\r";
	std::cout << " ";
	std::cout << "\r";
	std::cout << "--";
     	a++;
      	sleep(1);
    }
    return 0;
}
