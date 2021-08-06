#pragma once

#include <vector>
#include <string>
#include "PE.h"

class Dumpper
{
private:
	std::string BinaryLocation;
	FILE* f;

public:
	Dumpper(std::string _BinaryLocation);
	~Dumpper();
	void Parse();

protected:
	std::vector<uintptr_t> netvarFuncCandidates;
	unsigned char* binaryBuff;
	PE* pe;
	uintptr_t GetFileSize();
};

