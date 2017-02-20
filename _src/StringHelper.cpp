#include <stdio.h>
#include <cstring>

#include "StringHelper.h"
#include "MemoryHelper.h"

char* STRING::Copy(char*& dest, const char* src)
{
	MEMORY::SafeDeleteArr(dest);
	int len = strlen(src);

	if(len == 0)
		return NULL;

	dest = new char[len + 1];
	strcpy_s(dest,sizeof(char)*(len+1),src);

	return dest;
}

void STRING::Append(char*& dest, const char* first, const char* second)
{
	char* t_first = NULL;
	char* t_second = NULL;

	STRING::Copy(t_first,first);
	STRING::Copy(t_second,second);
	
	int firstLen = strlen(first);
	int secondLen = strlen(second);

	MEMORY::SafeDeleteArr(dest);

	if(firstLen + secondLen == 0)
		return;

	dest = new char[firstLen + secondLen + 1];
	for(int i = 0; i < firstLen; i++)
	{
		dest[i] = t_first[i];
	}
	for(int i = 0; i < secondLen; i++)
	{
		dest[i + firstLen] = t_second[i];
	}
	dest[firstLen + secondLen] = '\0';
}

char * STRING::ScanFromEnd(char * src, char * match)
{
	int src_len = strlen(src);
	int match_len = strlen(match);

	char* temp = NULL;
	STRING::Copy(temp, src);

	for (int i = src_len; i >= 0; i--)
	{
		if (src[i] == match[0])
		{
			if (src_len - i < match_len)
				continue;

			if (strstr(&src[i], match) == &src[i])
				return &src[i];
		}
	}

	return nullptr;
}

char * STRING::ScanFromStart(char * src, char * match)
{
	return strstr(src, match);
}
