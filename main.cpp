/*****************************************************************/
// Tool for working with Virtual Floppy Disk (VFD) files
//
//  Links:
//		https://wiki.osdev.org/FAT
//
/****************************************************************/

#include <iostream>
#include <exception>
#include <fstream>
#include <iomanip>
#include <memory>
#include <string>
#include <stdio.h>
#include "options.h"
#include "floppydrive.h"
#include "biosparmblock.h"

using std::string;
using std::cout;
using std::endl;
using std::exception;
using std::ifstream;
using std::transform;

// Globals

// Function Prototypes
void printUsage();

//unsigned char FAT_table[sector_size];
//unsigned int fat_offset = active_cluster + (active_cluster / 2);// multiply by 1.5
//unsigned int fat_sector = first_fat_sector + (fat_offset / section_size);
//unsigned int ent_offset = fat_offset % section_size;
//
////at this point you need to read from sector "fat_sector" on the disk into "FAT_table".
//
//unsigned short table_value = *(unsigned short*)&FAT_table[ent_offset];
//
//if (active_cluster & 0x0001)
//table_value = table_value >> 4;
//else
//table_value = table_value & 0x0FFF;

int main(int argc, char *argv[])
{
	unsigned int status = 0;
	string iFName;
	unsigned int bytesRead = 0;
	unsigned int totalBytesRead = 0;
	Options* options = new Options();
	FloppyDrive* floppy = NULL;
	BIOSParmBlock* biosPB = NULL;

	if (options->validateOptions(argc, argv)) {

		try {

			floppy = new FloppyDrive(options->iFileName);

			if (options->isInfo) {
				biosPB = floppy->readBIOSParmBlock();
				if (biosPB) {
					biosPB->printBiosPB();
					status = 0;
				}
			}
			//TODO: Implement create option
			else if (options->isCreate) {
				if (floppy->create()) {
					status = 0;
				}
				else {
					cout << "Unable to create VFD file  " << floppy->Name << endl;
				}

				//TODO: Implement create option
				if (options->isBootSector) {
					cout << "Boot sector option not currently implemented." << endl;
				}

			}

			delete floppy;
		}
		catch (exception ex) {
			cout << "Error: " << ex.what() << endl;
		}

		cout << endl << options->toString() << endl;
	}
	else {
		printUsage();
		status = 1;
	}

	delete options;
	return status;
}

void printUsage() {
	cout << "Usage: VFDTool <options> <fileName>" << endl;
	cout << "\t-i            : print VFD file info" << endl;
	cout << "\t-c            : create VFD file" << endl;
	cout << "\t-b <fileName> : Write boot sector to VFD" << endl;
	cout << "\t<fileName>    : name of VFD file" << endl;
}
