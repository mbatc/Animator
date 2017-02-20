#include "CFont.h"

CFont::CFont()
	:
	m_pCharacter(NULL),
	m_nCharacters(0)
{}

CFont::~CFont()
{}

CSprite CFont::GetCharacterSprite(char c)
{
	for (int i = 0; i < m_nCharacters; i++)
	{
		if (m_pCharacter[i].m_char == c)
			return m_pCharacter[i].m_sprite;
	}

	return CSprite();
}

CSprite CFont::GetCharacterSprite(int ascii)
{
	for (int i = 0; i < m_nCharacters; i++)
	{
		if (m_pCharacter[i].m_ascii == ascii)
			return m_pCharacter[i].m_sprite;
	}

	return CSprite();
}

CFont::CHARACTER CFont::GetCharacter(char c)
{
	for (int i = 0; i < m_nCharacters; i++)
	{
		if (m_pCharacter[i].m_char == c)
			return m_pCharacter[i];
	}

	return CHARACTER();
}

CFont::CHARACTER CFont::GetCharacter(int ascii)
{
	for (int i = 0; i < m_nCharacters; i++)
	{
		if (m_pCharacter[i].m_ascii == ascii)
			return m_pCharacter[i];
	}

	return CHARACTER();
}

