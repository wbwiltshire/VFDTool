/*************************************************************************************/
// Directory class header
/*************************************************************************************/
#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;

// Template classes
template<class T>
string FormatWithCommas(T value)
{
	stringstream ss;
	ss.imbue(static std::locale(""));
	ss << std::fixed << value;
	return ss.str();
}

//byte align the structure, so compiler won't pad storage
#pragma pack(push, 1)
struct DIRECTORY {
	unsigned char  filename[8];           //filename
	unsigned char  ext[3];                //extension (8.3 filename format)
	unsigned char  attrib;                //file attributes
	unsigned char  reserved;
	unsigned char  timeCreatedMs;         //creation time
	unsigned short timeCreated;
	unsigned short dateCreated;           //creation date
	unsigned short dateLastAccessed;
	unsigned short firstClusterHiBytes;
	unsigned short lastModTime;           //last modification date/time
	unsigned short lastModDate;
	unsigned short firstCluster;          //first cluster of file data
	unsigned int   fileSize;              //size in bytes

};
#pragma pack(pop)

class Directory {
private:
	DIRECTORY* directory;
	unsigned short numberOfEntries;
	
	//methods
	unsigned short freeEntries();
	bool isEmpty(string&, string&);

public:
	Directory(DIRECTORY*, unsigned short);
	bool addEntry(string, unsigned int);
	void print();
	~Directory();
};