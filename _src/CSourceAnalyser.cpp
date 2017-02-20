#include "CSourceAnalyser.h"
#include "Helper.h"

CSourceAnalyser::CSourceAnalyser()
{
	char* temp = 0;
	STRING::Append(temp, SRCFP, "*");
	m_fileList.SetDirectory(temp);
	MEMORY::SafeDeleteArr(temp);
}

void CSourceAnalyser::GetInfo()
{
	m_fileList.FindFiles();
	m_nFiles = m_fileList.GetNumberOfFiles();

	for (int i = 0; i < m_nFiles; i++)
	{
		char* temp = 0;
		STRING::Append(temp,SRCFP,m_fileList.GetFilename(i));
		GetFileData(temp);
		MEMORY::SafeDeleteArr(temp);
	}
}

void CSourceAnalyser::PrintData()
{
	cout << "\n\n-- Source Code Analysis --\n"
		<< "\tNumber of Files: " << m_nFiles << "\n"
		<< "\tTotal Size of Files: " << m_totalSizeBytes << " bytes\n"
		<< "\t                   : " << ((float)m_totalSizeBytes) / 1024.0f << " kilobytes\n"
		<< "\t                   : " << ((float)m_totalSizeBytes) / 1024.0f / 1024.0f << " gigabytes\n"
		<< "\tTotal Lines of Code: " << m_nLines << "\n"
		<< "___________________________\n\n";

}

void CSourceAnalyser::GetFileData(char* filename)
{
	if (!filename)
		return;

	FILE* pFile = fopen(filename,"r");
	if (!pFile)
		return;

	m_nLines++;
	do
	{
		if (!feof(pFile))
		{
			char c = fgetc(pFile);
			if (c == '\n')
				m_nLines++;

			m_totalSizeBytes += sizeof(char);
		}
	} while (!feof(pFile));

	fclose(pFile);
}