#include "CFontFileContainer.h"
#include "Helper.h"

#include <D3D11.h>
#include <iostream>
#include <stdio.h>

#include <GdiPlus.h>
#pragma comment( lib,"gdiplus.lib" )

using namespace std;

FILE_FONT_ATTRIBUTES GetFileAttribute(char* buffer)
{
	if(!strcmp(buffer,_FILE_FONT_TEXTURE_DIMENSIONS))
		return FONT_TEXTURE_DIMENSIONS;
	else if(!strcmp(buffer,_FILE_FONT_CHARACTER_HEIGHT))
		return FONT_CHARACTER_HEIGHT;
	else if(!strcmp(buffer,_FILE_FONT_CHARACTER_DEFINITION))
		return FONT_CHARACTER_DEFINITION;
	else
		return FONT_UNKNOWN;

	return FONT_UNKNOWN;
}

CFontFileContainer::CFontFileContainer()
	:
	m_pCharacter(NULL),
	m_texWidth(0),
	m_texHeight(0),
	m_characterHeight(0),
	m_nCharacters(0),
	m_pCharacterSurface(NULL),
	m_pChracterSprite(NULL)
{}

CFontFileContainer::~CFontFileContainer()
{}

bool CFontFileContainer::LoadCurrentFile(CD3DGraphics& gfx)
{
	if(!m_pCurrentFile)
	{
		cout << "CFontFileContainer: No file has been specified...\n";
		return false;
	}

	int dirIndex = strlen(m_pBaseDirectory);
	char* tempFilename = NULL;

	//Getting the path and name of the .font file
	STRING::Append(tempFilename,m_pBaseDirectory,"Font Definition/");
	STRING::Append(tempFilename,tempFilename,&m_pCurrentFile[strlen(m_pBaseDirectory)]);
	int i;
	for( i = strlen(tempFilename); i > 0 && tempFilename[i] != '.'; i--);
	tempFilename[i] = '\0';
	STRING::Append(tempFilename,tempFilename,".font");

	m_pFile = fopen(tempFilename,"rb");
	if(!m_pFile)
	{
		cout << "CFontFileContainer: Could not open file '" << tempFilename << "'...\n";
		return false;
	}

	do
	{
		char buffer[32] = {0};
		if(!feof(m_pFile))
		{
			fscanf(m_pFile,"%s",buffer);
			switch(GetFileAttribute(buffer))
			{
			case FONT_TEXTURE_DIMENSIONS:
				fscanf(m_pFile,"%d%d",&m_texWidth,&m_texHeight);
				break;
			case FONT_CHARACTER_HEIGHT:
				fscanf(m_pFile,"%d",&m_characterHeight);
				break;
			case FONT_CHARACTER_DEFINITION:
				if(!LoadCharacter())
				{
					cout << "CFontFileContainer: An Unknown Error Occured...\n";
					return false;
				}
				break;
			}
		}
	}while(!feof(m_pFile));

	fclose(m_pFile);

	if(!LoadTexture())
	{
		cout << "CFontFileContainer: Could Not Load Font Texture '" << m_pCurrentFile <<"'...\n";
	}

	if(!LoadCharacterSurfaces(gfx))
	{
		cout << "CFontFileContainer: Could Not Character Surfaces...\n";
	}

	return true;
}

bool CFontFileContainer::LoadCharacter()
{
	char c = '\0';
	int ascii = 0,x = 0,y = 0,width = 0;

	if(!m_pFile)
		return false;

	fscanf(m_pFile,"%d",&ascii);
	fread(&c,sizeof(char),1,m_pFile);
	fread(&c,sizeof(char),1,m_pFile);
	fscanf(m_pFile,"%d%d%d",&x,&y,&width);

	if(c == '\0')
		return false;

	CharacterDefinition temp;
	temp.ascii = ascii;
	temp.c = c;
	temp.width = width;
	temp.x = x;
	temp.y = y;

	MEMORY::LIST::AddToList(m_pCharacter,&temp,m_nCharacters);

	if(!m_pCharacter)
		return false;

	return true;
}

bool CFontFileContainer::LoadTexture()
{
	m_fileTexture.Init(m_pCurrentFile);

	for (int i = 0; i < m_fileTexture.GetHeight() * m_fileTexture.GetWidth(); i++)
	{
		m_fileTexture.GetPixels()[i].x = 1.0f - m_fileTexture.GetPixels()[i].x;
		m_fileTexture.GetPixels()[i].y = 1.0f - m_fileTexture.GetPixels()[i].y;
		m_fileTexture.GetPixels()[i].z = 1.0f - m_fileTexture.GetPixels()[i].z;

		m_fileTexture.GetPixels()[i].w = (m_fileTexture.GetPixels()[i].x + m_fileTexture.GetPixels()[i].y + m_fileTexture.GetPixels()[i].z) / 3.0f;
	}


	return true;
}

bool CFontFileContainer::LoadCharacterSurfaces(CD3DGraphics& gfx)
{
	MEMORY::SafeDeleteArr(m_pCharacterSurface);
	int nSurfaces = 0;

	for(int i = 0; i < m_nCharacters;i++)
	{
		D3DXVECTOR4* temp = new D3DXVECTOR4[m_pCharacter[i].width * m_characterHeight];
		int width = m_pCharacter[i].width;
		int height = m_characterHeight;
		for(int x = 0 ; x < width; x++)
		{
			for(int y = 0; y < height; y++)
			{
				temp[x + y * width] = m_fileTexture.GetPixels()[
					(m_pCharacter[i].x + x) +
					(y + m_pCharacter[i].y) * 
					m_texWidth];
			}
		}
		CSurface surface;
		surface.Init(temp,width,height);
		CSprite sprite;
		sprite.Init(surface, gfx);
		MEMORY::LIST::AddToList(m_pChracterSprite,&sprite,nSurfaces);
	}

	return true;
}

void CFontFileContainer::GetLoadedFont(CFont& font)
{
	font.m_pCharacter = new CFont::CHARACTER[m_nCharacters];

	for (int i = 0; i < m_nCharacters; i++)
	{
		font.m_pCharacter[i].m_sprite = m_pChracterSprite[i];
		font.m_pCharacter[i].m_ascii = m_pCharacter[i].ascii;
		font.m_pCharacter[i].m_char = m_pCharacter[i].c;
	}
	font.m_fontHeight = m_characterHeight;
	font.m_nCharacters = m_nCharacters;

	for (int i = 0; i < m_nCharacters; i++)
	{
		if (' ' == m_pCharacter[i].c)
		{
			font.m_spaceSize = m_pCharacter[i].width;
			break;
		}
	}
}