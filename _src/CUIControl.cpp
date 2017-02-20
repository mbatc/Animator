#include "CUIControl.h"
#include "Helper.h"
#include "SystemMessage.h"
#include "CSystem.h"

const char* CORNER[] = {
	"_bl",
	"_tl",
	"_tr",
	"_br"
};

const char* EDGE[] = {
	"_b",
	"_l",
	"_t",
	"_r"
};


CUIControl::CUIControl()
	:
	m_controlText(NULL),
	m_pMenu(NULL),
	m_pParent(NULL),
	m_bInnerBorder(false),
	m_bRenderBorder(false)
{}

CUIControl::CUIControl(CUIMenu * pMenu)
	:
	m_controlText(NULL),
	m_pMenu(pMenu),
	m_pParent(NULL),
	m_bInnerBorder(false),
	m_bRenderBorder(false)
{}

CUIControl::CUIControl(CUIControl* pParent)
	:
	m_controlText(NULL),
	m_pMenu(NULL),
	m_pParent(pParent),
	m_bInnerBorder(false),
	m_bRenderBorder(false)
{}

CUIControl::~CUIControl()
{
	MEMORY::SafeDeleteArr(m_controlText);
}

void CUIControl::InitControl(RECT r, UINT id, char* active, char* inactive)
{
	m_borderWidth = 8;
	m_clip = false;

	SetPosition(r);
	m_buttonActiveID = id;

	SetTextOffsetX(TEXTOFFSET::LEFT);
	SetTextOffsetY(TEXTOFFSET::CENTER);
	SetBorderSprite();
	SetBorderColour(0.8f, 0.8f, 0.8f, 1.0f);

	OnInit(r,id,active,inactive);
}

UINT CUIControl::GetID()
{
	return m_buttonActiveID;
}

void CUIControl::CheckIfActive(int mouseX, int mouseY, bool buttonDown)
{
	if (mouseX > m_translatedPosX &&
		mouseX < m_activeRect.right - m_activeRect.left + m_translatedPosX &&
		mouseY > m_translatedPosY &&
		mouseY < m_activeRect.bottom - m_activeRect.top + m_translatedPosY)
	{
		if (buttonDown)
		{
			if (m_bIsActive == false)
			{
				OnPressed();
				m_bIsActive = true;
				SYSTEM::AddMessage(m_buttonActiveID, _SM_SUB_BT_PRESSED,true);
			}
			OnActive();
		}
		else
		{
			if (m_bIsActive == true)
			{
				OnRelease();
				m_bIsActive = false;
				SYSTEM::AddMessage(m_buttonActiveID, _SM_SUB_BT_RELEASED,true);
			}
			OnHover();
			SYSTEM::AddMessage(m_buttonActiveID, _SM_SUB_BT_HOVER);
		}
	}
	else
	{
		if (buttonDown)
			OnClickAway();
	}

	if(!buttonDown)
		m_bIsActive = buttonDown;
}

void CUIControl::Update()
{
	if (m_pParent)
	{
		m_translatedPosX = m_pParent->GetGlobalX() + m_xPos;
		m_translatedPosY = m_pParent->GetGlobalY() + m_yPos;
	}
	m_spriteStateActive.SetPosition(m_translatedPosX, m_translatedPosY);
	m_spriteStateInactive.SetPosition(m_translatedPosX, m_translatedPosY);
	SetPosition(m_activeRect);
	UpdateBounds();
	OnUpdate();
}

void CUIControl::Render(CD3DGraphics & gfx)
{
	OnRender();
	if (m_bRenderBorder)
	{
		RenderBorder();
	}
}

void CUIControl::SetPosition(RECT r)
{
	m_activeRect = r;

	m_xPos = m_activeRect.left;
	m_yPos = m_activeRect.top;

	m_spriteStateActive.Resize(m_activeRect.right - m_activeRect.left, m_activeRect.bottom - m_activeRect.top);
	m_spriteStateInactive.Resize(m_activeRect.right - m_activeRect.left, m_activeRect.bottom - m_activeRect.top);
}

void CUIControl::SetText(char * text)
{
	STRING::Copy(m_controlText, text);
}

void CUIControl::SetMenu(CUIMenu * pMenu)
{
	m_pMenu = pMenu;
}

void CUIControl::SetParent(CUIControl * pParent)
{
	m_pParent = pParent;
}

void CUIControl::SetBorderColour(float r, float g, float b, float a)
{
	SetBorderColour(D3DXVECTOR4(r, g, b, a));
}

void CUIControl::SetBorderColour(D3DXVECTOR4 color)
{
	for (int i = 0; i < 4; i++)
	{
		m_bdrCorner[i].SetColor(color);
		m_bdrEdge[i].SetColor(color);
	}
}

void CUIControl::SetBorderSprite()
{
	for (int i = 0; i < 4; i++)
	{
		m_bdrCorner[i] = CSystem::m_gfx.DEFAULTSPRITE;
		m_bdrEdge[i] = CSystem::m_gfx.DEFAULTSPRITE;
	}
}

void CUIControl::SetBorderSprite(CSprite* corner, CSprite* edge)
{
	for (int i = 0; i < 4; i++)
	{
		m_bdrCorner[i] = corner[i];
		m_bdrEdge[i] = edge[i];
	}
}

void CUIControl::SetBorderSprite(char * corner, char * edge)
{
	char* cfile = 0;
	char* efile = 0;
	for (int i = 0; i < 4; i++)
	{
		STRING::Append(cfile, corner, CORNER[i]);
		STRING::Append(cfile, cfile, ".png");
		STRING::Append(efile, edge, EDGE[i]);
		STRING::Append(efile, efile, ".png");
		m_bdrCorner[i].Init(cfile, CSystem::m_gfx);
		m_bdrEdge[i].Init(efile, CSystem::m_gfx);
	}
}

void CUIControl::SetActiveSprite(char* filename)
{
	m_spriteStateActive.Init(filename, CSystem::m_gfx);
}

void CUIControl::SetInactiveSprite(char* filename)
{
	m_spriteStateInactive.Init(filename, CSystem::m_gfx);
}

void CUIControl::SetID(UINT ID)
{
	m_buttonActiveID = ID;
}

void CUIControl::RenderBorder(bool b)
{
	m_bRenderBorder = b;
}

void CUIControl::RenderBorder(bool left, bool right, bool top, bool bottom)
{
	m_bRenderBorder = (left || right || top || bottom);
	m_bdrRender[0] = bottom;
	m_bdrRender[1] = left;
	m_bdrRender[2] = top;
	m_bdrRender[3] = right;
}

void CUIControl::SetBorderInner(bool b)
{
	m_bInnerBorder = b;
	for (int i = 0; i < 4; i++)
	{
		m_bdrRender[i] = true;
	}
}

void CUIControl::RenderText()
{
	RECT t;
	if (m_clip)
	{
		t.bottom = m_activeRect.bottom - m_activeRect.top + m_translatedPosY;
		t.top = m_translatedPosY;
		t.left = m_translatedPosX;
		t.right = m_activeRect.right - m_activeRect.left + m_translatedPosX;
	}
	else
	{
		t.bottom = t.top = t.right = t.left = -1;
	}

	int x = GetTextX();
	int y = GetTextY();

	if (m_bIsActive)
	{
		SYSTEM::SetTextColor(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
		SYSTEM::RenderText(m_controlText, x, y, 1.0f, &t);
		SYSTEM::SetTextColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		SYSTEM::SetTextColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
		SYSTEM::RenderText(m_controlText, x, y, 1.0f, &t);
		SYSTEM::SetTextColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void CUIControl::OnPressed()
{
}

void CUIControl::OnActive()
{

}

void CUIControl::OnHover()
{
}

void CUIControl::OnRelease()
{
}

void CUIControl::OnUpdate()
{
}

void CUIControl::OnRender()
{
	RECT t;
	if (m_clip)
	{
		t.bottom = m_activeRect.bottom - m_activeRect.top + m_translatedPosY;
		t.top = m_translatedPosY;
		t.left = m_translatedPosX;
		t.right = m_activeRect.right - m_activeRect.left + m_translatedPosX;
	}
	else
	{
		t.bottom = t.top = t.right = t.bottom = -1;
	}
	
	if (m_bIsActive)
	{
		SYSTEM::SetTextColor(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
		SYSTEM::RenderSprite(m_spriteStateActive, 8);
		SYSTEM::RenderText(m_controlText, GetTextX(), GetTextY(), 1.0f, &t);
		SYSTEM::SetTextColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		SYSTEM::SetTextColor(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
		SYSTEM::RenderSprite(m_spriteStateInactive, 8);
		SYSTEM::RenderText(m_controlText, GetTextX(), GetTextY(), 1.0f, &t);
		SYSTEM::SetTextColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void CUIControl::OnClickAway()
{
}

void CUIControl::OnInit(RECT r, UINT id, char* active, char* inactive)
{
	if (active)
	{
		m_spriteStateActive.Init(active, CSystem::m_gfx);
	}
	else
	{
		SetActiveSprite(CSystem::m_gfx.DEFAULTSPRITE);
		GetSprite(true).SetColor(1.0f, 1.0f, 1.0f, 0.3f);
	}

	if (inactive)
	{
		m_spriteStateInactive.Init(inactive, CSystem::m_gfx);
	}
	else
	{
		SetInactiveSprite(CSystem::m_gfx.DEFAULTSPRITE);
		GetSprite(false).SetColor(1.0f, 1.0f, 1.0f, 0.6f);
	}
}

void CUIControl::RenderBorder()
{
	RECT r;
	r.right = m_translatedPosX + GetWidth();
	r.left = m_translatedPosX;
	r.top = m_translatedPosY;
	r.bottom = m_translatedPosY + GetHeight();

	CUIMenu* pMenu = GetMenu();
	CUIControl* pControl = 0;
	do
	{
		pControl = GetParent();
		if (!pControl)
			break;

		if (pControl->GetMenu())
			pMenu = pControl->GetMenu();
	} while (pMenu == 0 && pControl != 0);
	if (pMenu)
	{
		if (m_clip)
		{
			for (int i = 0; i < 4; i++)
			{
				m_bdrCorner[i].SetRectLock(pMenu->GetRect());
				m_bdrEdge[i].SetRectLock(pMenu->GetRect());
			}
		}
	}

	if (m_bInnerBorder)
	{
		m_bdrCorner[0].SetPosition(r.left, r.bottom-m_borderWidth);
		m_bdrCorner[1].SetPosition(r.left, r.top);
		m_bdrCorner[2].SetPosition(r.right-m_borderWidth, r.top);
		m_bdrCorner[3].SetPosition(r.right-m_borderWidth, r.bottom-m_borderWidth);

		m_bdrEdge[0].SetPosition(r.left, r.bottom-m_borderWidth);
		m_bdrEdge[1].SetPosition(r.left, r.top);
		m_bdrEdge[2].SetPosition(r.left, r.top);
		m_bdrEdge[3].SetPosition(r.right-m_borderWidth, r.top);
	}
	else
	{
		m_bdrCorner[0].SetPosition(r.left-m_borderWidth, r.bottom);
		m_bdrCorner[1].SetPosition(r.left-m_borderWidth, r.top-m_borderWidth);
		m_bdrCorner[2].SetPosition(r.right, r.top - m_borderWidth);
		m_bdrCorner[3].SetPosition(r.right, r.bottom);

		m_bdrEdge[0].SetPosition(r.left, r.bottom);
		m_bdrEdge[1].SetPosition(r.left - m_borderWidth, r.top);
		m_bdrEdge[2].SetPosition(r.left, r.top - m_borderWidth);
		m_bdrEdge[3].SetPosition(r.right, r.top);
	}

	m_bdrEdge[0].Resize(GetWidth(), m_borderWidth);
	m_bdrEdge[1].Resize(m_borderWidth, GetHeight());
	m_bdrEdge[2].Resize(GetWidth(), m_borderWidth);
	m_bdrEdge[3].Resize(m_borderWidth, GetHeight());

	for (int i = 0; i < 4; i++)
	{
		if(m_bdrRender[i])
			SYSTEM::RenderSprite(m_bdrEdge[i], 8);
	}

	bool rendercorners[4] = {
		(m_bdrRender[0] && m_bdrRender[1]),
		(m_bdrRender[1] && m_bdrRender[2]),
		(m_bdrRender[2] && m_bdrRender[3]),
		(m_bdrRender[3] && m_bdrRender[0])
	};

	for (int i = 0; i < 4; i++)
	{
		m_bdrCorner[i].Resize(m_borderWidth, m_borderWidth);
		if (rendercorners[i])
			SYSTEM::RenderSprite(m_bdrCorner[i], 8);
	}
}

void CUIControl::UpdateBounds()
{
	m_bounds.left = GetGlobalX();
	m_bounds.right = GetGlobalX() + GetWidth();
	m_bounds.top = GetGlobalY();
	m_bounds.bottom = GetGlobalY() + GetHeight();

	RECT b;
	if (m_pParent)
	{
		b = m_pParent->GetBounds();
	}
	else if (m_pMenu)
	{
		b = m_pMenu->GetRect();
	}
	else
	{
		m_bounds = m_activeRect;
		return;
	}

	if (b.left > GetGlobalX())
		m_bounds.left = b.left;
	if (b.right < GetGlobalX() + GetWidth())
		m_bounds.right = b.right;
	if (b.top > GetGlobalY())
		m_bounds.top = b.top;
	if (b.bottom < GetGlobalY() + GetHeight())
		m_bounds.bottom = b.bottom;
}

int CUIControl::GetTextX()
{
	int x = GetGlobalX() + 2;

	switch (m_text_offset_x)
	{
	case TEXTOFFSET::LEFT:
		if (m_bInnerBorder && m_bRenderBorder)
		{
			x += m_borderWidth;
		}
		break;
	case TEXTOFFSET::CENTER:
		if (m_controlText)
		{
			int text_width = CSystem::g_textRenderer.GetWidthOfString(m_controlText,1.0f);
			x += ((GetWidth() - text_width) / 2) - 2;
		}
		break;
	case TEXTOFFSET::RIGHT:
		if (m_controlText)
		{
			int text_width = CSystem::g_textRenderer.GetWidthOfString(m_controlText,1.0f);
			x += (GetWidth() - text_width) - 4;
		}
	}
	return x;
}

int CUIControl::GetTextY()
{
	int y = GetGlobalY();
	switch (m_text_offset_y)
	{
	case TEXTOFFSET::TOP:
		if (m_bRenderBorder && m_bInnerBorder)
		{
			y += m_borderWidth + 2;
		}
		break;
	case TEXTOFFSET::CENTER:
		if (m_controlText)
		{
			y += ((GetHeight() - CSystem::g_textRenderer.GetHeightOfString(m_controlText)) / 2);
		}
		break;
	case TEXTOFFSET::BOTTOM:
		if (m_controlText)
		{
			y += (GetHeight() - CSystem::g_textRenderer.GetHeightOfString(m_controlText)) - 4;
		}
	}
	return y;
}

void CUIControl::SetRenderBoundary(RECT r)
{
	m_spriteStateActive.SetRectLock(r);
	m_spriteStateInactive.SetRectLock(r);
}