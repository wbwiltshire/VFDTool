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
#include "biosparmblock.h"

using std::string;
using std::cout;
using std::endl;
using std::exception;
using std::ifstream;
using std::transform;

// Constants
const unsigned int BUFFSIZE = 512;

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
	char* buffer = new char[BUFFSIZE];
	unsigned int bytesRead = 0;
	unsigned int totalBytesRead = 0;
	Options* options = new Options();
	BIOSParmBlock* biosPB;

	if (options->validateOptions(argc, argv)) {

		try {

			if (options->isInfo) {

				// if file exists, then read BIOS Parameter Block
				ifstream infoStream(options->iFileName.c_str(), std::ios::binary);
				if (infoStream.good()) {

					// read the sector 0
					infoStream.read(buffer, BUFFSIZE);
					bytesRead = infoStream.gcount();
					totalBytesRead += bytesRead;

					biosPB = new BIOSParmBlock(buffer);
					biosPB->printBiosPB();

					status = 0;
				}
				else {
					printUsage();
					status = 1;
				}
				delete biosPB;
				infoStream.close();

			}
			//TODO: Implement create option
			else if (options->isCreate) {
				cout << "Create option not currently implemented." << endl;

				//TODO: Implement create option
				if (options->isBootSector) {
					cout << "Boot sector option not currently implemented." << endl;
				}

			}

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

	delete[] buffer;
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
