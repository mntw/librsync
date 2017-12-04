#include <cstdio>
#include <librsync.h>
#include <iostream>
#include <string.h>



int makeSignature(std::string filename, std::string sigFilename);
int makeDelta(std::string sigFilename, std::string filename2, std::string deltaFilename );
int makePatch(std::string filename, std::string deltaFilename, std::string updFilename);
