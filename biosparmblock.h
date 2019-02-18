/*************************************************************************************/
// BIOSParmBlock class header
/*************************************************************************************/
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
	unsigned char   jump[3] = { 0xEB, 0x3C, 0x90 };
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
	unsigned char   bootSignature[2] = { 0xAA, 0x55 };
};
#pragma pack(pop)

class BIOSParmBlock {
private:
	BiosParmBlock* biosParmBlock;

public:
	BIOSParmBlock(char*);
	void printBiosPB();
	string toString();
	~BIOSParmBlock();
};