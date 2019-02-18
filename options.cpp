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
		else if (argString == "-S") {
			(*this).isSettings = true;
			(*this).sFileName = argv[arg + 1];
			arg++;
			arg++;
		}
		else if (argString == "-B") {
			(*this).isBootSector = true;
			(*this).bFileName = argv[arg + 1];
			arg++;
			arg++;
		}
		else {
			(*this).iFileName = argv[arg];
			arg++;
		}
	}

	if ((isCreate || isInfo || isSettings || isBootSector) && (iFileName.length() > 0))
		status = true;

	return status;
}
string Options::toString() {
	return "|" + iFileName + "|" + sFileName + "|" + bFileName + "|" + (isInfo ? "true" : "false") + "|" +  (isCreate ? "true" : "false") + "|" + (isSettings ? "true" : "false") + "|" +  (isBootSector ? "true" : "false");
}

Options::~Options() {
}