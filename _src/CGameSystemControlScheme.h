#pragma once
#include <Windows.h>

class CGameSystemControlScheme
{
public:
	CGameSystemControlScheme()
	{
		playerForward = 'W';
		playerBackward = 'S';
		playerLeft = 'A';
		playerRight = 'D';
	}
	~CGameSystemControlScheme() {}

	UINT playerForward;
	UINT playerBackward;
	UINT playerLeft;
	UINT playerRight;
};