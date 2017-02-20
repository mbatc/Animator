#pragma once

#include "CGameobject.h"
#include "CGameScene.h"

class CGameSystemLogic;
class CGameSystemRender;
class CLevelEditor;

class CGameSystemData
{
	friend CGameSystemLogic;
	friend CGameSystemRender;
	friend CLevelEditor;

public:
	CGameSystemData();
	~CGameSystemData();

	void InitData(char* pGameDataFile);

	CGameobject* GetGameobject(int index);

	CGameScene* GetScene() { return m_pGameScene; }

	int GetNumberOfObjects();
	int GetNumberOfLights();

	CGameobject*	m_rig;
	CGameScene*		m_pGameScene;
	CLevelEditor*	m_pLevelEditor;
};