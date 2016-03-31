#include<iostream>

using namespace std;

LD(const char *s, const char *t)
{
 int *d; // pointer to matrix
 int n; // length of s
 int m; // length of t
 int i; // iterates through s
 int j; // iterates through t
 char s_i1; // ith character of s
 char s_i2; // ith character of s
 char t_j1; // jth character of t
 char t_j2; // jth character of t
 int *cost; // cost代价矩阵
 int result; // result
 int cell; // contents of target cell
 int above; // contents of cell immediately above
 int left; // contents of cell immediately to left
 int diag; // contents of cell immediately above and to left
 int sz; // number of cells in matrix

 // Step 1 

 n = strlen (s);
 m = strlen (t);
 if (n == 0) 
 {
  return m;
 }
 if (m == 0) 
 {
  return n;
 }
 sz = (n+1) * (m+1) * sizeof (int);
 d = (int *) malloc (sz);
 cost = (int *) malloc (sz);

 // Step 2

 for (i = 0; i <= n; i++) 
 {
  PutAt (d, i, 0, n, i);
 }

 for (j = 0; j <= m; j++)
 {
  PutAt (d, 0, j, n, j);
 }
 for (int g=0;g<=m;g++)//把代价距离矩阵全部初始化为同一个值，以后可根据此值判断相应的方格是否被赋过值
 {
  for(int h=0;h<=n;h++)
  {
   PutAt(cost,h,g,n,2);
  }
 }
 // Step 3

 for (i = 1; i <= n; i++) 
 {

  s_i1 = s[i-1];
  s_i2 = s[i];
  bool sbd=false;
  bool tbd=false;
  if(s_i1>=' '&&s_i1<='@'||s_i1>='A'&&s_i1<='~')
  {//s为标点符号或其他非中文符号和数字
  sbd=true;
  }
  // Step 4

  for (j = 1; j <= m; j++) 
  {

   tbd=false;
   t_j1 = t[j-1];
   t_j2 = t[j];
   // Step 5
   if(t_j1>=' '&&t_j1<='@'||t_j1>='A'&&t_j1<='~')
   {//t也为标点符号
    tbd=true;
   }
   if(!sbd)
   {//s为汉字
    if(!tbd)
    {//t也为汉字
     if (s_i1 == t_j1&&s_i2 == t_j2) 
     {
      bool tt=false;
      int temp=GetAt(cost,i,j,n);
      if(temp==2)
      {
       PutAt(cost,i,j,n,0);
       tt=true;
      }
      if(tt)
      {//因为st全市汉字，所以把代价矩阵他相邻的未赋过值的三个格赋值
       int temp1=GetAt(cost,i+1,j,n);
       if(temp1==2)
       {
        PutAt(cost,i+1,j,n,0);
       }
       int temp2=GetAt(cost,i,j+1,n);
       if(temp2==2)
       {
        PutAt(cost,i,j+1,n,0);
       }
       int temp3=GetAt(cost,i+1,j+1,n);
       if(temp3==2)
       {
        PutAt(cost,i+1,j+1,n,0);
       }
      }
     }
     else 
     {
      bool tt=false;
      int temp=GetAt(cost,i,j,n);
      if(temp==2)
      {
       PutAt(cost,i,j,n,1);
       tt=true;
      }
      if(tt)
      {
       int temp1=GetAt(cost,i+1,j,n);
       if(temp1==2)
       {
        PutAt(cost,i+1,j,n,1);
       }
       int temp2=GetAt(cost,i,j+1,n);
       if(temp2==2)
       {
        PutAt(cost,i,j+1,n,1);
       }
       int temp3=GetAt(cost,i+1,j+1,n);
       if(temp3==2)
       {
        PutAt(cost,i+1,j+1,n,1);
       }
      }
     }
    }
    else
    {//t为符号
     bool tt=false;
     int temp=GetAt(cost,i,j,n);
     if(temp==2)
     {
      PutAt(cost,i,j,n,1);
      tt=true;
     }
     if(tt)
     {
      int temp1=GetAt(cost,i+1,j,n);
      if(temp1==2)
      {
       PutAt(cost,i+1,j,n,1);
      }
     }
    
    }

   }
   else
   {//s为符号
    if(!tbd)
    {//t为汉字 
     bool tt=false;
     int temp=GetAt(cost,i,j,n);
     if(temp==2)
     {
      PutAt(cost,i,j,n,1);
      tt=true;
     }
     if(tt)
     {
      int temp1=GetAt(cost,i,j+1,n);
      if(temp1==2)
      {
       PutAt(cost,i,j+1,n,1);
      }
     }
    }
    else
    {
     if(s_i1==t_j1)
     {
      int temp=GetAt(cost,i,j,n);
      if(temp==2)
      {
       PutAt(cost,i,j,n,0);
      }
     }
     else
     {
      int temp=GetAt(cost,i,j,n);
      if(temp==2)
      {
       PutAt(cost,i,j,n,1);
      }
     }
    }

   }

    // Step 6

   above = GetAt (d,i-1,j, n);
   left = GetAt (d,i, j-1, n);
   diag = GetAt (d, i-1,j-1, n);
   int curcost=GetAt(cost,i,j,n);
   cell = Minimum (above + 1, left + 1, diag + curcost);
   PutAt (d, i, j, n, cell);
  }
 }

  // Step 7

  result = GetAt (d, n, m, n);
  free (d);
  return result;

}
