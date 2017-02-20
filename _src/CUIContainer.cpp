#include "CUIContainer.h"
#include "CSystem.h"

CUIContainer::CUIContainer()
	:
	CUIControl()
{}

CUIContainer::CUIContainer(CUIControl * parent)
	:
	CUIControl(parent)
{}

CUIContainer::CUIContainer(CUIMenu * menu)
	:
	CUIControl(menu)
{}

void CUIContainer::OnInit(RECT r, UINT id, char * active, char * inactive)
{
	r.left -= 8;
	r.top -= 8;
	r.right += 8;
	r.bottom += 8;

	m_bg.SetParent(this);
	m_bg.Clip(true);
	m_bg.InitControl(r, id, active, inactive);
	m_bg.SetBorderInner(true);
	m_bg.RenderBorder(true, true, false, true);
}

void CUIContainer::OnRender()
{
	m_bg.SetBorderWidth(m_borderWidth);
	m_bg.Render(CSystem::m_gfx);

	int text_x;
	int text_y;
	int text_width = 0;
	if(m_controlText)
		text_width = CSystem::g_textRenderer.GetWidthOfString(m_controlText);
	
	text_x = m_bg.GetGlobalX();
	text_y = m_bg.GetGlobalY() - CSystem::g_textRenderer.GetActiveFont().GetFontHeight()/2;
	
	CSprite spr = CSystem::m_gfx.DEFAULTSPRITE;

	spr.Resize(m_bg.GetWidth() - text_width, m_borderWidth);
	spr.SetPosition(m_bg.GetGlobalX() + text_width, m_bg.GetGlobalY());
	spr.SetColor(0.8f, 0.8f, 0.8f, 1.0f);

	SYSTEM::RenderText(m_controlText, text_x, text_y);
	SYSTEM::RenderSprite(spr,8);
}

void CUIContainer::OnActive()
{

}

void CUIContainer::OnUpdate()
{
	m_bg.Update();
}
