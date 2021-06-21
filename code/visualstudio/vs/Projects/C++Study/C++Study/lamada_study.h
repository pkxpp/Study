#include <iostream>
#include <functional>
using namespace std;

void TestLambda();

typedef void (*FuncCallback)(void*);
typedef std::function<void(void*)> FUNCTION_CALLBACK;

class LambdaStudy
{
public:
	void TestValueAndReferenceCapture();
	void TestValueAndReferenceCapture(int j);

	void TestLambdaWithCaptureCannotToFunctionPointerPrint(void* arg, int nLen);
	void TestLambdaWithCaptureCannotToFunctionPointerInterface(FuncCallback callback, void* pCallbackCookie);
	void TestLambdaWithCaptureCannotToFunctionPointer();
}
;