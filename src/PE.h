#pragma once
#include <Windows.h>
#include <cstdint>
#include <stdio.h>

class PE
{
public:
	PE(void* binaryBuff);
	uintptr_t RVAToFileOffset(uintptr_t dwRVA);
	uintptr_t FileOffsetToRVA(uintptr_t dwOffset);

private:
	PIMAGE_DOS_HEADER imgDosHeader;
	PIMAGE_NT_HEADERS imgNtHeader;
};