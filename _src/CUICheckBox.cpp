#include "CUICheckBox.h"
#include "CSystem.h"

CUICheckBox::CUICheckBox()
	:
	CUIControl()
{}

CUICheckBox::CUICheckBox(CUIControl* parent)
	:
	CUIControl(parent)
{}
CUICheckBox::CUICheckBox(CUIMenu* menu)
	:
	CUIControl(menu)
{}

void CUICheckBox::OnInit(RECT r, UINT id, char * active, char * inactive)
{
	CUIControl::OnInit(r, id, active, inactive);
	m_spr = CSystem::m_gfx.DEFAULTSPRITE;
	m_spr.Resize(6, 6);

	SetTextOffsetX(TEXTOFFSET::LEFT);
	SetTextOffsetY(TEXTOFFSET::CENTER);
}

void CUICheckBox::OnRender()
{
	m_spr.SetRectLock(GetBounds());
	m_spr.SetPosition(GetGlobalX(), GetGlobalY() + GetHeight() / 2 - 2);
	m_spr.Resize(8, 8);
	m_spr.SetColor(0.4f, 0.4f, 0.4f, 1.0f);
	SYSTEM::RenderSprite(m_spr, 8);

	if (m_state)
	{
		m_spr.Resize(6, 6);
		m_spr.SetPosition(m_spr.GetX() + 1, m_spr.GetY() + 1);
		m_spr.SetColor(0.8f, 0.8f, 0.8f, 1.0f);
		SYSTEM::RenderSprite(m_spr,8);
	}

	if(m_controlText)
		SYSTEM::RenderText(m_controlText, GetTextX() + 10, GetTextY(), 1.0f, &GetBounds());
}

void CUICheckBox::OnPressed()
{
	m_state = !m_state;
}

void CUICheckBox::SetState(bool s)
{
	m_state = s;
}

bool CUICheckBox::GetState()
{
	return m_state;
}