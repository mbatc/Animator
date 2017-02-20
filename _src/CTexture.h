#pragma once

#include "CFileContainer.h"
#include "CD3DGraphics.h"
#include "CSurface.h"

class CTexture : public CFileContainer
{
public:
	CTexture();
	~CTexture();

	bool InitTexture(CD3DGraphics& gfx);
	bool InitTexture(CSurface surface, CD3DGraphics& gfx);

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }

	ID3D11ShaderResourceView* GetTexture() 
	{
		return m_pTexture;
	}

private:
	int m_width;
	int m_height;

	ID3D11ShaderResourceView* m_pTexture;
};