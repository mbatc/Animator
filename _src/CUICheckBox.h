#pragma once

#include "CUIControl.h"

class CUICheckBox : public CUIControl
{
public:
	CUICheckBox();
	CUICheckBox(CUIControl* parent);
	CUICheckBox(CUIMenu* menu);

	void OnInit(RECT r, UINT id, char* active = NULL, char* inactive = NULL);
	void OnRender();
	void OnPressed();

	void SetState(bool s);
	bool GetState();
private:
	bool m_state;
	CSprite m_spr;
};