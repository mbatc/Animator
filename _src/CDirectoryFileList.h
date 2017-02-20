#pragma once

#include <cstdio>

class CDirectoryFileList
{
public:
	CDirectoryFileList();
	~CDirectoryFileList();

	void SetDirectory(char* directory);
	void SetFileExtension(char* extension);
	void FindFiles();

	char* GetFilename(int index);
	int   GetNumberOfFiles() { return m_nFiles; }
private:
	char*	m_pCurrentDirectory;
	char*	m_pFileType;

	int		m_nFiles;
	char**	m_ppFilename;
};