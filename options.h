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
	bool isInfo = false;
	bool isCreate = false;
	bool isBootSector = false;

	Options();
	bool validateOptions(int, char*[]);
	string toString();
	~Options();
};