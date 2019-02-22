/*****************************************************************/
// Tool for working with Virtual Floppy Disk (VFD) files
//
//  Links:
//		https://wiki.osdev.org/FAT
//      
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
#include "directory.h"

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
	DIRECTORY* dirEntries = NULL;
	Directory* directory = NULL;

	if (options->validateOptions(argc, argv)) {

		try {

			floppy = new FloppyDrive(options->iFileName);

			//TODO: Implement option to list files in the FAT12 Directory, if it exists
			//TODO: Implement option for writing a file to the FAT12 File System
			if (options->isInfo) {
				biosPB = floppy->readBIOSParmBlock();
				if (biosPB) {
					biosPB->print();
					cout << "Formatted                : " << (floppy->isFormatted() ? "True" : "False") << endl;;
					status = 0;
				}
			}
			else if (options->isList) {
				biosPB = floppy->readBIOSParmBlock();
				if (biosPB) {
					dirEntries = floppy->readDirectory();
					if (dirEntries) {
						directory = new Directory(dirEntries, biosPB->biosParmBlock.bpbRootEntries);
						directory->print();
						status = 0;
						delete directory;
					}
				}
			}
			else if (options->isAdd) {
				biosPB = floppy->readBIOSParmBlock();
				if (biosPB) {
					dirEntries = floppy->readDirectory();
					if (dirEntries) {
						directory = new Directory(dirEntries, biosPB->biosParmBlock.bpbRootEntries);
						if (directory) {
							if (floppy->addFile(options->aFileName, directory))
								status = 0;
						}
						delete directory;
					}
				}
			}
			else if (options->isCreate) {
				if (floppy->create()) {
					status = 0;
				}
				else {
					cout << "Unable to create VFD file  " << floppy->Name << endl;
				}

				if (options->isBootSector) {
					if (floppy->createWithBootSector(options->bFileName)) {
						status = 0;
					}
					else {
						cout << "Unable to create VFD file with boot sector " << floppy->Name << endl;
					}
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
	cout << "\t-l            : list files in VFD file directory" << endl;
	cout << "\t-a <fileName> : add file to a VFD file directory" << endl;
	cout << "\t-r <fileName> : remove a file from VFD file directory" << endl;
	cout << "\t<fileName>    : name of VFD file" << endl;
}
