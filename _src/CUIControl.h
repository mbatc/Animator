#pragma once

#include <Windows.h>

#include "CSprite.h"
#include "CD3DGraphics.h"

#define UI_LIST_ITEM 0x0001u

enum TEXTOFFSET
{
	LEFT,
	RIGHT,
	CENTER,
	TOP,
	BOTTOM
};

class CUIMenu;

class CUIControl
{
	friend CUIMenu;
public:
	CUIControl();
	CUIControl(CUIMenu* pMenu);
	CUIControl(CUIControl* pParent);
	~CUIControl();

	CUIControl* GetParent() { return m_pParent; }
	CUIMenu* GetMenu() { return m_pMenu; }

	const int GetGlobalX() { return m_translatedPosX; }
	const int GetGlobalY() { return m_translatedPosY; }

	RECT GetRect() { return m_activeRect; }
	RECT GetBounds() { return m_bounds; }

	char* GetText() { return m_controlText; }

	CSprite& GetSprite(bool active) { return active ? m_spriteStateActive : m_spriteStateInactive; }

	virtual void InitControl(RECT r, UINT id,char* active = NULL, char* inactive = NULL);

	UINT GetID();

	virtual void CheckIfActive(int mouseX, int mouseY, bool buttonDown);
	void Update();
	void Render(CD3DGraphics& gfx);
	void Clip(bool b) { m_clip = b; }
	
	void SetPosition(RECT r);
	void SetText(char* text);

	void SetMenu(CUIMenu* pMenu);
	void SetParent(CUIControl* pParent);

	void SetTextOffsetX(TEXTOFFSET offset) { m_text_offset_x = offset; }
	void SetTextOffsetY(TEXTOFFSET offset) { m_text_offset_y = offset; }

	/////////////////////////////////////////////////////////////////////
	//CONTROL BORDER FUNCTIONS
	//*******************************************************************

	void RenderBorder(bool b);
	void RenderBorder(bool left, bool right, bool top, bool bottom);
	void SetBorderInner(bool b);
	void SetBorderWidth(int w) { m_borderWidth = w; }
	int GetBorderWidth() { return m_borderWidth; }

	void SetBorderColour(float r, float g, float b, float a);
	void SetBorderColour(D3DXVECTOR4 color);

	void SetBorderSprite();
	void SetBorderSprite(CSprite* corner, CSprite* edge);
	void SetBorderSprite(char* corner, char* edge);
	//*******************************************************************

	void SetActiveSprite(char* filename);
	void SetActiveSprite(CSprite sprite) { m_spriteStateActive = sprite; }
	void SetInactiveSprite(char* filename);
	void SetInactiveSprite(CSprite sprite) { m_spriteStateInactive = sprite; }

	void SetID(UINT ID);

	int GetHeight() { return m_activeRect.bottom - m_activeRect.top; }
	int GetWidth() { return m_activeRect.right - m_activeRect.left; }

	void RenderText();
protected:

	virtual void OnPressed();
	virtual void OnActive();
	virtual void OnHover();
	virtual void OnRelease();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnClickAway();
	virtual void OnInit(RECT r, UINT id, char* active = NULL, char* inactive = NULL);

	void RenderBorder();
	void UpdateBounds();

	int GetTextX();
	int GetTextY();

	CUIMenu* m_pMenu;
	CUIControl* m_pParent;

	UINT m_buttonActiveID;
	char* m_controlText;

	void SetRenderBoundary(RECT r);

	RECT m_activeRect;
	RECT m_bounds;

	int m_xPos, m_yPos, m_translatedPosX, m_translatedPosY;
	bool m_bIsActive;

	bool m_bRenderBorder;
	bool m_bInnerBorder;

	bool m_clip;

	int m_borderWidth;

	CSprite m_bdrCorner[4];
	CSprite m_bdrEdge[4];
	bool	m_bdrRender[4];

	CSprite m_spriteStateActive;
	CSprite m_spriteStateInactive;

	TEXTOFFSET m_text_offset_x;
	TEXTOFFSET m_text_offset_y;
};