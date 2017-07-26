#include <iostream>
#include <cuda_runtime.h>
#include <cudnn.h>
using namespace std;

void main(){
	cudnnHandle_t handle;
	cudnnStatus_t t = cudnnCreate(&handle);
	cout<< cudnnGetErrorString(t);
	getchar();
}