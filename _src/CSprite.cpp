#include "CSprite.h"
#include "Helper.h"

#define _USE_MATH_DEFINES
#include <math.h>

CSprite::CSprite()
	:
	m_pTexture(NULL),
	m_bInitFromSurface(false),
	m_pVertexBuffer(NULL),
	m_yPos(0),
	m_xPos(0),
	m_yScale(1.0f),
	m_xScale(1.0f),
	m_color(1.0f, 1.0f, 1.0f, 1.0f),
	m_anchorX(0.0f),
	m_anchorY(0.0f)
{
	m_areaLock.bottom = -1;
	m_areaLock.left = -1;
	m_areaLock.right = -1;
	m_areaLock.top = -1;
}

CSprite::~CSprite()
{

}

void CSprite::Init(char* filename, CD3DGraphics& gfx)
{
	m_pTexture = SYSTEM::RESOURCE::GetTexture(filename);
	m_bInitFromSurface = false;

	CreateVertexBuffer(gfx);
}

void CSprite::Init(CSurface surface, CD3DGraphics& gfx)
{
	if (m_bInitFromSurface)
		MEMORY::SafeDelete(m_pTexture);

	m_pTexture = new CTexture();
	m_pTexture->InitTexture(surface, gfx);

	m_bInitFromSurface = true;

	CreateVertexBuffer(gfx);
}

void CSprite::CreateVertexBuffer(CD3DGraphics& gfx)
{
	MEMORY::DX::SafeRelease(m_pVertexBuffer);

	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = sizeof(CUSTOMVERTEX) * NVERTS_SPRITE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	CUSTOMVERTEX* temp = new CUSTOMVERTEX[NVERTS_SPRITE];

	data.pSysMem = temp;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	if (FAILED(gfx.GetDevice().pDevice->CreateBuffer(&desc, &data, &m_pVertexBuffer)))
		return;
}

D3DXMATRIX CSprite::GetWorldMatrix(CD3DGraphics& gfx)
{
	float x, y;
	y = -((-gfx.GetClientHeight() / 2.0f) + m_xPos) / (gfx.GetClientHeight() / 2.0f);
	x = ((-gfx.GetClientWidth() / 2.0f) + m_yPos) / (gfx.GetClientWidth() / 2.0f);

	D3DXMATRIX w;

	D3DXMATRIX r;
	D3DXMATRIX t1;
	D3DXMATRIX t2;

	D3DXMatrixIdentity(&w);
	D3DXMatrixTranslation(&t1, -x, -y, 0.0f);
	D3DXMatrixRotationZ(&r, m_rotation);
	D3DXMatrixTranslation(&t2, x, y, 0.0f);
	w = t1*r*t2;
	return w;
}

void CSprite::Rotate2DPoint(D3DXVECTOR2 & v, float x, float y, float ax, float ay)
{
	float s = sinf(m_rotation);
	float c = cosf(m_rotation);

	v.x = x - ax;
	v.y = y - ay;

	float xnew = v.x * c - v.y * s;
	float ynew = v.x * s + v.y * c;

	v.x = xnew + ax;
	v.y = ynew + ay;
}

void CSprite::Render(CD3DGraphics & gfx)
{
	if (!m_pTexture)
	{
		//ADD OUTPUT CLASS
		return;
	}

	CUSTOMVERTEX* tempBuffer = GeneratePoints(gfx);

	if (!tempBuffer)
	{
		//ADD OUTPUT CLASS
		return;
	}

	D3D11_MAPPED_SUBRESOURCE resource;

	HRESULT result;
	result = gfx.GetDevice().pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if (FAILED(result))
		return;

	CUSTOMVERTEX* pData = (CUSTOMVERTEX*)resource.pData;
	memcpy(pData, tempBuffer, sizeof(CUSTOMVERTEX)*NVERTS_SPRITE);

	gfx.GetDevice().pDeviceContext->Unmap(m_pVertexBuffer, 0);
	MEMORY::SafeDeleteArr(tempBuffer);
	UINT stride = sizeof(CUSTOMVERTEX);
	UINT offset = 0;

	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);

	gfx.GetDevice().pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx.GetDevice().pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	gfx.RenderSprite(m_pTexture->GetTexture(), world);
}

CUSTOMVERTEX* CSprite::GeneratePoints(CD3DGraphics& gfx)
{
	if (!m_pTexture)
		return NULL;

	float top, bottom, left, right;

	int width = m_pTexture->GetWidth() * m_xScale;
	int height = m_pTexture->GetHeight() * m_yScale;

	int x = m_xPos;
	int y = m_yPos;
	int spriteWidth = width;
	int spriteHeight = height;
	
	if (m_areaLock.bottom != -1 && m_areaLock.left != -1 &&
		m_areaLock.right != -1 && m_areaLock.top != -1)
	{
		if (m_xPos > m_areaLock.right || m_xPos + width < m_areaLock.left ||
			m_yPos > m_areaLock.bottom || m_yPos + height < m_areaLock.top)
			return NULL;


		if (m_xPos < m_areaLock.left)
			x = m_areaLock.left;
		if (width + m_xPos > m_areaLock.right)
			spriteWidth = m_areaLock.right - m_xPos;
		if (height + m_yPos > m_areaLock.bottom)
			spriteHeight = m_areaLock.bottom - m_yPos;
		if (m_yPos < m_areaLock.top)
			y = m_areaLock.top;
	}

	D3DXVECTOR2 tl;
	D3DXVECTOR2 tr;
	D3DXVECTOR2 bl;
	D3DXVECTOR2 br;

	Rotate2DPoint(tl, x - m_anchorX, y - m_anchorY, m_xPos, m_yPos);
	Rotate2DPoint(tr, m_xPos + spriteWidth - m_anchorX, y - m_anchorY, m_xPos, m_yPos);
	Rotate2DPoint(bl, x - m_anchorX, m_yPos + spriteHeight - m_anchorY, m_xPos, m_yPos);
	Rotate2DPoint(br, m_xPos + spriteWidth - m_anchorX, m_yPos + spriteHeight - m_anchorY, m_xPos, m_yPos);

	int cheight = gfx.GetClientHeight();
	int cwidth = gfx.GetClientWidth();

	float tl_x = ((-cwidth / 2.0f) + tl.x) / (cwidth / 2.0f);
	float tl_y = -((-cheight / 2.0f) + tl.y) / (cheight / 2.0f);

	float tr_x = ((-cwidth / 2.0f) + tr.x) / (cwidth / 2.0f);
	float tr_y = -((-cheight / 2.0f) + tr.y) / (cheight / 2.0f);

	float bl_x = ((-cwidth / 2.0f) + bl.x) / (cwidth / 2.0f);
	float bl_y = -((-cheight / 2.0f) + bl.y) / (cheight / 2.0f);

	float br_x = ((-cwidth / 2.0f) + br.x) / (cwidth / 2.0f);
	float br_y = -((-cheight / 2.0f) + br.y) / (cheight / 2.0f);

	/*bottom = -((-gfx.GetClientHeight() / 2.0f) + m_yPos + spriteHeight) / (gfx.GetClientHeight()/2.0f) - (m_anchorY*height / (gfx.GetClientHeight()/2));
	left = ((-gfx.GetClientWidth() / 2.0f) + x) / (gfx.GetClientWidth() / 2.0f) - (m_anchorX*width / (gfx.GetClientWidth() * 2));
	right = ((-gfx.GetClientWidth() / 2.0f) + m_xPos + spriteWidth) / (gfx.GetClientWidth() / 2.0f) - (m_anchorX*width / (gfx.GetClientWidth() * 2));*/
/*
	float anchorX = left;
	float anchorY = top;*/

	float leftUV = (float)(x - m_xPos) / (float)width;
	float rightUV = 1.0f-((float)(width - spriteWidth)) / (float)width;
	float topUV = (float)(y - m_yPos) / (float)height;
	float bottomUV = 1.0f-((float)(height - spriteHeight)) / (float)height;

	D3DXVECTOR2 UV[4] = {
		{ leftUV ,topUV},
		{ rightUV,topUV},
		{ rightUV,bottomUV},
		{ leftUV ,bottomUV}
	};

	CUSTOMVERTEX* verts = new CUSTOMVERTEX[4];

	verts[0].position = D3DXVECTOR3(tl_x, tl_y, 0.0f);
	verts[1].position = D3DXVECTOR3(tr_x, tr_y, 0.0f);
	verts[2].position = D3DXVECTOR3(br_x, br_y, 0.0f);
	verts[3].position = D3DXVECTOR3(bl_x, bl_y, 0.0f);

	verts[0].UV = UV[0];
	verts[1].UV = UV[1];
	verts[2].UV = UV[2];
	verts[3].UV = UV[3];

	verts[0].color = verts[1].color = verts[2].color = verts[3].color = m_color;

	return verts;
}

int CSprite::GetWidth()
{
	if (!m_pTexture)
		return -1;
	return m_pTexture->GetWidth()*m_xScale;
}

int CSprite::GetHeight()
{
	if (!m_pTexture)
		return -1;
	return m_pTexture->GetHeight()* m_yScale;
}

int CSprite::GetX()
{
	return m_xPos;
}

int CSprite::GetY()
{
	return m_yPos;
}

float CSprite::GetScaleY()
{
	return m_yScale;
}

float CSprite::GetScaleX()
{
	return m_xScale;
}

float CSprite::GetRotation()
{
	return m_rotation;
}

void CSprite::SetRectLock(RECT r)
{
	m_areaLock = r;
}

bool CSprite::WithinLock()
{
	int width = GetWidth() * GetScaleX();
	int height = GetHeight() * GetScaleY();

	if (m_areaLock.bottom != -1 && m_areaLock.left != -1 &&
		m_areaLock.right != -1 && m_areaLock.top != -1)
	{
		if (m_xPos + width < m_areaLock.left || m_xPos > m_areaLock.right ||
			m_yPos > m_areaLock.bottom || m_yPos + height < m_areaLock.top)
			return false;
	}

	if (m_xPos > SYSTEM::GetWindowWidth() || m_yPos > SYSTEM::GetWindowHeight() ||
		m_xPos + width < 0 || m_yPos + height < 0)
		return false;

	return true;
}

void CSprite::SetPosition(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
}

void CSprite::SetScaleX(float x)
{
	m_xScale = x;
}

void CSprite::SetScaleY(float y)
{
	m_yScale = y;
}

void CSprite::SetScale(float x, float y)
{
	m_xScale = x;
	m_yScale = y;
}

void CSprite::SetColor(float r, float g, float b, float a)
{
	m_color = D3DXVECTOR4(r, g, b, a);
}

void CSprite::SetColor(D3DXVECTOR4 color)
{
	m_color = color;
}

void CSprite::SetAnchor(float x, float y)
{
	m_anchorX = x;
	m_anchorY = y;
}

void CSprite::SetRotation(float r)
{
	m_rotation = r;
}

void CSprite::Resize(int width, int height)
{
	if (!m_pTexture)
		return;

	m_xScale = (float)width / (float)m_pTexture->GetWidth();
	m_yScale = (float)height / (float)m_pTexture->GetHeight();
}
