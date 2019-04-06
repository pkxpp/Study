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