#pragma once
/*************************************************************************************/
// Options class header
/*************************************************************************************/
#include <iostream>
#include <algorithm>
#include <exception>
#include <fstream>
#include <string>

using std::cout;
using std::endl;
using std::string;

class Options {
private:
	
public:
	string iFileName;
	string bFileName;
	string aFileName;
	bool isInfo = false;
	bool isAdd = false;
	bool isCreate = false;
	bool isBootSector = false;
	bool isList = false;

	Options();
	bool validateOptions(int, char*[]);
	string toString();
	~Options();
};