/*************************************************************************************/
// Directory class
//
/*************************************************************************************/
#include "directory.h"

Directory::Directory(DIRECTORY* d, unsigned short entries) : directory(d), numberOfEntries(entries) {
}

///TODO: Need to check the Directory attribute to differentiate between files and directories
void Directory::print() {

	cout << "Directory listing:\n" << endl;
	DIRECTORY* ptr = directory;

	for (unsigned int cnt = 0; cnt < numberOfEntries; cnt++) {
		string fileName((char *)ptr->filename, 8);
		string extName((char *)ptr->ext, 3);
		//  Quit when we find the first blank entry
		if (isEmpty(fileName, extName))
			break;
		cout << fileName << "." << extName << "     " << FormatWithCommas(ptr->fileSize) << " bytes" << endl;
		// reset the locale
		cout.imbue(std::locale("C"));
		ptr += 1;
	}
}

bool Directory::addEntry(string fName, unsigned int size) {
	bool status = false;

	if (freeEntries() < numberOfEntries) {
		// 1. Fill entry structure
		cout << "Added " << fName << " to directory." << endl;
	}
	else
		cout << "Unable to add file. Directory is full!" << endl;

	return status;
}

//Directory entry can be empty if first by is E5 or '\0'
unsigned short Directory::freeEntries() {
	unsigned int entries = 0;
	DIRECTORY* ptr = directory;

	for (unsigned int cnt = 0; cnt < numberOfEntries; cnt++) {
		string fileName((char *)ptr->filename, 8);
		string extName((char *)ptr->ext, 3);

		if (isEmpty(fileName, extName)) {
			entries++;
			break;
		}
		ptr += 1;
	}

	return (numberOfEntries - entries);
}

//Directory entry can be empty if first by is E5 or '\0'
bool Directory::isEmpty(string& f, string& e) {
	bool status = false;

	if (f.at(0) == '\0' && e.at(0) == '\0')
		status = true;

	return status;
}


Directory::~Directory() {
}