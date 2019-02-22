/*************************************************************************************/
// Floppy Drive class
//
// Floppy Drive layout
// -------------------------------------
// | Boot Sector (512 bytes)           |
// -------------------------------------
// | Extended Boot Sector (512 bytes)  |
// -------------------------------------
// | FAT 1 (512 bytes)                 |
// -------------------------------------
// | FAT 2 (512 bytes)                 |
// -------------------------------------
// | Root Directory (512 bytes)        |
// -------------------------------------
// | Data ( bytes)                     |
// -------------------------------------
//
/*************************************************************************************/
#include "floppydrive.h"

using std::exception;
using std::ifstream;
using std::ofstream;

FloppyDrive::FloppyDrive(string n) : Name (n) {
	biosPB = new BIOSParmBlock();
}

bool FloppyDrive::write() {
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

bool FloppyDrive::create() {
	bool status = false;

	biosPB->setFAT12();
	if (write())
		status = true;

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

bool FloppyDrive::createWithBootSector(string bsFName) {
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

			// Write the new VFD will with the boot sector just read
			if (write())
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
	

FloppyDrive::~FloppyDrive() {
	delete biosPB;
	delete directory;
}