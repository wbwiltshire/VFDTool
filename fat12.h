/*************************************************************************************/
// FAT12 class header
/*************************************************************************************/
#pragma once

//byte align the structure, so compiler won't pad storage
#pragma pack(push, 1)
struct FAT12TABLE {
	unsigned char data[512];
};
#pragma pack(pop)

class FAT12 {
private:
	FAT12TABLE* fatTable;

public:
	FAT12(FAT12TABLE*);
	unsigned short getCluster(unsigned int);
	~FAT12();
};