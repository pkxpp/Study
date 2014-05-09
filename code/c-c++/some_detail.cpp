#include <iostream>
using namespace std;

//////////////////////////
//size
struct tagLengthHeader{
	unsigned short Length;
};

struct tagBiProtoHeader : tagLengthHeader{
	unsigned short cSubProtocol;
};

struct MP_SYNC_ME : tagBiProtoHeader{
	char data[1];
};

struct tagTestData0{
	char data[0];
};
struct tagTestData1{
	char data[1];
};



//end


/////////////////////////////
//main
int main()
{
	//sizeof
	cout << "sizeof(MP_SYNC_ME): " << sizeof(MP_SYNC_ME) << endl;	//6
	cout << "sizeof(data[0]): " << sizeof(tagTestData0) << endl
		<< "sizeof(data[1] " << sizeof(tagTestData1) << endl;	// 0 1
	//return
	return 0;
}
