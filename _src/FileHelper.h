#pragma once
#include <cstdio>

namespace FILEH
{
	int GetNextScanLength(FILE* pFile);
	int GetStepsToEndOfLine(FILE* pFile);
	char* GetFileExtension(char* filename);

	namespace LOAD
	{
		void GetStringQuotation(FILE* pFile, char*& dst);
		void SafeScanString(FILE* pFile, char*& dst);
	}
}