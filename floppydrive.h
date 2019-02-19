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

using std::cout;
using std::endl;
using std::string;

// Constants
const unsigned int SECTORSIZE = 512;

class FloppyDrive {
private:
	BIOSParmBlock* biosPB;
	bool write();

public:
	string Name;
	FloppyDrive(string);
	BIOSParmBlock* readBIOSParmBlock();
	bool create();
	~FloppyDrive();
};