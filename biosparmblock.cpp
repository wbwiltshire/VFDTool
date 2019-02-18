/*************************************************************************************/
// Options class
/*************************************************************************************/
#include "biosparmblock.h"

BIOSParmBlock::BIOSParmBlock(char* buffer) {
	biosParmBlock = new BiosParmBlock();
	memcpy(biosParmBlock, buffer, sizeof(BiosParmBlock));
}

void BIOSParmBlock::printBiosPB() {
	char oemID[9];
	char volumeLabel[12];
	char diskType[9];


	memcpy(&oemID, (*biosParmBlock).OEMId, sizeof((*biosParmBlock).OEMId)); oemID[sizeof((*biosParmBlock).OEMId)] = '\0';
	memcpy(&volumeLabel, (*biosParmBlock).bsVolumeLabel, sizeof((*biosParmBlock).bsVolumeLabel)); volumeLabel[sizeof((*biosParmBlock).bsVolumeLabel)] = '\0';
	memcpy(&diskType, (*biosParmBlock).bsFileSystem, sizeof((*biosParmBlock).bsFileSystem)); diskType[sizeof((*biosParmBlock).bsFileSystem)] = '\0';

	cout << "OEM Id                   : " << oemID << endl;
	cout << "sector size              : " << (*biosParmBlock).bpbBytesPerSector << " bytes" << endl;
	cout << "cluster size             : " << short((*biosParmBlock).bpbSectorsPerCluster) << " sector(s)" << endl;
	cout << "reserved (boot) sectors  : " << (*biosParmBlock).bpbReservedSectors << endl;
	cout << "number of FATs           : " << short((*biosParmBlock).bpbNumberOfFATs) << endl;
	cout << "root entries             : " << (*biosParmBlock).bpbRootEntries << endl;
	cout << "total sectors            : " << (*biosParmBlock).bpbTotalSectors << " sectors" << endl;
	cout << "media descriptor byte    : 0x" << std::hex << std::setw(2) << short((*biosParmBlock).bpbMedia) << std::dec << endl;
	cout << "sectors per FAT          : " << (*biosParmBlock).bpbSectorsPerFAT << endl;
	cout << "sectors per track        : " << (*biosParmBlock).bpbSectorsPerTrack << endl;
	cout << "heads                    : " << (*biosParmBlock).bpbHeadsPerCylinder << endl;
	cout << "hidden sectors           : " << (*biosParmBlock).bpbHiddenSectors << endl;
	cout << "big size sectors         : " << (*biosParmBlock).bpbTotalSectorsBig << " sectors" << endl;
	cout << "physical drive Id        : 0x" << std::hex << std::setw(2) << std::setfill('0') << short((*biosParmBlock).bsDriveNumber) << endl;
	cout << "signature                : 0x" << std::hex << std::setw(2) << short((*biosParmBlock).bsExtBootSignature) << endl;
	cout << "serial number            : " << std::hex << std::setw(8) << (*biosParmBlock).bsSerialNumber << std::dec << std::setfill(' ') << endl;
	cout << "volume label             : " << volumeLabel << endl;
	cout << "disk type                : " << diskType << endl;

}

BIOSParmBlock::~BIOSParmBlock() {
	delete biosParmBlock;
}