#include <iostream>
#include <string>
using namespace std;

template<typename T>
int compare(const T& v1, const T&v2){
  if (v1 < v2) return -1;
  if (v1 > v2) return 1;
  return 0;
}

int main()
{
  cout << compare(1, 2) << endl;
  string s1 = "abc";
  string s2 = "abd";
  cout << compare(s2, s1) << endl;
  cout << compare("ab", "ac") << endl;

  return 0;
}
