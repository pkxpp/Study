#include <cstdio>
#include <cstdlib>
int* fun(int n)
{
    int *p1=new int[n];
    int *p2=new int[n];
    return p2;
}
int main()
{
    int n;
    scanf("%d",&n);
    int *p=fun(n);
    delete [] p;
    return 0;
}
