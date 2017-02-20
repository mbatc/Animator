#include "FileHelper.h"
#include "MemoryHelper.h"
#include "StringHelper.h"
#include <string>

int FILEH::GetNextScanLength(FILE * pFile)
{
	fpos_t pos;
	fgetpos(pFile, &pos);

	char temp = '\0';
	bool validCheck = false;
	int bufLen = 0;
	while (temp != ' ' || !validCheck)
	{
		if (!feof(pFile))
		{
			fread(&temp, sizeof(char), 1, pFile);
			if (temp != ' ')
			{
				validCheck = true;
				bufLen++;
			}
			if (temp == '\0' || temp == '\n')
			{
				temp = ' ';
			}
		}
		else
		{
			break;
		}
	}
	if (validCheck)
		bufLen++;

	fsetpos(pFile, &pos);
	return bufLen;
}

int FILEH::GetStepsToEndOfLine(FILE * pFile)
{
	fpos_t pos;
	fgetpos(pFile, &pos);
	int bufLen = 0;
	bool ValidScan = false;
	char temp = '\0';

	while (!ValidScan || temp != '\n')
	{
		if (!feof(pFile))
		{
			fread(&temp, sizeof(char), 1, pFile);

			if (temp != ' ')
			{
				ValidScan = true;
			}

			if (ValidScan)
				bufLen++;
		}
		else
		{
			break;
		}
	}
	if (ValidScan)
		bufLen++;
	fsetpos(pFile, &pos);
	return bufLen;
}

char * FILEH::GetFileExtension(char * filename)
{
	if (!filename)
		return NULL;

	int len = strlen(filename);

	for (int i = len; i > 0; i--)
	{
		if (filename[i] == '.')
			return &filename[i];
	}

	return NULL;
}

void FILEH::LOAD::GetStringQuotation( FILE * pFile, char *& dst)
{
	if (!pFile)
		return;

	MEMORY::SafeDeleteArr(dst);
	if (feof(pFile))
		return;
	char temp = '\0';
	char quoteType = '\0';
	int dstLen = 0;

	fread(&temp,sizeof(char),1,pFile);
	while (temp != '"' && temp != '\'')
	{
		if (feof(pFile))
			return;

		fread(&temp, sizeof(char), 1, pFile);
	}
	quoteType = temp;
	do
	{
		if (!feof(pFile))
		{
			fread(&temp, sizeof(char), 1, pFile);
			MEMORY::LIST::AddToList(dst, &temp, dstLen);
		}
	} while (temp != quoteType);

	dst[dstLen - 1] = '\0';
}

void FILEH::LOAD::SafeScanString(FILE * pFile, char *& dst)
{
	if (!pFile)
		return;
	
	MEMORY::SafeDeleteArr(dst);
	dst = new char[GetNextScanLength(pFile)];
	if (!dst)
		return;

	fscanf(pFile, "%s", dst);
}
