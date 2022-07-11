/*************************************************************************************/
// Floppy Drive class header
/*************************************************************************************/
#pragma once
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
enum FORMAT { FAT_12, FAT_16, UNKNOWN };

class FloppyDrive {
private:
	BIOSParmBlock* biosPB = NULL;
	DIRECTORY* directory = NULL;
	FAT12TABLE* fatTable = NULL;
	bool write();

public:
	string Name;
	FORMAT Format = UNKNOWN;
	FloppyDrive(string);
	BIOSParmBlock* readBIOSParmBlock();
	DIRECTORY* readDirectory();
	bool create();
	bool format();
	bool writeBootSector(string);
	bool addFile(string, Directory*);
	bool isFormatted();
	string getFormat();
	~FloppyDrive();
};