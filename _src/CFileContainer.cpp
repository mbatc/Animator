#include <cstring>

#include "CFileContainer.h"
#include "Helper.h"

CFileContainer::CFileContainer()
{
	m_pCurrentFile = NULL;
	m_pBaseDirectory = NULL;
	m_pFile = NULL;
}

CFileContainer::~CFileContainer()
{}

bool CFileContainer::SetFilename(const char* filename)
{
	if(!filename)
		return false;
	int len = strlen(filename);
	STRING::Copy(m_pCurrentFile,filename);

	if(!m_pCurrentFile)
		return false;

	char* temp = NULL;
	STRING::Copy(temp, filename);

	int dir_i;
	for( dir_i = len; dir_i >= 0 && temp[dir_i] != '/' && temp[dir_i] != '\\'; dir_i--);
	temp[dir_i + 1] = '\0';

	STRING::Copy(m_pBaseDirectory,temp);	

	cout << "CFileContainer: Current File Set To " << '"' << filename << '"' << endl;

	return true;
}

char* CFileContainer::GetFilename()
{
	return m_pCurrentFile;
}

char* CFileContainer::GetDirectory()
{
	return m_pBaseDirectory;
}

int CFileContainer::GetNextScanLength()
{
	fpos_t pos;
	fgetpos(m_pFile,&pos);

	char temp = '\0';
	bool validCheck = false;
	int bufLen = 0;
	while(temp != ' ' || !validCheck)
	{
		if(!feof(m_pFile))
		{
			fread(&temp,sizeof(char),1,m_pFile);
			if(temp != ' ')
			{
				validCheck = true;
				bufLen++;
			}
			if(temp == '\0' || temp == '\n')
			{
				temp = ' ';
			}
		}
		else
		{
			break;
		}
	}
	if(validCheck)
		bufLen++;

	fsetpos(m_pFile,&pos);
	return bufLen;
}

int CFileContainer::GetStepsToEndOfLine()
{
	fpos_t pos;
	fgetpos(m_pFile,&pos);
	int bufLen = 0;
	bool ValidScan = false;
	char temp = '\0';

	while(!ValidScan || temp != '\n')
	{
		if(!feof(m_pFile))
		{
			fread(&temp,sizeof(char),1,m_pFile);

			if(temp != ' ')
			{
				ValidScan = true;
			}

			if(ValidScan)
				bufLen++;
		}
		else
		{
			break;
		}
	}
	if(ValidScan)
		bufLen++;
	fsetpos(m_pFile,&pos);
	return bufLen;
}