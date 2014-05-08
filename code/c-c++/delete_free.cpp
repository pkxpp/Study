#include <iostream>

using namespace std;

struct A{
  int a;
};

struct B{
  int b;
};

int main(){
  char *pTest = (char*)malloc(8);
  A *pA = (A*)pTest;
  pA->a = 4;
  B *pB = (B*)(pTest+4);
  pB->b = 8;

  cout << pA->a << pB->b << endl;
  delete(pA);
  cout << pB->b << endl;
  cout << pA->a << endl;
  pA->a = 3;
  cout << pA->a << endl;
  return 0; 
}
