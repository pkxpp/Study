#include <fstream>
#include <string>
#include <iostream>

using namespace std;
int main()
{
	ofstream outfile("outfile");
	ifstream infile("infile");

	if(!infile){
		cerr << "Error: unable to open input file" << endl;
		return -1;
	}

	if( !outfile)
	{
		cerr << "Error: unable to open output file" << endl;
		return -2;
	}

	string word;
	while( infile >> word)
		outfile << word << " ";

	return 0;
}