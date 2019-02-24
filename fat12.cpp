/*************************************************************************************/
// FAT12 class
//
// Convert cluster number to sector
/*************************************************************************************/
#include <memory>
#include "fat12.h"

FAT12::FAT12(FAT12TABLE* f) : fatTable(f) {
}


//TODO: FAT data starts in 2nd cluster
//     First 12 bits (cluster 0) has media type (FF0), next 12 bits (cluster 1) is the FAT state (FFF == clean), then the directory clusters start at cluster 2
//     0xFF8 or higher means end of file
//     0xFF7 means bad sector
unsigned short FAT12::getCluster(unsigned int cluster) {
	unsigned short data = 0;

	//TODO: Replace cluster offset computation with the following:
	//      unsigned int FAT_Offset = file->currentCluster + (file->currentCluster / 2); //multiply by 1.5 
	//      because it rounds down
	unsigned int offset = (cluster % 2 == 0) ? (unsigned int)(cluster * 1.5) : (unsigned int)(cluster * 1.5) + 1;
	FAT12TABLE* ptr = fatTable;
	for (unsigned char idx = 0; idx < offset; idx++)
		ptr++;
	memcpy(&data, ptr, sizeof(data));

	//TODO: Need to adjust based on if cluster is odd or even
	//if (file->currentCluster & 0x0001)
	//	nextCluster >>= 4;      //grab high 12 bits
	//else
	//	nextCluster &= 0x0FFF;   //grab low 12 bits

	data = data & 0xFFF0;			// mask to 12 bits;
	return data;
}

FAT12::~FAT12() {
}