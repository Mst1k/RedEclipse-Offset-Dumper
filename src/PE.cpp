#include "PE.h"

PE::PE(void * binaryBuff)
{
	printf("Parsing the Binary... ");
	this->imgDosHeader = (PIMAGE_DOS_HEADER)binaryBuff;
	if (this->imgDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		MessageBox(NULL, L"PE : Signature DOS not matching", NULL, NULL);
		exit(1);
	}

	this->imgNtHeader = (PIMAGE_NT_HEADERS)((uintptr_t)binaryBuff + this->imgDosHeader->e_lfanew - 1);
	if (this->imgNtHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		MessageBox(NULL, L"PE : Signature NT not matching", NULL, NULL);
		exit(1);
	}
	printf("Done! \n");
}

uintptr_t PE::RVAToFileOffset(uintptr_t dwRVA)
{
	int i;
	WORD wSections;
	PIMAGE_SECTION_HEADER pSectionHdr;

	pSectionHdr = IMAGE_FIRST_SECTION(this->imgNtHeader);
	wSections = this->imgNtHeader->FileHeader.NumberOfSections;

	for (i = 0; i < wSections; i++)
	{
		if (pSectionHdr->VirtualAddress <= dwRVA)
			if ((pSectionHdr->VirtualAddress + pSectionHdr->Misc.VirtualSize) > dwRVA)
			{
				dwRVA -= pSectionHdr->VirtualAddress;
				dwRVA += pSectionHdr->PointerToRawData;

				return (dwRVA);
			}

		pSectionHdr++;
	}

	return (-1);
}

uintptr_t PE::FileOffsetToRVA(uintptr_t dwOffset)
{
	int i;
	WORD wSections;
	PIMAGE_SECTION_HEADER pSectionHdr;

	pSectionHdr = IMAGE_FIRST_SECTION(this->imgNtHeader);
	wSections = this->imgNtHeader->FileHeader.NumberOfSections;

	for (i = 0; i < wSections; i++)
	{
		if (pSectionHdr->PointerToRawData <= dwOffset)
			if ((pSectionHdr->PointerToRawData + pSectionHdr->SizeOfRawData) > dwOffset)
			{
				dwOffset -= pSectionHdr->PointerToRawData;
				dwOffset += pSectionHdr->VirtualAddress;

				return (dwOffset);
			}

		pSectionHdr++;
	}

	return (-1);
}
