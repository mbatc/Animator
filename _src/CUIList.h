#pragma once

#include <Windows.h>
#include "CUIControl.h"

class CUIListItem;

class CUIList : public CUIControl
{
public:
	CUIList();
	CUIList(CUIControl* pParent);
	CUIList(CUIMenu* pMenu);
	~CUIList();

	CUIListItem*	GetListItem(int index);
	CUIListItem*	GetListItem(const char* name);
	char*			GetItemName(int index);
	void*			GetItemData(int index);
	void*			GetItemData(char* name);
	CUIListItem*	GetActiveSelection();
		
	bool AddListItem(CUIListItem* item);
	bool SetListType(const UINT type);
	
	void OnUpdate();
	void OnHover();
	void OnActive();
	void OnRender();
	void OnInit(RECT r, UINT id, char* active = NULL, char* inactive = NULL);
	void CheckIfActive(int mouseX, int mouseY, bool mouseDown);

	void SetItemHeight(int h) { m_itemHeight = h; }

	void SetActiveSelection(CUIListItem* item);
	void SetActiveSelection(int index) { m_activeSelection = index; }

	int GetActiveIndex() { return m_activeSelection; }

	void Clear();

	void LoadFromFolder(char* filepath, char* extension);
	void ReloadFromFolder();

	int GetItemCount() { return m_nItems; }
private:
	CUIListItem**	m_pItems;
	int				m_nItems;


	UINT m_displayType;

	int	m_itemHeight;

	int m_scrollY;
	int m_scrollX;

	int m_cur_scroll_y;
	int	m_scroll_accel_y;

	int m_cur_scroll_x;

	int m_listHeight;

	int m_activeSelection;

	char* file_directory;
	char* file_extension;
};