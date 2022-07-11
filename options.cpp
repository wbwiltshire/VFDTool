/*************************************************************************************/
// Options class
/*************************************************************************************/
#include "options.h"

Options::Options() {

}
bool Options::validateOptions(int argc, char* argv[]) {
	bool status = false;
	unsigned short arg = 0;
	string argString;

	while (arg < argc) {
		argString = string(argv[arg]);

		// Convert to upper case
		transform(argString.begin(), argString.end(), argString.begin(), ::toupper);

		if (argString == "-INFO")
		{
			(*this).isInfo = true;
			arg++;
		}
		if (argString == "-LIST")
		{
			(*this).isList = true;
			arg++;
		}
		else if (argString == "-INIT")
		{
			(*this).isCreate = true;
			arg++;
		}
		//TODO: Need more robust checking for this option (file specified and exists)
		else if (argString == "-BOOT") {
			(*this).isBootSector = true;
			(*this).bFileName = argv[arg + 1];
			arg++;
			arg++;
		}
		//TODO: Need more robust checking for this option (file specified and exists)
		else if (argString == "-ADD") {
			(*this).isAdd = true;
			(*this).aFileName = argv[arg + 1];
			arg++;
			arg++;
		}
		//TODO: Need more robust checking for this option (file exists)
		else {
			(*this).iFileName = argv[arg];
			arg++;
		}
	}

	//TODO: Check if files exists
	// try {
	//if (isBootSector) {
	//    ifstream bsStream(options->iFileName.c_str(), std::ios::binary);
	//    if (bsStream.good()) {
	//    }
	//    bsStream.close();
	//}  
	//if (iFileName.length() > 0) {
	//   ifstream inStream(options->iFileName.c_str(), std::ios::binary);
	//   if (inStream.good()) {
	//   }
	//   inStream.close();
	//}
	//catch (exception ex)
	//{
	//}

	//TODO: Need more robust checking for valid combinations of options
	if ((isCreate || isInfo || isList || isBootSector || isAdd) && (iFileName.length() > 0))
		status = true;


	return status;
}
string Options::toString() {
	return "|" + iFileName + "|" + bFileName + "|" + aFileName + "|" + (isInfo ? "true" : "false") + "|" + (isList ? "true" : "false") + "|" +  (isCreate ? "true" : "false") + "|" + 
		(isAdd ? "true" : "false") + "|" +  (isBootSector ? "true" : "false");
}

Options::~Options() {
}