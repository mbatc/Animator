#include "CGameSystemData.h"
#include "CComponent.h"
#include "CSystem.h"
#include "CGameobject.h"

#include "CCompAnimEditor.h"

#include <iostream>

using namespace std;

CGameSystemData::CGameSystemData()
{
	m_pGameScene = new CGameScene();
}

CGameSystemData::~CGameSystemData()
{}

void CGameSystemData::InitData(char* pGameDataFile)
{
	int i = m_pGameScene->AddGameobject("Game Data\\defobj.gmobj");
	m_rig = m_pGameScene->GetGameobject(i);
	m_rig->GetTransform().GetTranslation() = CVector3(SYSTEM::GetWindowWidth() / 2, SYSTEM::GetWindowHeight() / 2,0.0f);
	m_rig->AddComponent(new CCompAnimEditor());
	m_rig->AddComponent(new CCompActorAnim("Game Data\\Actors\\Default\\"));
	m_rig->AddComponent(new CCompAnimController());
	CCompActorAnim* a = (CCompActorAnim*)m_rig->GetComponent(CACTORANIM);
	SYSTEM::AddGraphicsInit(a);
	
	CCompAnimController* c = (CCompAnimController*)m_rig->GetComponent(CANIMCONTROL);
	c->SetActor((CCompActorAnim*)m_rig->GetComponent(CACTORANIM));
	c->AddAnimation("Game Data\\Animations\\animation.anim", "edit");
	c->SetAnimation("edit");
	
	CCompAnimEditor* e = (CCompAnimEditor*)m_rig->GetComponent(CANIMEDITOR);
	e->SetActor((CCompActorAnim*)m_rig->GetComponent(CACTORANIM));
	e->SetController(c);
	e->InitUI();
}

CGameobject* CGameSystemData::GetGameobject(int index)
{
	return m_pGameScene->GetGameobject(index);
}

int CGameSystemData::GetNumberOfObjects()
{
	return m_pGameScene->GetNumberOfGameobjects();
}
int CGameSystemData::GetNumberOfLights()
{
	return m_pGameScene->GetNumberOfLights();
}