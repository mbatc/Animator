#include "CTexture.h"
#include "Helper.h"

CTexture::CTexture()
	:
	m_pTexture(NULL),
	m_height(-1),
	m_width(-1)
{}

CTexture::~CTexture()
{
	MEMORY::DX::SafeRelease(m_pTexture);
}

bool CTexture::InitTexture(CD3DGraphics& gfx)
{
	if(!m_pCurrentFile)
		return false;

	MEMORY::DX::SafeRelease(m_pTexture);
	HRESULT result;
	result = D3DX11CreateShaderResourceViewFromFile(gfx.GetDevice().pDevice,m_pCurrentFile,NULL,NULL,
		&m_pTexture,NULL);
	if(result == D3D11_ERROR_FILE_NOT_FOUND)
	{
		cout << "CTexture: Could Not Find Texture File '" << m_pCurrentFile << "'\n";
		return false;
	}

	ID3D11Texture2D* tex;
	D3D11_TEXTURE2D_DESC desc;
	m_pTexture->GetResource((ID3D11Resource**)&tex);
	tex->GetDesc(&desc);

	m_height = desc.Height;
	m_width = desc.Width;

	//Game Assets\Models\Texture Bank

	return true;
}

bool CTexture::InitTexture(CSurface surface, CD3DGraphics& gfx)
{
	if (!surface.GetPixels())
		return false;
	if (surface.GetHeight() <= 0)
		return false;
	if (surface.GetWidth() <= 0)
		return false;

	D3D11_TEXTURE2D_DESC texDesc;
	D3D11_SUBRESOURCE_DATA data;
	ID3D11Texture2D* tex;

	data.pSysMem = surface.GetPixels();
	data.SysMemPitch = sizeof(D3DXVECTOR4) * surface.GetWidth();
	data.SysMemSlicePitch = 0;

	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.Height = surface.GetHeight();
	texDesc.Width = surface.GetWidth();
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	HRESULT result;

	result = gfx.GetDevice().pDevice->CreateTexture2D(&texDesc, &data, &tex);
	if (FAILED(result))
	{
		cout << "CTexture: Could not create texture from CSurface...\n";
		return false;
	}

	result = gfx.GetDevice().pDevice->CreateShaderResourceView(tex, NULL, &m_pTexture);
	if (FAILED(result))
	{
		cout << "CTexture: Could not create Shader Resource View from Texture...\n";
		return false;
	}

	m_height = texDesc.Height;
	m_width = texDesc.Width;

	return true;
}