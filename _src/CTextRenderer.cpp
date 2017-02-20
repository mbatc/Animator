#include "CTextRenderer.h"
#include "Helper.h"

CTextRenderer::CTextRenderer()
	:
	m_color(0.0f,0.0f,0.0f,1.0f),
	m_pFont(NULL)
{}

CTextRenderer::~CTextRenderer()
{}

void CTextRenderer::Init(CD3DGraphics* gfx)
{
	m_pGfx = gfx;
}

void CTextRenderer::RenderText(char* text, float x, float y, float scale, RECT* lock)
{
	if (!m_pFont)
		return;
	if (m_color.w == 0.0f)
		return;

	int xPix = m_pGfx->GetClientWidth() * x;
	int yPix = m_pGfx->GetClientHeight() * y;

	RenderText(text, xPix, yPix, scale, lock);
}

void CTextRenderer::RenderText(char* text, int x, int y, float scale, RECT* lock)
{
	int strLen = strlen(text);

	int currentX = x;
	int currentY = y;

	for (int i = 0; i < strLen; i++)
	{
		if (text[i] == '\n')
		{
			currentX = x;
			currentY += m_pFont->GetFontHeight() * scale;
		}
		else
		{
			RenderCharacter(text[i], currentX, currentY, scale, lock);
		}
	}
}

int CTextRenderer::GetWidthOfString(const char * string, float scale)
{
	int width = 0;
	if (!string)
		return 0;

	CSprite character;

	int str_len = strlen(string);

	for (int i = 0; i < str_len; i++)
	{
		char c = string[i];
		switch (c)
		{
		case '\t':
			width += 6 * 4 * scale;
			break;
		case '\0':
			return width;
		case ' ':
			width += 6 * scale;
			break;
		default:
			character = m_pFont->GetCharacterSprite(c);
			character.SetScale(scale, scale);
			width += character.GetWidth() * scale;
			break;
		}
	}

	return width;
}

int CTextRenderer::GetHeightOfString(const char * string, float scale)
{
	int height = GetActiveFont().GetFontHeight() * scale;
	if (!string)
		return 0;

	int str_len = strlen(string);
	for (int i = 0; i < str_len; i++)
	{
		if (string[i] == '\n')
			height += GetActiveFont().GetFontHeight();
	}
	return height;
}

void CTextRenderer::RenderCharacter(char c, int& x, int& y, float scale, RECT* lock)
{
	CSprite character;

	switch (c)
	{
	case '\t':
		x += 6 * 4 * scale;
		break;
	case '\0':
		return;
	case ' ':
		x += 6 * scale;
		break;
	default:
		character = m_pFont->GetCharacterSprite(c);

		character.SetPosition(x, y);
		character.SetScale(scale, scale);
		character.SetColor(m_color);
		if (lock)
			character.SetRectLock(*lock);

		SYSTEM::RenderSprite(character,9);
		
		x += character.GetWidth() * scale;
		break;
	}
}

void CTextRenderer::SetColor(D3DXVECTOR4 color)
{
	m_color = color;
}

void CTextRenderer::SetFont(CFont* font)
{
	m_pFont = font;
}