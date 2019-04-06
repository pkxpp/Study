#include <stdio.h> 
#include <stdlib.h> 

int calculateGCD(int a, int b); 
void printHelp(); 

int main( int argc, const char* argv[] ) 
{ 
	int numA, numB, gcd; 

	if (argc != 3) 
	{ 
		printHelp(); 
		return -1; 
	} 

	numA = atoi( argv[1] ); 
	numB = atoi( argv[2] ); 

	if(numA == 0 || numB == 0) 
	{ 
		printf("Wrong input format, see help"); 
		printHelp(); 
		return -1; 
	} 

	// Make sure that numA is not less then numB 
	// otherwise change them places 
	if (numA < numB) 
	{ 
		numA = numA ^ numB; // 0^1 = 1, 0^0 = 0, 1^1 = 0, 1^0=1; 
		numB = numA ^ numB; 
		numA = numA ^ numB; 
	} 

	gcd = calculateGCD(numA, numB); 
	printf("Common divisor is: %d", gcd); 
	return 0; 
} 

int calculateGCD(int biggerNum, int smallerNum) 
{ 
	int remainder = biggerNum % smallerNum; 

	if (remainder == 0) 
		return smallerNum; 

	return calculateGCD(smallerNum, remainder); 
} 

void printHelp() 
{ 
	printf("This tool calculates a common greatest divisor"); 
	printf("Usage: comdiv integerNumberA integerNumberB"); 
}