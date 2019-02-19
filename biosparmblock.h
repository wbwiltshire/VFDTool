/*************************************************************************************/
// BIOSParmBlock class header
/*************************************************************************************/
#pragma once

#include <iostream>
#include <algorithm>
#include <exception>
#include <iomanip>
#include <fstream>
#include <string>

using std::cout;
using std::endl;
using std::string;

//byte align the structure, so compiler won't pad storage
#pragma pack(push, 1)
struct BiosParmBlock {
	//BiosParmBlock() : jump{ 0xEB, 0x3C, 0x90 }, OEMId{ 'M','S','D','O','S','5','.','0' }, bpbBytesPerSector(512),
	//	bpbSectorsPerCluster(0x01), bpbReservedSectors(1), bpbNumberOfFATs(2), bpbRootEntries(224), bpbTotalSectors(2880),
	//	bpbMedia(0xf0), bpbSectorsPerFAT(9), bpbSectorsPerTrack(18), bpbHeadsPerCylinder(2), bpbHiddenSectors(0),
	//	bpbTotalSectorsBig(0), bsDriveNumber(0x00), bsUnused(0x00), bsExtBootSignature(0x29), bsSerialNumber(0xCB239F6),
	//	bsVolumeLabel{ 'N','O',' ','N','A','M','E',' ',' ',' ',' ' }, bsFileSystem{ 'F','A','T','1','2',' ',' ',' ' }, bootCode{ 0 },
	//	bootSignature{ 0xAA, 0x55 } {}
	unsigned char   jump[3];
	unsigned char   OEMId[8];
	unsigned short  bpbBytesPerSector;
	unsigned char   bpbSectorsPerCluster;
	unsigned short  bpbReservedSectors;
	unsigned char   bpbNumberOfFATs;
	unsigned short  bpbRootEntries;
	unsigned short  bpbTotalSectors;
	unsigned char   bpbMedia;
	unsigned short  bpbSectorsPerFAT;
	unsigned short  bpbSectorsPerTrack;
	unsigned short  bpbHeadsPerCylinder;
	unsigned int    bpbHiddenSectors;
	unsigned int    bpbTotalSectorsBig;
	unsigned char   bsDriveNumber;
	unsigned char   bsUnused;
	unsigned char   bsExtBootSignature;
	unsigned int    bsSerialNumber;
	char            bsVolumeLabel[11];
	char            bsFileSystem[8];
	unsigned char   bootCode[448];
	unsigned char   bootSignature[2];
};
#pragma pack(pop)

class BIOSParmBlock {
private:

public:
	BiosParmBlock biosParmBlock;

	BIOSParmBlock();
	void fill(char*);
	void setFAT12();
	void printBiosPB();
	~BIOSParmBlock();
};