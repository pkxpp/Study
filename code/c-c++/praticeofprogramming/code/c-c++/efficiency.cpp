/*
 * pk 
 * 2013-10-20
 * 想看看两种方式的计算效率怎么样
 * ps：文件名efficiency就是效率的意思，刚查在线翻译的^_^
*/

#include <ctime>
#include <iostream>
#include <stdio.h>

using namespace std;

#define MAX 0x7fff
#define CIRCLE_MAX 100000000
void func_calc1(time_t tCurrent, int nDays);
void func_calc2(time_t tCurrent, int nDays);

int main()
{
 //
  time_t tStart;
  time_t tEnd;
  
  time_t tStand = 1268604000;
  
  srand(time(0));  
  
  tStart = time(NULL);
  for (int i = 0; i < CIRCLE_MAX; i++)
  {
    int nRand = rand()%MAX;
    func_calc1(tStart , nRand);
  }
  tEnd = time(NULL);
  cout << "1: " << difftime(tEnd, tStart) << endl;
  
  tStart = time(NULL);
  for (int i = 0; i < CIRCLE_MAX; i++)
  {
    int nRand = rand()%MAX;
    func_calc2(tStart , nRand);
  }
  tEnd = time(NULL);
  cout << "2: " << difftime(tEnd, tStart) << endl;

  return 1;
}

void func_calc1(time_t tCurrent, int nDays)
{
  time_t tStart = 1268604000;//2010-3-15
  
  bool bRet = false;
  time_t tPassed = tStart + nDays * 86400;
  tm tmCurrent = *localtime(&tCurrent);
  tm tmPassed = *localtime(&tPassed);
  if (tmCurrent.tm_year != tmPassed.tm_year)
    bRet = true;
  else if(tmCurrent.tm_mon != tmPassed.tm_mon)
    bRet = true;

}

void func_calc2(time_t tCurrent, int nDays)
{
  time_t tStart = 1268604000;//2010-3-15
  
  bool bRet;

  time_t tPassed = tStart + nDays * 86400;
  time_t tDiff = tStart - tPassed;
  if (tDiff/86400 > 31 || tDiff/86400 < -31)
    bRet = true;
  else{
    tm tmCurrent = *localtime(&tCurrent);
    tm tmPassed = *localtime(&tPassed);
    if (tmCurrent.tm_year != tmPassed.tm_year)
      bRet = true;
    else if(tmCurrent.tm_mon != tmPassed.tm_mon)
      bRet = true;
  }
}
