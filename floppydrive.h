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
const unsigned int FAT12_DATACLUSTERCOUNT = 4084;			 // Microsoft FAT32 System Specification
const unsigned int FAT16_DATACLUSTERCOUNT = 65524;           // Link: http://download.microsoft.com/download/1/6/1/161ba512-40e2-4cc9-843a-923143f3456c/fatgen103.doc
enum FORMAT { FAT_12, FAT_16, FAT_32, FAT_UNKNOWN };

class FloppyDrive {
private:
	BIOSParmBlock* biosPB = NULL;
	DIRECTORY* directory = NULL;
	FAT12TABLE* fatTable = NULL;
	bool writeToFile();

public:
	string Name;
	FORMAT Format = FAT_UNKNOWN;
	FloppyDrive(string);
	BIOSParmBlock* readBIOSParmBlock();
	DIRECTORY* readDirectory();
	bool create();
	bool format();
	bool writeBootSector(string);
	bool addFile(string, Directory*);
	bool isFormatted();
	string getFATFormat();
	~FloppyDrive();
};