#pragma once

#include "CUIControl.h"

class CUITextIO : public CUIControl
{
public:
	CUITextIO();
	CUITextIO(CUIMenu* pMenu);
	CUITextIO(CUIControl* pParent);
	
	bool IsActive() { return (m_activeID == GetID()); }

private:
	void OnUpdate();
	void OnRender();
	void OnActive();
	void OnInit();
	void OnClickAway();

	void GetInputBuffer();

	static UINT m_activeID;

	int start;
	int end;

	float m_timer;
	bool m_indicator;
};