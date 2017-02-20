#pragma once

#include "CFont.h"

class CTextRenderer
{
public:
	CTextRenderer();
	~CTextRenderer();

	void Init(CD3DGraphics* gfx);

	void SetFont(CFont* font);
	void SetColor(D3DXVECTOR4 color);
	void RenderText(char* text, int x, int y, float scale = 1.0f, RECT* lock = NULL);
	void RenderText(char* text, float x, float y, float scale = 1.0f, RECT* lock = NULL);

	CFont& GetActiveFont() { return *m_pFont; }

	int GetWidthOfString(const char* string, float scale = 1.0f);
	int GetHeightOfString(const char* string, float scale = 1.0f);
private:
	void RenderCharacter(char c, int& xPos, int& yPos, float scale, RECT* lock);

	CFont* m_pFont;
	D3DXVECTOR4 m_color;
	CD3DGraphics* m_pGfx;
};