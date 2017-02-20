#pragma once

#define NVERTS_SPRITE 4

#include "CTexture.h"
#include "CSurface.h"
#include "CD3DGraphics.h"
#include <Windows.h>

class CSprite
{
public:
	CSprite();
	~CSprite();

	void Init(char* filename, CD3DGraphics& gfx);
	void Init(CSurface surface, CD3DGraphics& gfx);

	void Render(CD3DGraphics& gfx);

	int GetWidth();
	int GetHeight();

	int GetX();
	int GetY();

	float GetScaleY();
	float GetScaleX();

	float GetRotation();

	void Resize(int width, int height);

	void SetRectLock(RECT r);
	bool WithinLock();

	void SetPosition(int x, int y);
	void SetScaleX(float x);
	void SetScaleY(float y);
	void SetScale(float x, float y);
	void SetColor(float r, float g, float b, float a);
	void SetColor(D3DXVECTOR4 color);

	void SetAnchor(float x, float y);


	void SetRotation(float r);

private:
	CUSTOMVERTEX* GeneratePoints(CD3DGraphics& gfx);
	void CreateVertexBuffer(CD3DGraphics& gfx);

	D3DXMATRIX GetWorldMatrix(CD3DGraphics& gfx);

	void Rotate2DPoint(D3DXVECTOR2& v, float x, float y, float ax, float ay);

	bool	m_bInitFromSurface;

	int		m_xPos, m_yPos;
	float	m_xScale, m_yScale;
	float	m_rotation;

	float	m_anchorX;
	float	m_anchorY;

	D3DXVECTOR4 m_color;
	RECT		m_areaLock;
	CTexture* m_pTexture;

	ID3D11Buffer* m_pVertexBuffer;
};