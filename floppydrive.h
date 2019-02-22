/*************************************************************************************/
// Floppy Drive class header
/*************************************************************************************/
#include <iostream>
#include <algorithm>
#include <exception>
#include <iomanip>
#include <fstream>
#include <string>
#include "biosparmblock.h"
#include "directory.h"

using std::cout;
using std::endl;
using std::string;

// Constants
const unsigned int SECTORSIZE = 512;

class FloppyDrive {
private:
	BIOSParmBlock* biosPB;
	DIRECTORY* directory;
	bool write();

public:
	string Name;
	FloppyDrive(string);
	BIOSParmBlock* readBIOSParmBlock();
	DIRECTORY* readDirectory();
	bool create();
	bool createWithBootSector(string);
	~FloppyDrive();
};