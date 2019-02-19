/*************************************************************************************/
// FAT12 class header
/*************************************************************************************/
#pragma once

//byte align the structure, so compiler won't pad storage
#pragma pack(push, 1)
#pragma pack(pop)

class FAT12 {
private:
	FAT12* fat[];

public:
	FAT12();
	~FAT12();
};