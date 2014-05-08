/*
 * pk
 * 2013-10-21
 * 想知道05这样计算出来的是不是八进制
*/
#include <iostream>

using namespace std;

int main()
{
  long a = 20130903;
  
  int nYear = (a/10000)%10000;
  int nMon = (a/100)%100;
  int nDay = a%100;
  cout << nYear << endl
       << nMon << endl
       << nDay << endl;
  
  return 0;
}
