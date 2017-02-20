#include "CUIManager.h"
#include "Helper.h"
#include "SystemMessage.h"
#include "CSystem.h"

#include "CCompAnimEditor.h"

CUIManager::CUIManager()
{

}

CUIManager::~CUIManager()
{}


void CUIManager::SetSystem(CSystem* system)
{
	m_system = system;
}

void CUIManager::CheckInteraction()
{
	CheckFocus();
	for (int i = 0; i < m_nMenus; i++)
	{
		if(m_pMenuList[i]->isActive())
			m_pMenuList[i]->CheckControls(MOUSE::GetMouseX(), MOUSE::GetMouseY(), MOUSE::IsLeftMouseDown());
	}
}

CUIMenu * CUIManager::GetMenu(UINT menuID)
{
	for (int i = 0; i < m_nMenus; i++)
	{
		if (m_pMenuList[i]->GetID() == menuID)
			return m_pMenuList[i];
	}

	return NULL;
}

CUIMenu* CUIManager::AddMenu(CUIMenu * menu)
{
	if (!menu)
		return NULL;

	MEMORY::LIST::AddToListArr(m_pMenuList, menu, m_nMenus);

	return m_pMenuList[m_nMenus - 1];
}

CUIMenu* CUIManager::AddMenu(CD3DGraphics & gfx, UINT id, RECT r, bool show, bool update,
	bool background, D3DXVECTOR4 color)
{
	MEMORY::LIST::AddToListArr(m_pMenuList, new CUIMenu(), m_nMenus);
	int i = m_nMenus - 1;

	m_pMenuList[i]->Init(gfx, id, r, background, color);
	m_pMenuList[i]->ShowMenu(show);
	m_pMenuList[i]->UpdateWindow(update);

	return m_pMenuList[i];
}

void CUIManager::CheckFocus()
{
	RECT r;
	int mouse_x = MOUSE::GetMouseX();
	int mouse_y = MOUSE::GetMouseY();

	for (int i = 0; i < m_nMenus; i++)
	{
		if (m_pMenuList[i]->isActive())
		{
			r = m_pMenuList[i]->GetRect();
			if (MOUSE::IsInRegion(r))
			{
				m_pFocus = m_pMenuList[i];
				m_nFocus = i;
				return;
			}
		}
	}

	m_pFocus = NULL;
	m_nFocus = -1;
}

bool CUIManager::MessageProc(UINT msg, UINT subMsg)
{
	((CCompAnimEditor*)SYSTEM::GetGMData()->GetGameobject(0)->
		GetComponent(CANIMEDITOR))->UIMANAGER_MSG_PROC(msg,subMsg);
	return true;
}

void CUIManager::Update()
{
	CheckInteraction();
	for (int i = 0; i < m_nMenus; i++)
	{
		m_pMenuList[i]->Update();
	}
}

void CUIManager::Render(CD3DGraphics & gfx)
{
	if (m_pFocus)
	{
		char msg[256] = { 0 };
		sprintf(msg, "Focused Menu: %i", m_pFocus->GetID());

		SYSTEM::RenderText(msg, 0.42f, 0.2f);
	}

	for (int i = 0; i < m_nMenus; i++)
	{
		m_pMenuList[i]->Render(gfx);
	}
}

void CUIManager::SetFocus(UINT ID)
{
	m_pFocus = GetMenu(ID);
}

void CUIManager::SetFocus(CUIMenu * pMenu)
{
	for (int i = 0; i < m_nMenus; i++)
	{
		if (pMenu == m_pMenuList[i])
		{
			m_pFocus = m_pMenuList[i];
			return;
		}
	}
}

CUIMenu * CUIManager::GetFocus()
{
	return m_pFocus;
}