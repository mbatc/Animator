#pragma once

#include "CDirectoryFileList.h"

#define SRCFP "_src\\"

class CSourceAnalyser
{
public:
	CSourceAnalyser();
	
	void GetInfo();
	void PrintData();

private:
	int m_nLines;
	int m_nFiles;
	int m_totalSizeBytes;

	void GetFileData(char* filename);
	

	CDirectoryFileList m_fileList;
};