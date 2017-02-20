#include "CGameSystemLogic.h"
#include "CSystem.h"
#include "Helper.h"

CGameSystemLogic::CGameSystemLogic(CGameSystemData& gameData)
	:
	m_gameData(gameData)
{}

CGameSystemLogic::~CGameSystemLogic()
{}

bool CGameSystemLogic::doFrame(float deltaTime)
{
	return m_gameData.m_pGameScene->Update(deltaTime);
}