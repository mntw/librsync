#include "librsfunc.hpp"


int main(int argc, char * argv[]) {

	if (argc != 2) {
		std::cout << 
		"Wrong parameters, please enter two files that have to be synchronyzed!" 
		<< std::endl;
		return -1;
	}
	std::string filename = argv[1];
	std::string sigFilename = filename + ".sig";
	
	makeSignature(filename, sigFilename);
	
	
	return 0;
}
