#include "CDirectoryFileList.h"
#include "Helper.h"

#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>

CDirectoryFileList::CDirectoryFileList()
	:
	m_pCurrentDirectory(NULL),
	m_ppFilename(NULL)
{
	m_nFiles = 0;
	m_pFileType = NULL;
}

CDirectoryFileList::~CDirectoryFileList()
{
	MEMORY::SafeDeleteArr(m_pCurrentDirectory);
	for (int i = 0; i < m_nFiles; i++)
	{
		MEMORY::SafeDeleteArr(m_ppFilename[i]);
	}

	MEMORY::SafeDeleteArr(m_ppFilename);
}

void CDirectoryFileList::SetDirectory(char* directory)
{
	STRING::Copy(m_pCurrentDirectory, directory);
}

void CDirectoryFileList::SetFileExtension(char* extension)
{
	STRING::Copy(m_pFileType, extension);
}

void CDirectoryFileList::FindFiles()
{
	WIN32_FIND_DATA ffd;
	HANDLE fileHandle = INVALID_HANDLE_VALUE;
	DWORD dwError;

	bool checkFileType = (bool)(m_pFileType);

	if (!m_pCurrentDirectory)
		return;

	cout << "CDirectoryFileList: Finding Files in directory '" << m_pCurrentDirectory << "'...\n";

	fileHandle = FindFirstFile(m_pCurrentDirectory, &ffd); 
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		cout << "No Files In Directory...\n";
	}

	do
	{
		if (ffd.cFileName)
		{
			char* temp = NULL;
			STRING::Copy(temp, ffd.cFileName);

			if (m_pFileType)
			{
				char* ext = FILEH::GetFileExtension(ffd.cFileName);

				if (!ext || strcmp(ext, m_pFileType))
					continue;
			}

			cout << ffd.cFileName << endl;
			MEMORY::LIST::AddToListArr(m_ppFilename, temp, m_nFiles);
		}
	} while (FindNextFile(fileHandle, &ffd) != 0);

	dwError = GetLastError();

	if (dwError != ERROR_NO_MORE_FILES)
	{
		cout << "CDirectoryFileList: Something Went Wrong...\n";
	}

	FindClose(fileHandle);

	if (m_nFiles > 0)
	{
		cout << m_nFiles << " Files found in directory: " << m_pCurrentDirectory << endl;
	}
}

char* CDirectoryFileList::GetFilename(int index)
{
	if (m_ppFilename)
	{
		if (m_ppFilename[index])
		{
			return m_ppFilename[index];
		}
	}

	return NULL;
}
