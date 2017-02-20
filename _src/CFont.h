#pragma once

#include "CSprite.h"

class CFontFileContainer;

class CFont
{
	friend CFontFileContainer;

	struct CHARACTER
	{
		CHARACTER()
			:
			m_ascii(0),
			m_char('\0')
		{}
		int m_ascii;
		char m_char;
		CSprite m_sprite;
	};
public:
	CFont();
	~CFont();

	int GetFontHeight() { return m_fontHeight; }

	CSprite GetCharacterSprite(char c);
	CSprite GetCharacterSprite(int ascii);

	CHARACTER GetCharacter(char c);
	CHARACTER GetCharacter(int ascii);

	int m_spaceSize;

private:
	CHARACTER* m_pCharacter;
	int m_nCharacters;
	int m_fontHeight;
};