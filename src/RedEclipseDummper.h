#pragma once
#include "Dumpper.h"

struct Offset{
	std::string offName; // validxint
	uintptr_t offset; // 70E580
};

class RedEclipseDummper :
	public Dumpper
{
private:
	std::vector<Offset> offsets;
public:
	RedEclipseDummper(std::string _BinaryLocation);
	void DumpOffsets();
	
};

void LogFinal();

