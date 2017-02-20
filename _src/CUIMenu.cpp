#include "CUIMenu.h"
#include "Helper.h"

CUIMenu::CUIMenu()
	:
	m_pControlList(NULL),
	m_controlIDList(NULL),
	m_nControls(0),
	m_bDrawbackground(true),
	m_menuID(-1)
{}

CUIMenu::~CUIMenu()
{}

void CUIMenu::Init(CD3DGraphics& gfx, UINT id, RECT menuRect, bool drawBackground, D3DXVECTOR4 bgColor)
{
	m_menuRect = menuRect;
	m_bDrawbackground = drawBackground;
	m_bgColor = bgColor;

	CSurface temp;
	D3DXVECTOR4 pixels[16];
	for (int i = 0; i < 16; i++)
	{
		pixels[i] = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	temp.Init(pixels,4,4);
	m_bg.Init(temp, gfx);
	m_bg.Resize(m_menuRect.right - m_menuRect.left, m_menuRect.bottom - m_menuRect.top);

	m_bg.SetPosition(m_menuRect.left, m_menuRect.top);
	m_bg.SetColor(m_bgColor);

	m_menuID = id;
}

void CUIMenu::ShowMenu(bool showWindow)
{
	m_bShowMenu = showWindow;
}

void CUIMenu::UpdateWindow(bool updateWindow)
{
	m_bUpdateMenu = updateWindow;
}

void CUIMenu::DrawBackground(bool drawBackground)
{
	m_bDrawbackground = drawBackground;
}

void CUIMenu::CheckControls(int x, int y, bool mouseDown)
{
	for (int i = 0; i < m_nControls; i++)
	{
		m_pControlList[i]->CheckIfActive(x, y, mouseDown);
	}
}

bool CUIMenu::MenuProc(UINT msg, UINT subMsg)
{
	return false;
}

void CUIMenu::Update()
{
	if (!m_bUpdateMenu)
		return;

	m_bg.SetPosition(m_menuRect.left, m_menuRect.top);
	m_bg.SetColor(m_bgColor);


	if (!m_pControlList)
		return;

	for (int i = 0; i < m_nControls; i++)
	{
		if (!m_pControlList[i])
			continue;
		if (m_pControlList[i]->GetParent() == NULL)
		{
			m_pControlList[i]->m_translatedPosX = m_pControlList[i]->m_xPos + m_menuRect.left;
			m_pControlList[i]->m_translatedPosY = m_pControlList[i]->m_yPos + m_menuRect.top;
		}
		m_pControlList[i]->Update();
	}
}

void CUIMenu::Render(CD3DGraphics & gfx)
{
	if (!m_bShowMenu)
		return;

	m_bg.SetColor(m_bgColor);
	SYSTEM::RenderSprite(m_bg, 8);


	if (!m_pControlList)
		return;
	for (int i = 0; i < m_nControls; i++)
	{
		if (!m_pControlList[i])
			continue;

		if (m_pControlList[i]->m_pMenu)
			m_pControlList[i]->SetRenderBoundary(m_menuRect);
		else if (m_pControlList[i]->m_pParent)
			m_pControlList[i]->SetRenderBoundary(m_pControlList[i]->GetParent()->GetRect());
		m_pControlList[i]->Render(gfx);
	}
}

void CUIMenu::ResizeMenu(RECT r, bool moveControls)
{
	if (moveControls)
	{
		float xScl = (r.right - r.left) / (m_menuRect.right - m_menuRect.left);
		float yScl = (r.bottom - r.top) / (m_menuRect.bottom - m_menuRect.top);

		for (int i = 0; i < m_nControls; i++)
		{
			m_pControlList[i]->m_xPos *= xScl;
			m_pControlList[i]->m_yPos *= yScl;
		}
	}
	
	m_menuRect = r;
	m_bg.Resize(r.right - r.left, r.bottom - r.top);
}

bool CUIMenu::AddControl(CUIControl* control, UINT ID)
{
	if (DoesIDExist(ID))
	{
		cout << "Error Adding Menu Item: control with ID '" << ID << "' already exists...\n";
		return false;
	}

	control->SetID(ID);
	MEMORY::LIST::AddToListArr(m_pControlList, control, m_nControls);
	m_nControls--;
	MEMORY::LIST::AddToList(m_controlIDList, &ID, m_nControls);
	return true;
}

CUIControl * CUIMenu::AddControl(UINT ID, RECT r, CUIControl * control, char* active, char* inactive)
{
	if (DoesIDExist(ID))
	{
		cout << "Error Adding Menu Item: control with ID '" << ID << "' already exists...\n";
		return NULL;
	}
	control->SetMenu(this);
	MEMORY::LIST::AddToListArr(m_pControlList, control, m_nControls);
	m_nControls--;
	MEMORY::LIST::AddToList(m_controlIDList, &ID, m_nControls);
	m_pControlList[m_nControls - 1]->InitControl(r, ID, active, inactive);
	return m_pControlList[m_nControls-1];
}

CUIControl * CUIMenu::GetControl(UINT id)
{
	for (int i = 0; i < m_nControls; i++)
	{
		if (m_pControlList[i]->GetID() == id)
			return m_pControlList[i];
	}
	return NULL;
}

CUIControl * CUIMenu::GetControl(int index)
{
	if (!m_pControlList) return NULL;

	if (index < 0 || index >= m_nControls)
		return nullptr;
	return m_pControlList[index];
}

bool CUIMenu::DoesIDExist(UINT ID)
{
	for (int i = 0; i < m_nControls; i++)
	{
		if (ID == m_controlIDList[i])
			return true;
	}
	return false;
}
