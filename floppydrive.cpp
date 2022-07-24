/*************************************************************************************/
// Floppy Drive class
//
// Floppy Drive layout
// ----------------------------------------------------------
// | Boot Sector (512 bytes) 1 sector                       |
// ----------------------------------------------------------
// | FAT 1 (9 sectors * 512 = 4,608 bytes)                  |
// ----------------------------------------------------------
// | FAT 2 (9 sectors * 512 = 4,608 bytes)                  |
// ----------------------------------------------------------
// | Root Directory (224 * 32 = 7,169 bytes) or 14 sectors  |
// ----------------------------------------------------------
// | Data ( bytes) rest of the disk / files                 |
// ----------------------------------------------------------
//
/*************************************************************************************/
#include "floppydrive.h"
#include "directory.h"
#include "fat12.h"

using std::exception;
using std::ifstream;
using std::ofstream;
using std::fstream;

unsigned char FAT12_Formatted[3] = { 0xf0, 0xff, 0xff };
unsigned char FAT16_Formatted[4] = { 0xf0, 0xff, 0xff, 0xff };

FloppyDrive::FloppyDrive(string n) : Name (n) {
	biosPB = new BIOSParmBlock();
}

//
// Create the physical file and format at FAT12
//  
// Default:
//   Media Type: f0 =>  3.5" diskette 1440 KB, 2 heads, 80 tracks, 18 sectors / track
//   Bytes: 2 heads * 80 tracks * 18 sectors / track = 2880 sectors * 512 bytes / sector = 1,474,560 bytes (1,440 * 1024 = 1,474,560 )
//   Sectors/Clusters
//   -------
//   1		- BPB Sector
//   18		- 2 FAT * 9 sector / FAT
//   14		- 1 Directory
//	 2847	- Data sectors
//   2880   - Total sectors
bool FloppyDrive::create() {
	bool status = false;

	biosPB->setFAT12();				// Initialize BIOS Parameter Block with the defaults described above 
	Format = FORMAT::FAT_12;		// Mark as formatted as FAT12
	if (writeToFile())					// Write the BIOS block, FAT Tables, Root Directory, and Data clusters to the physical file
		status = true;

	return status;
}

//
// Write the FAT initial cluster markers
//
bool FloppyDrive::format() {
	bool status = false;
	unsigned short FATSector = (biosPB->biosParmBlock.bpbReservedSectors + biosPB->biosParmBlock.bpbHiddenSectors + biosPB->biosParmBlock.bpbTotalSectorsBig)
		* biosPB->biosParmBlock.bpbBytesPerSector;
	char buffer[4];

	try {
		// Open the VFD file for output
		fstream outStream(Name.c_str(), std::ios::in | std::ios::out | std::ios::binary);
		if (outStream.good()) {
			// move to the first FAT table
			outStream.seekg(FATSector, std::ios::beg);

			if (Format == FAT_12) {
				memcpy(buffer, FAT12_Formatted, sizeof(FAT12_Formatted));
			}
			else if (Format == FAT_16) {
				memcpy(buffer, FAT16_Formatted, sizeof(FAT16_Formatted));
			}
			else
				cout << "Error: Unknown FAT format" << endl;

			// Write the first FAT cluster marker
			outStream.write(buffer, sizeof(buffer));

			// move to the Second FAT table
			FATSector += biosPB->biosParmBlock.bpbSectorsPerFAT * biosPB->biosParmBlock.bpbBytesPerSector;
			outStream.seekg(FATSector, std::ios::beg);

			// Write the 2nd FAT cluster marker
			outStream.write(buffer, sizeof(buffer));

			status = true;
			outStream.close();
		}
		else {
			cout << "Unable to VFD file: " << Name << endl;
		}
	}
	catch (exception ex) {
		cout << "Exception writing VFD file with boot sector: " << ex.what() << endl;
	}

	return status;
}

DIRECTORY* FloppyDrive::readDirectory() {
	unsigned int dirSize = biosPB->biosParmBlock.bpbRootEntries * sizeof(DIRECTORY);
	unsigned int dirOffset = ((biosPB->biosParmBlock.bpbSectorsPerFAT * biosPB->biosParmBlock.bpbNumberOfFATs) +
		biosPB->biosParmBlock.bpbReservedSectors + biosPB->biosParmBlock.bpbHiddenSectors) *
		biosPB->biosParmBlock.bpbBytesPerSector;

	directory = new DIRECTORY[biosPB->biosParmBlock.bpbRootEntries]();
	if (directory) {
		ifstream dirStream(Name.c_str(), std::ios::binary);
		if (dirStream.good()) {
			//jump to the directory and read it
			dirStream.seekg(dirOffset, std::ios::beg);
			dirStream.read((char *)directory, dirSize);
		}
	}
	return directory;
}

bool FloppyDrive::writeBootSector(string bsFName) {
	bool status = false;
	short size = 0;
	unsigned short sectorSize = biosPB->biosParmBlock.bpbBytesPerSector;
	char *buffer = new char[sectorSize];

	try {
		// Open the boot sector file for input
		ifstream bsStream(bsFName.c_str(), std::ios::binary);
		if (bsStream.good()) {
			// get size of file
			bsStream.seekg(0, bsStream.end);
			size = bsStream.tellg();
			bsStream.seekg(0);
			//TODO: Add warning if boot sector > 512 bytes (i.e. sector size)

			// read the boot sector into buffer
			bsStream.read(buffer, sectorSize);
			memcpy(&biosPB->biosParmBlock, buffer, sectorSize);

			// Write the new VFD with the boot sector just read
			if (writeToFile())
				status = true;
		}
		else {
			cout << "Unable to open bootloader file: " << bsFName << endl;
		}
		bsStream.close();
	}
	catch (exception ex) {
		cout << "Exception writing VFD file with boot sector: " << ex.what() << endl;
	}

	delete[] buffer;
	return status;
}

bool FloppyDrive::addFile(string name, Directory* directory) {
	bool status = false;
	unsigned int size = 0;

	try {
		ifstream inStream(name, std::ios::binary);
		if (inStream.good()) {
			// get size of file
			inStream.seekg(0, inStream.end);
			size = inStream.tellg();
			inStream.seekg(0);
			// If it's not too big
			// Add the entry to the directory
			directory->addEntry(name, size);
			// Write the directory to the VFD
			// Write each sector to the file to the VFD
			// Update the FAT table
			// Write FAT table to bot VFD FAT tables
			inStream.close();
		}
		else
			cout << "File " << name << " doesn't exist." << endl;
	}
	catch (exception ex) {
		cout << "Exception writing VFD file with boot sector: " << ex.what() << endl;
	}

	return status;
}

BIOSParmBlock* FloppyDrive::readBIOSParmBlock() {
	unsigned int bytesRead = 0;
	char* buffer = new char[SECTORSIZE];

	try {
		// if file exists, then read BIOS Parameter Block
		ifstream infoStream(Name.c_str(), std::ios::binary);
		if (infoStream.good()) {

			// read the sector 0
			infoStream.read(buffer, SECTORSIZE);
			bytesRead = infoStream.gcount();

			biosPB->fill(buffer);
			infoStream.close();
		}
		else {
			cout << "VFD file " << Name << " not found." << endl;
		}
	}
	catch (exception ex) {
		cout << "Exception: " << ex.what() << endl;
	}

	delete[] buffer;
	return biosPB;
}

bool FloppyDrive::isFormatted() {
	bool status = false;
	unsigned short FATsector = (biosPB->biosParmBlock.bpbReservedSectors + biosPB->biosParmBlock.bpbHiddenSectors + biosPB->biosParmBlock.bpbTotalSectorsBig)
		* biosPB->biosParmBlock.bpbBytesPerSector;
	char buffer[4];

	try {
		if (Format == FAT_12 || Format == FAT_16) {
			ifstream inStream(Name.c_str(), std::ios::binary);
			if (inStream.good()) {
				// Jump to the first FAT Table
				inStream.seekg(FATsector, std::ios::beg);

				if (Format == FAT_12) {
					inStream.read(buffer, 3);
					if (memcmp(buffer, FAT12_Formatted, 3) == 0)
						status = true;
				}
				else{
					inStream.read(buffer, 4);
					if (memcmp(buffer, FAT16_Formatted, 4) == 0)
						status = true;
				}
				inStream.close();
			}
			else
				cout << "File " << Name << " doesn't exist." << endl;
		}
	}
	catch (exception ex) {
		cout << "Exception in FloppyDrive::isFormattted(): " << ex.what() << endl;
	}
	return status;
}

/*************************************************************************************/
//
// Determine format (FAT12/FAT16/FAT32)
// Document: Micorosoft FAT32 System Specification - page 14
// Link: http://download.microsoft.com/download/1/6/1/161ba512-40e2-4cc9-843a-923143f3456c/fatgen103.doc
//
/*************************************************************************************/
string  FloppyDrive::getFATFormat() {
	string description;
	unsigned short rootDirSectors = ((biosPB->biosParmBlock.bpbRootEntries * 32) + (biosPB->biosParmBlock.bpbBytesPerSector - 1)) /
		biosPB->biosParmBlock.bpbBytesPerSector;
	unsigned int dataSectors = biosPB->biosParmBlock.bpbTotalSectors - (biosPB->biosParmBlock.bpbReservedSectors +
		(biosPB->biosParmBlock.bpbNumberOfFATs * biosPB->biosParmBlock.bpbSectorsPerFAT) + rootDirSectors);
	unsigned int dataClusterCount = dataSectors / biosPB->biosParmBlock.bpbSectorsPerCluster;			// (rounds down)

	if (dataClusterCount <= FAT12_DATACLUSTERCOUNT) {
		Format = FAT_12;
		description = "FAT12";
	}
	else if (dataClusterCount <= FAT16_DATACLUSTERCOUNT) {
		Format = FAT_16;
		description = "FAT16";
	}
	else {
		Format = FAT_32;
		description = "FAT32";
	}

	return description;
}

bool FloppyDrive::writeToFile() {
	bool status = false;
	unsigned short sectorSize = biosPB->biosParmBlock.bpbBytesPerSector;
	char* buffer = new char[sectorSize];

	try {
		// if file exists, then over-write
		ofstream createStream(Name.c_str(), std::ios::binary);
		if (createStream.good()) {
			// 1. Write BIOSParmBlock
			memcpy(buffer, &biosPB->biosParmBlock, sectorSize);
			createStream.write(buffer, sectorSize);
			//fwrite(&test, 1, biosPB->SectorSize(), outFile);
			// 2. Write (totalSectors - 1) of 0's
			memset(buffer, 0, sectorSize);
			for (unsigned short cnt = 0; cnt < (biosPB->biosParmBlock.bpbTotalSectors - 1); cnt++)
				createStream.write(buffer, sectorSize);
			//bytesWritten = infoStream.gcount();
			status = true;
			createStream.close();
			delete[] buffer;
		}
	}
	catch (exception ex) {
		cout << "Exception writing VFD file: " << ex.what() << endl;
	}

	return status;
}

FloppyDrive::~FloppyDrive() {
	delete biosPB;
	delete directory;
	delete fatTable;
}