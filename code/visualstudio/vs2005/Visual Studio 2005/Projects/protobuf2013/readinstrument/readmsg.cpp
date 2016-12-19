#include <iostream>
#include <fstream>
#include <string>
#include "../instrument.pb.h"
using namespace std;

// Iterates though all people in the AddressBook and prints info about them.
void ListPeople(const depth5md::InstrumentSet& iSet) {
	for (int i = 0; i < iSet.node_size(); i++) {
		const depth5md::InstrumentNode& node = iSet.node(i);

		cout << "ID: " << node.id() << endl;
		
		for (int j = 0; j < node.instrument_size(); j++) {
			const string &inst = node.instrument(j);
			cout << inst << endl;
		}
	}
}

// Main function:  Reads the entire address book from a file and prints all
//   the information inside.
int main(int argc, char* argv[]) {
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	if (argc != 2) {
		cerr << "Usage:  " << argv[0] << " ADDRESS_BOOK_FILE" << endl;
		return -1;
	}

	depth5md::InstrumentSet iSet;

	{
		// Read the existing address book.
		fstream input(argv[1], ios::in | ios::binary);
		if (!iSet.ParseFromIstream(&input)) {
			cerr << "Failed to parse instrument set." << endl;
			return -1;
		}
	}

	ListPeople(iSet);

	// Optional:  Delete all global objects allocated by libprotobuf.
	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}