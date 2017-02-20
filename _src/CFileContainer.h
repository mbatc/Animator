#pragma once

#ifndef UNKNOWN
#define UNKNOWN -1
#endif

#include <cstdio>

class CFileContainer
{
public:
	CFileContainer();
	~CFileContainer();

	bool SetFilename(const char* filename);
	char* GetFilename();
	char* GetDirectory();

protected:	
	int GetNextScanLength();
	int GetStepsToEndOfLine();

	char* m_pBaseDirectory;
	char* m_pCurrentFile;
	
	FILE* m_pFile;
};