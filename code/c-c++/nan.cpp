#include <iostream>
#include <math.h>

using namespace std;

int main()
{
    float a = acos(2);
    cout << "a = " << a << endl;
    cout << (a != a) << endl;
    cout << "isinf(a) = " << isinf(NAN) << endl;
    cout << "isnan(a) = " << isnan(NAN) << endl;

    return 0;
}
