#pragma once
#include "CFileContainer.h"
#include "CD3DGraphics.h"
#include "CFont.h"

#define _FILE_FONT_TEXTURE_DIMENSIONS "fd"
#define _FILE_FONT_CHARACTER_HEIGHT "ch"
#define _FILE_FONT_CHARACTER_DEFINITION "c"

enum FILE_FONT_ATTRIBUTES
{
	FONT_UNKNOWN = -1,
	FONT_TEXTURE_DIMENSIONS,
	FONT_CHARACTER_HEIGHT,
	FONT_CHARACTER_DEFINITION
};

class CFontFileContainer : public CFileContainer
{
	struct CharacterDefinition
	{
		char c;
		int ascii;
		int x, y;
		int width;
	};

public:
	CFontFileContainer();
	~CFontFileContainer();

	void GetLoadedFont(CFont& font);

	bool LoadCurrentFile(CD3DGraphics& gfx);
private:
	bool LoadCharacter();
	bool LoadTexture();
	bool LoadCharacterSurfaces(CD3DGraphics& gfx);

private:

	CharacterDefinition* m_pCharacter;
	CSurface* m_pCharacterSurface;
	CSprite* m_pChracterSprite;

	CSurface m_fileTexture;

	int m_texWidth ,m_texHeight, m_characterHeight;
	int m_nCharacters;
};