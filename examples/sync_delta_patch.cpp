#include <cstdio>
#include <librsync.h>
#include <iostream>
#include <string.h>

int main(int argc, char * argv[]) {

	if (argc != 3) {
		std::cout << "Wrong parameters, please enter two files that have to be synchronyzed!" << std::endl;
		return -1;
	}
	std::string filename = argv[1];
	std::string filename2 = argv[2];
	std::string sigFilename = filename + ".sig";
	std::string deltaFilename = filename + ".delta";
	std::string updFilename = filename + ".upd";
	
	FILE * srcFile; // source file descriptor
	FILE * sigFile; // signature file descriptor
	FILE * newFile; // changed file that has to be sync'd with srcFile
	FILE * deltaFile; 

	srcFile = fopen(filename.c_str(), "r"); 
	sigFile = fopen(sigFilename.c_str(), "w+");

	rs_job_t * sig_job;
	rs_sig_begin((size_t) RS_DEFAULT_BLOCK_LEN, (size_t) RS_DEFAULT_STRONG_LEN);

	rs_result res = rs_sig_file(srcFile, sigFile, RS_DEFAULT_BLOCK_LEN, RS_DEFAULT_STRONG_LEN, NULL); // creating signature

	fclose(srcFile);	
	fclose(sigFile);	
	
	rs_signature_t * sig; // signature object

	sigFile = fopen(sigFilename.c_str(), "r");
	res = rs_loadsig_file(sigFile, &sig, NULL); // and load signature from file to signature obj
	
	newFile = fopen(filename2.c_str(), "r");	
	deltaFile = fopen(deltaFilename.c_str(), "w+");

	rs_build_hash_table(sig);
	res = rs_delta_file(sig, newFile, deltaFile, NULL); // delta calculation of signature
	
	fclose(newFile);
	fclose(sigFile);
	fclose(deltaFile);

	srcFile = fopen(filename.c_str(), "r");
	deltaFile = fopen(deltaFilename.c_str(), "r");
	newFile = fopen(updFilename.c_str(), "w+");	

	res = rs_patch_file(srcFile, deltaFile, newFile, NULL);

	fclose(srcFile);
	fclose(newFile);
	fclose(deltaFile);
	
//	remove(sigFilename.c_str());
	remove(deltaFilename.c_str());
	remove(filename.c_str());
	rename(updFilename.c_str(), filename.c_str());
	
	return 0;
}
