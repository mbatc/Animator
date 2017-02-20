#pragma once

#include "CUIMenu.h"
#include "CUIControl.h"

class CSystem;

class CUIManager
{
public:
	CUIManager();
	~CUIManager();

	void SetSystem(CSystem* system);

	bool MessageProc(UINT msg, UINT subMsg);
	void Update();
	void Render(CD3DGraphics& gfx);

	void SetFocus(UINT ID);
	void SetFocus(CUIMenu* pMenu);
	CUIMenu* GetFocus();

	void CheckInteraction();

	CUIMenu* GetMenu(UINT menuID);

	CUIMenu* AddMenu(CUIMenu* menu);
	CUIMenu* AddMenu(CD3DGraphics& gfx, UINT id, RECT r ,bool show = true, bool update = true, bool background = true, D3DXVECTOR4 color = D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f));
private:
	void CheckFocus();

	CUIMenu** m_pMenuList;
	int m_nMenus;

	int m_nFocus;
	CUIMenu* m_pFocus;

	CSystem* m_system;
};