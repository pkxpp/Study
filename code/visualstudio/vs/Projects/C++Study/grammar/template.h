#pragma once
// 定义1： 基础模版函数
template<typename T>
inline void PushData(T data) {
	printf("[PushData]Template 1\n");
	// Code...
};
// 定义2：基础模版函数(类型指针)
template<typename T>
inline void PushData(T* data) {
	printf("[PushData]Template 2\n");
	// Code...
};
// 定义3：特化的模版函数
template<>
inline void PushData(const char* data) {
	printf("[PushData]Template 3\n");
	// Code...
}


void TestTemplate()
{
	PushData<char*>("String");
	PushData("String");
}