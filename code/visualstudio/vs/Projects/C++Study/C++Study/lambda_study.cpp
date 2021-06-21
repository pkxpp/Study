#include "lamada_study.h"

void TestLambda() {
	int j = 12;
	auto by_val_lambda = [=] { return j + 1;};
	auto by_ref_lambda = [&] { return j + 1;};
	cout << "by_val_lambda: " << by_val_lambda() << endl;
	cout << "by_ref_lambda: " << by_ref_lambda() << endl;
	j++;
	cout << "by_val_lambda: " << by_val_lambda() << endl;
	cout << "by_ref_lambda: " << by_ref_lambda() << endl;
}


void LambdaStudy::TestValueAndReferenceCapture()
{
	int j = 12;
	auto by_val_lambda = [=] { return j + 1;};
	auto by_ref_lambda = [&] { return j + 1;};
	cout << "by_val_lambda: " << by_val_lambda() << endl;
	cout << "by_ref_lambda: " << by_ref_lambda() << endl;
	j++;
	cout << "by_val_lambda: " << by_val_lambda() << endl;
	cout << "by_ref_lambda: " << by_ref_lambda() << endl;
}

/*
* 即使是参数的形式传递进去的j，也是传入的那一刻变成了常量，j++并没有影响后面的值传递调用，依旧还是原来的值
*/
void LambdaStudy::TestValueAndReferenceCapture(int j)
{
	auto by_val_lambda = [=] { return j + 1;};
	auto by_ref_lambda = [&] { return j + 1;};
	cout << "by_val_lambda: " << by_val_lambda() << endl;
	cout << "by_ref_lambda: " << by_ref_lambda() << endl;
	j++;
	cout << "by_val_lambda: " << by_val_lambda() << endl;
	cout << "by_ref_lambda: " << by_ref_lambda() << endl;
}

void LambdaStudy::TestLambdaWithCaptureCannotToFunctionPointerInterface(FuncCallback callback, void* pCallbackCookie)
{
	if (callback)
	{
		callback(pCallbackCookie);
	}
}

void LambdaStudy::TestLambdaWithCaptureCannotToFunctionPointerPrint(void* arg, int nLen)
{
	std::cout << "PrintPtr" << std::endl;
}

/*
 * @2021/04/21
 * 需求上剑三引擎的接口是提供了一个函数指针，但是我这边用的是std::function，但是当捕获的时候会编译报错
 */
void LambdaStudy::TestLambdaWithCaptureCannotToFunctionPointer()
{

	/*FUNCTION_CALLBACK callback = [](void*)
	{
		std::cout << "call back." << std::endl;
	};*/
	FUNCTION_CALLBACK call = std::bind(&LambdaStudy::TestLambdaWithCaptureCannotToFunctionPointerPrint, this, nullptr, 5);

	// E0413	不存在从 "lambda []void (void *)->void" 到 "FuncCallback" 的适当转换函数
	/*TestLambdaWithCaptureCannotToFunctionPointerInterface([&](void*) {
		call(NULL);
	}, NULL);*/

	TestLambdaWithCaptureCannotToFunctionPointerInterface([](void* pCookie) {
		FUNCTION_CALLBACK callback = *(static_cast<FUNCTION_CALLBACK*>(pCookie));
		//call(NULL);
		if (callback)
		{
			callback(NULL);
		}
	}, &call);
}