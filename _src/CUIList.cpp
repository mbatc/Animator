#include "CUIList.h"
#include "CUIListItem.h"

#include "Helper.h"
#include "CSystem.h"

#include "CDirectoryFileList.h"
#include "SystemMessage.h"

CUIList::CUIList()
	:
	CUIControl(),
	m_displayType(CUIListItem::DT_PREV_TEXT),
	m_activeSelection(-1),
	file_directory(0),
	file_extension(0)
{}

CUIList::CUIList(CUIControl * pParent)
	:
	CUIControl(pParent),
	m_displayType(CUIListItem::DT_PREV_TEXT),
	m_activeSelection(-1),
	file_directory(0),
	file_extension(0)
{}

CUIList::CUIList(CUIMenu * pMenu)
	:
	CUIControl(pMenu),
	m_displayType(CUIListItem::DT_PREV_TEXT),
	m_activeSelection(-1),
	file_directory(0),
	file_extension(0)
{}

CUIList::~CUIList()
{}

CUIListItem * CUIList::GetListItem(int index)
{
	return m_pItems[index];
}

CUIListItem * CUIList::GetListItem(const char * name)
{
	if (!m_pItems)
		return NULL;
	for (int i = 0; i < m_nItems; i++)
	{
		if (!m_pItems[i])
			continue;

		if (!strcmp(m_pItems[i]->GetItemName(), name))
		{
			return m_pItems[i];
		}
	}
}

char * CUIList::GetItemName(int index)
{
	if (!m_pItems)
		return NULL;

	return m_pItems[index] ? m_pItems[index]->GetItemName() : NULL;
}

void * CUIList::GetItemData(int index)
{
	if (!m_pItems)
		return NULL;

	return m_pItems[index] ? m_pItems[index]->GetData() : NULL;
}

void * CUIList::GetItemData(char * name)
{
	if (!m_pItems)
		return NULL;
	for (int i = 0; i < m_nItems; i++)
	{
		if (!m_pItems[i])
			continue;

		if (strcmp(m_pItems[i]->GetItemName(), name))
		{
			return m_pItems[i]->GetData();
		}
	}
}

CUIListItem * CUIList::GetActiveSelection()
{
	if (!m_pItems)
		return nullptr;

	if(m_activeSelection >= 0)
		return m_pItems[m_activeSelection];

	return nullptr;
}

bool CUIList::AddListItem(CUIListItem * item)
{
	if (!item)
		return false;

	item->SetParent((CUIControl*)this);
	item->SetDisplayType(m_displayType);

	MEMORY::LIST::AddToListArr(m_pItems, item, m_nItems);

	return true;
}

bool CUIList::SetListType(const UINT type)
{
	switch (type)
	{
	case CUIListItem::DT_PREVIEW:
		break;
	case CUIListItem::DT_PREV_TEXT:
		break;
	case CUIListItem::DT_TEXT:
		break;
	default:
		return false;
	}

	m_displayType = type;

	if (!m_pItems)
		return true;

	for (int i = 0; i < m_nItems; i++)
	{
		if (m_pItems[i])
			m_pItems[i]->SetDisplayType(type);
	}
}

void CUIList::OnUpdate()
{
	m_cur_scroll_y += m_scroll_accel_y;
	m_listHeight = m_itemHeight*m_nItems;

	int m_controlHeight = GetRect().bottom - GetRect().top;

	if (m_cur_scroll_y - m_controlHeight < -m_listHeight) m_cur_scroll_y = -m_listHeight + m_controlHeight;
	if (m_cur_scroll_y > 0) m_cur_scroll_y = 0;

	RECT ir;
	ir.top = m_cur_scroll_y;
	ir.bottom = m_itemHeight + m_cur_scroll_y;
	ir.left = 0;
	ir.right = GetWidth();

	for (int i = 0; i < m_nItems; i++)
	{
		if (i == m_activeSelection)
			m_pItems[i]->IsSelected(true);
		else
			m_pItems[i]->IsSelected(false);

		m_pItems[i]->SetPosition(ir);
		m_pItems[i]->Update();
		ir.top += m_itemHeight;
		ir.bottom += m_itemHeight;
	}

	if (!MOUSE::IsLeftMouseDown())
	{
		if (m_scroll_accel_y > 0)
		{
			m_scroll_accel_y -= 1;
			if (m_scroll_accel_y < 0)
				m_scroll_accel_y = 0;
		}
		if (m_scroll_accel_y < 0)
		{
			m_scroll_accel_y += 1;
			if (m_scroll_accel_y > 0)
				m_scroll_accel_y = 0;
		}
	}
}

void CUIList::OnHover()
{
}

void CUIList::OnActive()
{
	m_scroll_accel_y = MOUSE::GetChangeY();
	SYSTEM::AddMessage(GetID(), _SM_UI_SELECTION_CHANGE);
}

void CUIList::OnRender()
{
	CUIControl::OnRender();
	if (!m_pItems)
		return;

	for (int i = 0; i < m_nItems; i++)
	{
		if (m_pItems[i])
			m_pItems[i]->Render(CSystem::m_gfx);
	}

	RenderBorder();
}

void CUIList::OnInit(RECT r, UINT id, char* active, char* inactive)
{
	CUIControl::OnInit(r,id,active,inactive);
	GetSprite(false).SetColor(0.1f, 0.1f, 0.1f, 1.0f);
	GetSprite(true).SetColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void CUIList::CheckIfActive(int mouseX, int mouseY, bool mouseDown)
{
	CUIControl::CheckIfActive(mouseX, mouseY, mouseDown);
	for (int i = 0; i < m_nItems; i++)
	{
		if (m_pItems[i])
			m_pItems[i]->CheckIfActive(mouseX, mouseY, mouseDown);
	}
}

void CUIList::SetActiveSelection(CUIListItem * item)
{
	if (!item)
	{
		m_activeSelection = -1;
		return;
	}
	if (!m_pItems)
		return;

	for (int i = 0; i < m_nItems; i++)
	{
		if (m_pItems[i] == item)
			m_activeSelection = i;
	}
}

void CUIList::Clear()
{
	MEMORY::SafeDeleteArr(m_pItems);
	m_nItems = 0;
	m_activeSelection = -1;
}

void CUIList::LoadFromFolder(char * filepath, char * extension)
{
	MEMORY::SafeDeleteArr(m_pItems);
	m_nItems = 0;

	if (!filepath)
		return;

	char* file = 0;

	int str_len = strlen(filepath);
	if (filepath[str_len - 1] != '*')
	{
		STRING::Append(file_directory, filepath, "*");
	}
	else
	{
		STRING::Copy(file_directory, filepath);
	}

	if (extension)
		STRING::Copy(file_extension, extension);

	CDirectoryFileList fileList;
	fileList.SetFileExtension(extension);
	fileList.SetDirectory(filepath);
	fileList.FindFiles();

	for (int i = 0; i < fileList.GetNumberOfFiles(); i++)
	{
		CUIListItem* item = new CUIListItem;
		char* filename = 0;
		STRING::Copy(filename, fileList.GetFilename(i));
		char* ext = FILEH::GetFileExtension(filename);
		ext[0] = '\0';
		item->SetItemName(filename);

		char* temp = 0;

		STRING::Copy(temp, fileList.GetFilename(i));
		ext = FILEH::GetFileExtension(temp);
		ext[0] = '\0';

		item->SetData((void*)temp);
		AddListItem(item);
	}
}

void CUIList::ReloadFromFolder()
{
	char* dir = 0; char* ext = 0;
	STRING::Copy(dir, file_directory);
	STRING::Copy(ext, file_extension);
	LoadFromFolder(dir, ext);
}
