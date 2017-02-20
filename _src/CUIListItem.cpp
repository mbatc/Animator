#include "CUIListItem.h"
#include "Helper.h"
#include "CUIList.h"
#include "CSystem.h"

CUIListItem::CUIListItem()
	:
	CUIControl(),
	m_pData(NULL),
	m_displayType(DT_PREV_TEXT),
	m_pItemName(NULL)
{
	m_buttonActiveID = UI_LIST_ITEM;
	SetActiveSprite(CSystem::m_gfx.DEFAULTSPRITE);
	SetInactiveSprite(CSystem::m_gfx.DEFAULTSPRITE);

	GetSprite(false).SetColor(0.5f, 0.5f, 0.5f, 1.0f);
	GetSprite(true).SetColor(1.0f, 1.0f, 1.0f, 0.0f);

	m_bSelected = false;
}

CUIListItem::CUIListItem(char * name, char * filename, void * pData, UINT type)
	:
	CUIControl(),
	m_pData(pData),
	m_displayType(type)
{
	m_buttonActiveID = UI_LIST_ITEM;
	SetItemName(name);
	if(filename)
		SetPreview(filename);

	SetActiveSprite(CSystem::m_gfx.DEFAULTSPRITE);
	SetInactiveSprite(CSystem::m_gfx.DEFAULTSPRITE);

	GetSprite(false).SetColor(0.5f, 0.5f, 0.5f, 1.0f);
	GetSprite(true).SetColor(1.0f, 1.0f, 1.0f, 0.0f);
	m_bSelected = false;
}

CUIListItem::CUIListItem(CUIMenu * pMenu, char * name, char * filename, void * pData, UINT type)
	:
	CUIControl(pMenu),
	m_pData(pData),
	m_displayType(type)
{
	m_buttonActiveID = UI_LIST_ITEM;
	SetItemName(name);
	SetPreview(filename);

	SetActiveSprite(CSystem::m_gfx.DEFAULTSPRITE);
	SetInactiveSprite(CSystem::m_gfx.DEFAULTSPRITE);

	GetSprite(false).SetColor(0.5f, 0.5f, 0.5f, 1.0f);
	GetSprite(true).SetColor(1.0f, 1.0f, 1.0f, 0.0f);
	m_bSelected = false;
}

CUIListItem::CUIListItem(CUIControl * pParent, char * name, char * filename, void * pData, UINT type)
	:
	CUIControl(pParent),
	m_pData(pData),
	m_displayType(type)
{
	m_buttonActiveID = UI_LIST_ITEM;
	SetItemName(name);
	SetPreview(filename);

	SetActiveSprite(CSystem::m_gfx.DEFAULTSPRITE);
	SetInactiveSprite(CSystem::m_gfx.DEFAULTSPRITE);

	GetSprite(false).SetColor(0.5f, 0.5f, 0.5f, 1.0f);
	GetSprite(true).SetColor(1.0f, 1.0f, 1.0f, 0.0f);
	m_bSelected = false;
}

CUIListItem::~CUIListItem()
{}

void CUIListItem::OnInit(RECT r, UINT id, char * active, char * inactive)
{
	m_buttonActiveID = UI_LIST_ITEM;

	GetSprite(false).SetColor(0.5f, 0.5f, 0.5f, 1.0f);
	GetSprite(true).SetColor(1.0f, 1.0f, 1.0f, 0.0f);
	m_bSelected = false;
	m_clip = true;
}

void * CUIListItem::GetData()
{
	return m_pData;
}

char * CUIListItem::GetItemName()
{
	return m_pItemName;
}

void CUIListItem::SetItemName(char * name)
{
	if (!name)
		return;
	STRING::Copy(m_pItemName, name);
}

void CUIListItem::SetData(void * pData)
{
	m_pData = pData;
}

void CUIListItem::SetDisplayType(UINT type)
{
	m_displayType = type;
}

void CUIListItem::SetPreview(char * filename)
{
	m_preview.Init(filename, CSystem::m_gfx);
}

void CUIListItem::OnUpdate()
{}

void CUIListItem::OnRender()
{
	GetSprite(!m_bSelected).Resize(GetRect().right - GetRect().left, GetRect().bottom - GetRect().top);
	GetSprite(!m_bSelected).SetPosition(GetGlobalX(), GetGlobalY());

	RECT r;
	r.bottom = GetParent()->GetGlobalY() + GetParent()->GetHeight();
	r.top = GetParent()->GetGlobalY();
	r.left = GetParent()->GetGlobalX();
	r.right = GetParent()->GetGlobalX() + GetParent()->GetWidth();
	GetSprite(!m_bSelected).SetRectLock(GetBounds());
	SYSTEM::RenderSprite(GetSprite(!m_bSelected), 9);
	if(m_clip)
		m_preview.SetRectLock(GetBounds());

	switch (m_displayType)
	{
	case DT_PREVIEW:
		RenderPreview();
		break;
	case DT_PREV_TEXT:
		RenderPrevText();
		break;
	case DT_TEXT:
		RenderText();
		break;
	}
}

void CUIListItem::OnPressed()
{
	IsSelected(!m_bSelected);
	if (m_pParent)
	{
		if (((CUIList*)m_pParent)->GetActiveSelection() == this)
		{
			((CUIList*)m_pParent)->SetActiveSelection(nullptr);
			m_bSelected = false;
		}
		else
		{
			((CUIList*)m_pParent)->SetActiveSelection(this);
		}
	}

}

void CUIListItem::RenderPreview()
{
	m_preview.Resize(GetRect().right - GetRect().left, GetRect().bottom - GetRect().top);
	m_preview.SetPosition(GetGlobalX(),GetGlobalY());

	SYSTEM::RenderSprite(m_preview, 9);
}

void CUIListItem::RenderPrevText()
{
	RECT r = GetRect();
	int pSideSize = (r.bottom - r.top);
	m_preview.Resize(pSideSize, pSideSize);
	m_preview.SetPosition(GetGlobalX(), GetGlobalY());
	int textheight = CSystem::g_textRenderer.GetActiveFont().GetFontHeight();
	int text_x = GetGlobalX() + pSideSize + 5;
	int text_y = GetGlobalY() + pSideSize / 2 - textheight/2;

	SYSTEM::RenderSprite(m_preview, 9);

	SYSTEM::RenderText(GetItemName(),text_x,text_y,1.0f,&GetBounds());
}

void CUIListItem::RenderText()
{
}
