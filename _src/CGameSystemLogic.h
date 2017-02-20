#pragma once

#include "CGameSystemData.h"

class CGameSystemLogic
{
public:
	CGameSystemLogic(CGameSystemData& m_gameData);
	~CGameSystemLogic();

	bool doFrame(float deltaTime);
private:
	CGameSystemData& m_gameData;
};