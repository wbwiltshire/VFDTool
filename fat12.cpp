/*************************************************************************************/
// FAT12 class
//
/*************************************************************************************/
#include <memory>
#include "fat12.h"

FAT12::FAT12(FAT12TABLE* f) : fatTable(f) {
}

unsigned short FAT12::getCluster(unsigned int cluster) {
	unsigned short data = 0;
	unsigned int offset = (cluster % 2 == 0) ? (unsigned int)(cluster * 1.5) : (unsigned int)(cluster * 1.5) + 1;
	FAT12TABLE* ptr = fatTable;
	for (unsigned char idx = 0; idx < offset; idx++)
		ptr++;
	memcpy(&data, ptr, sizeof(data));
	data = data & 0xFFF0;			// mask to 12 bits;
	return data;
}

FAT12::~FAT12() {
}