#include "librsfunc.hpp"

int makeSignature(std::string filename, std::string sigFilename) {
	FILE * srcFile = fopen(filename.c_str(), "r"); 
	FILE * sigFile = fopen(sigFilename.c_str(), "w+");

	rs_job_t * sig_job;
	rs_sig_begin((size_t) RS_DEFAULT_BLOCK_LEN, (size_t) RS_DEFAULT_STRONG_LEN);

	rs_result res = rs_sig_file(srcFile, sigFile, RS_DEFAULT_BLOCK_LEN, RS_DEFAULT_STRONG_LEN, NULL); // creating signature

	fclose(srcFile);	
	fclose(sigFile);	

	return 0;
}

int makeDelta(std::string sigFilename, std::string filename2, std::string deltaFilename ) {
	FILE * sigFile = fopen(sigFilename.c_str(), "r");
	FILE * newFile = fopen(filename2.c_str(), "r");	
	FILE * deltaFile = fopen(deltaFilename.c_str(), "w+");

	rs_signature_t * sig; // signature object
	rs_loadsig_file(sigFile, &sig, NULL); // and load signature from file to signature obj
	rs_build_hash_table(sig);
	rs_delta_file(sig, newFile, deltaFile, NULL); // delta calculation of signature
	

	fclose(newFile);
	fclose(sigFile);
	fclose(deltaFile);
	return 0;
}

int makePatch(std::string filename, std::string deltaFilename, std::string updFilename){
	FILE * srcFile = fopen(filename.c_str(), "r");
	FILE * deltaFile = fopen(deltaFilename.c_str(), "r");
	FILE * newFile = fopen(updFilename.c_str(), "w+");	

	rs_patch_file(srcFile, deltaFile, newFile, NULL);

	fclose(srcFile);
	fclose(newFile);
	fclose(deltaFile);
	return 0;
}


