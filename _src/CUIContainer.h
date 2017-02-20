#pragma once
#include "CUIControl.h"

class CUIContainer : public CUIControl
{
public:
	CUIContainer();
	CUIContainer(CUIControl* parent);
	CUIContainer(CUIMenu* menu);	
private:
	void OnInit(RECT r, UINT id, char* active = NULL, char* inactive = NULL);
	void OnRender();
	void OnActive();
	void OnUpdate();

	CUIControl m_bg;
};