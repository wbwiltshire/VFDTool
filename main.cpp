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
void printUsage(string);

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
	string filesystem;
	string version = "0.9.0";

	if (options->validateOptions(argc, argv)) {

		try {

			floppy = new FloppyDrive(options->iFileName);

			//TODO: Implement option to list files in the FAT12 Directory, if it exists
			//TODO: Implement option for writing a file to the FAT12 File System
			if (options->isInfo) {
				biosPB = floppy->readBIOSParmBlock();
				if (biosPB) {
					biosPB->print();
					filesystem = floppy->getFormat();
					cout << "Filesystem detected      : " << filesystem << " (" << (floppy->isFormatted() ? "formatted)" : "not formatted)") <<  endl;;
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
			// Create option only
			else if (options->isCreate && !options->isBootSector) {
				if (floppy->create()) {
					cout << "created VFD file: " << floppy->Name << endl;
					if (floppy->format()) {
						cout << "formatted as " << floppy->getFormat() << endl;
						status = 0;
					}
				}
				else {
					cout << "Unable to create VFD file  " << floppy->Name << endl;
				}

			}
			// Create and write boot sector
			else if (options->isCreate && options->isBootSector) {
				if (floppy->create()) {
					cout << "created VFD file: " << floppy->Name << endl;
					if (floppy->writeBootSector(options->bFileName)) {
						cout << "wrote " << options->bFileName << " to " << floppy->Name << endl;
						if (floppy->format()) {
							cout << "formatted as " << floppy->getFormat() << endl;
							status = 0;
						}
						else {
							cout << "Unable to create VFD file " << floppy->Name << " with boot sector" << endl;
						}
					}
				}
				else {
					cout << "Unable to create VFD file  " << floppy->Name << endl;
				}
			}
			// Write bootsector only
			else if (!options->isCreate && options->isBootSector) {
				if (floppy->writeBootSector(options->bFileName)) {
					cout << "wrote " << options->bFileName << " to " << floppy->Name << endl;
					if (floppy->format()) {
						cout << "re-formatted as " << floppy->getFormat() << endl;
						status = 0;
					}
					else {
						cout << "Unable to format VFD file " << floppy->Name << " with boot sector" << endl;
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
		printUsage(version);
		status = 1;
	}

	delete options;
	return status;
}

void printUsage(string v) {
	cout << "VFDTool (version " << v << ")" << endl;
	cout << "Usage: VFDTool <options> <fileName>" << endl;
	cout << "\t-info            : print VFD file info" << endl;
	cout << "\t-init            : create VFD file" << endl;
	cout << "\t-boot <fileName> : Write boot sector to VFD" << endl;
	cout << "\t-list            : list files in VFD file directory" << endl;
	cout << "\t-add <fileName>  : add file to a VFD file directory" << endl;
	cout << "\t-del <fileName>  : delete a file from VFD file directory" << endl;
	cout << "\t-adir <dirName>  : add a directory to VFD file" << endl;
	cout << "\t-rdir <dirName>  : delete a directory from VFD file" << endl;
	cout << "\t<fileName>    : name of VFD file" << endl;
}
