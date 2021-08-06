#include "RedEclipseDummper.h"
#include <fstream>

bool containsOnlyASCII(const std::string& filePath) {
	for (auto c : filePath) {
		if (static_cast<unsigned char>(c) > 127) {
			return false;
		}
	}
	return true;
}

RedEclipseDummper::RedEclipseDummper(std::string _BinaryLocation) : Dumpper(_BinaryLocation)
{

}

void RedEclipseDummper::DumpOffsets()
{
	printf("Parsing Offsets... ");
	for (int i = 0; i < netvarFuncCandidates.size(); i++)
	{
		unsigned char* currCandidate = (unsigned char*)&binaryBuff[netvarFuncCandidates[i]];

		uint32_t* rel_off = (uint32_t*)(currCandidate + 8);
		uint32_t  rel_off_off_rva = pe->FileOffsetToRVA(netvarFuncCandidates[i] + 8);
		uint32_t  rel_off_rva = rel_off_off_rva + 4 + *rel_off;
		uint32_t  rel_off_raw = pe->RVAToFileOffset(rel_off_rva);

		uint32_t* rel_name_off = (uint32_t*)(currCandidate + 15);
		uint32_t  rel_name_off_rva = pe->FileOffsetToRVA(netvarFuncCandidates[i] + 15);
		uint32_t  rel_name_rva = rel_name_off_rva + 4 + *rel_name_off;
		uint32_t  rel_name_raw =  pe->RVAToFileOffset(rel_name_rva);

		char* offName_c = (char*)&binaryBuff[rel_name_raw];
		std::string offName_str(offName_c);

		Offset o;
		o.offName = offName_str;
		o.offset = rel_off_rva;

		if(containsOnlyASCII(offName_str))
			offsets.push_back(o);
	}
	printf("Done! \n");

	printf("Dummping the Offsets... ");
	std::ofstream dumpFile("dump.hpp");
	if (dumpFile.fail())
	{
		MessageBox(NULL, L"RedEclipseDummper : Could not create the dump file", NULL, NULL);
		exit(1);
	}
	dumpFile << "namespace RedEclipseDummper { \n\n";
	for (int i = 0; i < offsets.size(); i++)
	{
		char Line[512];
		sprintf_s(Line, 512, "	uintptr_t %s = 0x%08X;\n", offsets[i].offName.c_str(), (unsigned int)offsets[i].offset);
		dumpFile << Line;
	}
	dumpFile << "\n}";
	dumpFile.close();
	printf("Done! \n");
	
}
void LogFinal()
{
	printf("By Mstik\n");
	printf("Credits ==> guidedhacking.com\n");
	system("pause");
}
