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
#include "fat12.h"

using std::cout;
using std::endl;
using std::string;

// Constants
const unsigned int SECTORSIZE = 512;

class FloppyDrive {
private:
	BIOSParmBlock* biosPB;
	DIRECTORY* directory;
	FAT12TABLE* fatTable;
	bool write();

public:
	string Name;
	FloppyDrive(string);
	BIOSParmBlock* readBIOSParmBlock();
	DIRECTORY* readDirectory();
	bool create();
	bool createWithBootSector(string);
	bool addFile(string, Directory*);
	bool isFormatted();
	~FloppyDrive();
};