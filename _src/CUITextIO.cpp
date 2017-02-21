#include "CUITextIO.h"
#include "Helper.h"
#include "CD3DGraphics.h"
#include "CSystem.h"

#include <iostream>

UINT CUITextIO::m_activeID = 0;

CUITextIO::CUITextIO()
	:
	CUIControl()
{}

CUITextIO::CUITextIO(CUIMenu * pMenu)
	:
	CUIControl(pMenu)
{}

CUITextIO::CUITextIO(CUIControl * pParent)
	:
	CUIControl(pParent)
{}

void CUITextIO::OnUpdate()
{
	start = end;
	end = clock();
	if (m_activeID == GetID())
	{
		GetInputBuffer();
	}

	m_timer += (float)(end-start)/ (float)CLOCKS_PER_SEC;

	if (m_timer >= 0.5f)
	{
		m_indicator = !m_indicator;
		m_timer = 0;
	}

	if (KEYBOARD::IsDown(VK_RETURN))
	{
		OnClickAway();
	}
}

void CUITextIO::OnRender()
{
	if (m_activeID == GetID())
	{
		SYSTEM::RenderSprite(GetSprite(true), 8);
	}
	else
	{
		m_indicator = false;
		SYSTEM::RenderSprite(GetSprite(false), 8);
	}

	if (m_indicator)
	{
		if (m_controlText)
			STRING::Append(m_controlText, m_controlText, "|");
		else
			STRING::Copy(m_controlText, "|");
	}

	RECT t;
	RECT ar = GetRect();
	t.bottom = ar.bottom - ar.top + GetGlobalY();
	t.top = GetGlobalY();
	t.left = GetGlobalX();
	t.right = ar.right - ar.left + GetGlobalX();

	RenderText();

	if (m_indicator)
	{
		m_controlText[strlen(m_controlText) - 1] = '\0';
	}
}

void CUITextIO::OnActive()
{
	m_activeID = GetID();
}

void CUITextIO::OnClickAway()
{
	if (m_activeID == GetID())
		m_activeID = 0;
}

void CUITextIO::GetInputBuffer()
{
	const char* ib = KEYBOARD::GetInBuffer();

	if (!ib)
		return;

	char temp[2] = { '\0','\0' };
	char* text = 0;

	if(GetText())
		STRING::Copy(text, GetText());

	int ib_len = strlen(ib) + 1;

	for (int i = 0; i < ib_len; i++)
	{
		switch (ib[i])
		{
		case VK_BACK:
			if (text)
			{
				if(text[0] != '\0')
					text[strlen(text) - 1] = '\0';
			}
			break;
		default:
			temp[0] = ib[i];
			if (text)
				STRING::Append(text, text, temp);
			else
				STRING::Copy(text, temp);
			break;
		}
	}

	if (text)
		SetText(text);
	else
		SetText("");

	MEMORY::SafeDeleteArr(text);
}

void CUITextIO::OnInit()
{
	SetInactiveSprite(CD3DGraphics::DEFAULTSPRITE);
	SetActiveSprite(CD3DGraphics::DEFAULTSPRITE);

	GetSprite(true).SetColor(0.4f, 0.4f, 0.4f, 1.0f);
	GetSprite(false).SetColor(0.1f, 0.1f, 0.1f, 1.0f);

	m_timer = 0;
	end = clock();
}
