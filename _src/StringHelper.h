#pragma once

namespace STRING
{
	char* Copy(char*& dest, const char* src);
	void Append(char*& dest, const char* first, const char* second);

	//returns a pointer to the first appearance of 'match' from the end of the string
	//NULL if it does not appear
	char* ScanFromEnd(char* src, char* match);

	//returns a pointer to the first appearance of 'match' from the start of the string
	//NULL if it does no appear
	char* ScanFromStart(char* src, char* match);


}