#include "Dumpper.h"

uintptr_t Dumpper::GetFileSize()
{
	uintptr_t fSize = 0x0;

	LONG oldFilePos = ftell(f);
	fseek(f, 0, SEEK_END);
	fSize = ftell(f);
	fseek(f, oldFilePos, SEEK_SET);

	return fSize;
}

Dumpper::Dumpper(std::string _BinaryLocation)
{
	BinaryLocation = _BinaryLocation;
}

Dumpper::~Dumpper()
{
	fclose(f);
	delete pe;
	delete[] binaryBuff;
}

void Dumpper::Parse()
{
	printf("Loading the Binary... ");
	fopen_s(&f, BinaryLocation.c_str(), "r");
	if (!f)
	{
		MessageBox(NULL, L"Dumpper : Could not open the file", NULL, NULL);
		exit(1);
	}
	
	uintptr_t binarySize = this->GetFileSize();
	binaryBuff = new unsigned char[binarySize + 1];

	if (!fread(binaryBuff, 1, binarySize, f))
	{
		MessageBox(NULL, L"Dumpper : Could not read the file", NULL, NULL);
		exit(1);
	}
	printf("Done! \n");
	pe = new PE(binaryBuff);

	std::vector<unsigned char> Pat1 = { 0xe8 ,0xff ,0xff ,0xff ,0xff ,0x48 ,0x8d ,0x05 ,0xff ,0xff ,0xff ,0xff ,0x48 ,0x8d ,0x0d};
	std::string mask1 = "x????xxx????xxx";

	printf("Scanning for Offsets... ");
	for (uintptr_t i = 0; i < binarySize; i++)
	{
		if (Pat1[0] == binaryBuff[i])
		{
			bool match = true;
			for (int j = 0; j < Pat1.size(); j++)
			{
				if (Pat1[j] != binaryBuff[i + j] && mask1[j] != '?')
				{
					match = false;
					break;
				}
			}
			if (match)
				netvarFuncCandidates.push_back(i);
		}
	}
	printf("Done! \n");
}
