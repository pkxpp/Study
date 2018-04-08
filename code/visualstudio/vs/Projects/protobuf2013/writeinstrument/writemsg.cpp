#include <iostream>
#include <fstream>
#include <string>
#include "..\instrument.pb.h"
using namespace std;

// This function fills in a Person message based on user input.
void PromptForAddress(depth5md::InstrumentNode* instrument_node) {
	cout << "Enter person ID number: ";
	int id;
	cin >> id;
	instrument_node->set_id(id);
	cin.ignore(256, '\n');

	while (true) {
		cout << "Enter a Instrument: ";
		string inst;
		getline(cin, inst);
		if (inst.empty()) {
			break;
		}

		string* pInst = instrument_node->add_instrument();
		*pInst = inst;
	}
}

// Main function:  Reads the entire address book from a file,
//   adds one person based on user input, then writes it back out to the same
//   file.
int main(int argc, char* argv[]) {
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	if (argc != 2) {
		cerr << "Usage:  " << argv[0] << " ADDRESS_BOOK_FILE" << endl;
		return -1;
	}

	depth5md::InstrumentSet instrument_set;

	{
		// Read the existing address book.
		fstream input(argv[1], ios::in | ios::binary);
		if (!input) {
			cout << argv[1] << ": File not found.  Creating a new file." << endl;
		} else if (!instrument_set.ParseFromIstream(&input)) {
			cerr << "Failed to parse instrument." << endl;
			return -1;
		}
	}

	// Add an address.
	PromptForAddress(instrument_set.add_node());

	{
		// Write the new address book back to disk.
		fstream output(argv[1], ios::out | ios::trunc | ios::binary);
		if (!instrument_set.SerializeToOstream(&output)) {
			cerr << "Failed to write instrument." << endl;
			return -1;
		}
	}

	// Optional:  Delete all global objects allocated by libprotobuf.
	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}