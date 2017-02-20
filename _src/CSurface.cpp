#include "CSurface.h"
#include "Helper.h"

#include <gdiplus.h>

CSurface::CSurface() 
	:
	m_pPixels(NULL),
	m_width(-1),
	m_height(-1)
{}
CSurface::~CSurface() {}

void CSurface::Init(char* filename)
{
	Gdiplus::GdiplusStartupInput startupInput;
	ULONG_PTR token;
	GdiplusStartup(&token, &startupInput, NULL);

	int len = strlen(filename);
	wchar_t* temp = new wchar_t[len + 1];
	for (int i = 0; i < len; i++)
	{
		temp[i] = filename[i];
	}
	temp[len] = '\0';
	Gdiplus::Bitmap bitmap(temp);
	Gdiplus::Color pixel;

	if (bitmap.GetWidth() <= 0)
		return;
	if (bitmap.GetHeight() <= 0)
		return;

	m_width = bitmap.GetWidth();
	m_height = bitmap.GetHeight();

	m_pPixels = new D3DXVECTOR4[m_width * m_height];

	for (int x = 0; x < m_width; x++)
	{
		for (int y = 0; y < m_height; y++)
		{
			bitmap.GetPixel(x, y, &pixel);
			m_pPixels[x + y * m_width] = D3DXVECTOR4(pixel.GetR()/255.0f, pixel.GetG() / 255.0f, pixel.GetB() / 255.0f, pixel.GetA() / 255.0f);
		}
	}

	if (!m_pPixels)
		return;
}

void CSurface::Init(D3DXVECTOR4* pPixels, int width, int height)
{
	if (height < 0)
		return;
	if (width < 0)
		return;

	m_width = width;
	m_height = height;

	MEMORY::SafeDeleteArr(m_pPixels);
	m_pPixels = new D3DXVECTOR4[m_width * m_height];

	for (int i = 0; i < m_width * m_height; i++)
	{
		m_pPixels[i] = pPixels[i];
	}
}

int CSurface::GetHeight()
{
	return m_height;
}

int CSurface::GetWidth()
{
	return m_width;
}

D3DXVECTOR4* CSurface::GetPixels()
{
	return m_pPixels;
}