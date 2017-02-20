#pragma once

#include "CUIControl.h"

class CUIListItem : public CUIControl
{
public: 
	static const UINT DT_PREVIEW = 0;
	static const UINT DT_PREV_TEXT = 1;
	static const UINT DT_TEXT = 2;

public:
	CUIListItem();
	CUIListItem(char* name, char* filename, void* pData, UINT type = DT_PREV_TEXT);
	CUIListItem(CUIMenu* pMenu, char* name = NULL, char* filename = NULL, void* pData = NULL, UINT type = DT_PREV_TEXT);
	CUIListItem(CUIControl* pParent, char* name = NULL, char* filename = NULL, void* pData = NULL, UINT type = DT_PREV_TEXT);
	~CUIListItem();

	void OnInit(RECT r, UINT id, char* active = NULL, char* inactive = NULL);

	void* GetData();
	char* GetItemName();

	void SetItemName(char* name);
	void SetData(void* pData);
	void SetDisplayType(UINT type);
	void SetPreview(char* filename);
	void IsSelected(bool b) { m_bSelected = b; }

	void OnUpdate();
	void OnRender();
	void OnPressed();

private:
	void RenderPreview();
	void RenderPrevText();
	void RenderText();

	void*	m_pData;
	char*	m_pItemName;

	CSprite	m_preview;

	UINT m_displayType;

	bool m_bSelected;
};