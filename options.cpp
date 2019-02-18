/*************************************************************************************/
// Options class
/*************************************************************************************/
#include "Options.h"

Options::Options() {

}
bool Options::validateOptions(int argc, char* argv[]) {
	bool status = false;
	unsigned short arg = 0;
	string argString;

	while (arg < argc) {
		argString = string(argv[arg]);
		transform(argString.begin(), argString.end(), argString.begin(), ::toupper);

		if (argString == "-I")
		{
			(*this).isInfo = true;
			arg++;
		}
		else if (argString == "-C")
		{
			(*this).isCreate = true;
			arg++;
		}
		//TODO: Need more robust checking for this option (file specified and exists)
		else if (argString == "-B") {
			(*this).isBootSector = true;
			(*this).bFileName = argv[arg + 1];
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
	if ((isCreate || isInfo || isBootSector) && (iFileName.length() > 0))
		status = true;


	return status;
}
string Options::toString() {
	return "|" + iFileName + "|" + bFileName + "|" + (isInfo ? "true" : "false") + "|" +  (isCreate ? "true" : "false") + "|" +  (isBootSector ? "true" : "false");
}

Options::~Options() {
}