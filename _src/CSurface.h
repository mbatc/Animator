#pragma once
#include "CD3DGraphics.h"

class CSurface
{
public:
	CSurface();
	~CSurface();

	void Init(D3DXVECTOR4* pPixels, int width, int height);
	void Init(char* filename);

	int GetHeight();
	int GetWidth();
	D3DXVECTOR4* GetPixels();


private:

	int m_width, m_height;
	D3DXVECTOR4* m_pPixels;
};