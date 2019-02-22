/*************************************************************************************/
// Directory class
//
/*************************************************************************************/
#include "directory.h"

Directory::Directory(DIRECTORY* d, unsigned short entries) : directory(d), numberOfEntries(entries) {
}

void Directory::print() {

	cout << "Directory listing:\n" << endl;
	DIRECTORY* ptr = directory;

	for (unsigned int cnt = 0; cnt < numberOfEntries; cnt++) {
		string fileName((char *)ptr->filename, 8);
		string extName((char *)ptr->ext, 3);
		//  Quite when we find the first blank entry
		if (isEmpty(fileName, extName))
			break;
		ptr += 1;
		cout << fileName << "." << extName << endl;
		
	}
}
bool Directory::isEmpty(string& f, string& e) {
	bool status = false;

	if (f.at(0) == '\0' && e.at(0) == '\0')
		status = true;

	return status;
}

Directory::~Directory() {
}