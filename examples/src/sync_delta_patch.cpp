#include "librsfunc.hpp"


int main(int argc, char * argv[]) {

	if (argc != 3) {
		std::cout << 
		"Wrong parameters, please enter two files that have to be synchronyzed!" 
		<< std::endl;
		return -1;
	}
	std::string filename = argv[1];
	std::string filename2 = argv[2];
	std::string sigFilename = filename + ".sig";
	std::string deltaFilename = filename + ".delta";
	std::string updFilename = filename + ".upd";
	
	makeSignature(filename, sigFilename);
	makeDelta(sigFilename, filename2, deltaFilename );
	makePatch(filename, deltaFilename, updFilename);
	
	remove(sigFilename.c_str());
	remove(deltaFilename.c_str());
	remove(filename.c_str());
	rename(updFilename.c_str(), filename.c_str());
	
	return 0;
}
