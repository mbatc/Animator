#pragma once

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "D3DX11.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <D3DX11async.h>

#include "CShaderOrthographic.h"
//#include "CMaterial.h"

class CSprite;

struct CUSTOMVERTEX
{
	D3DXVECTOR3 position;
	D3DXVECTOR4 color;
	D3DXVECTOR2 UV;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 tangent;
	D3DXVECTOR3 bitangent;
};

struct D3DDevice
{
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
};

struct GFXMatrices
{
	D3DXMATRIX projection;
	D3DXMATRIX ortho;
	D3DXMATRIX view;
};

class CD3DGraphics
{
public:
	CD3DGraphics();
	~CD3DGraphics();

	bool Init(HWND hWnd, UINT clientWidth, UINT clientHeight);

	bool BeginFrame();
	bool EndFrame();

	bool RenderSprite(ID3D11ShaderResourceView* tex, D3DXMATRIX world = D3DXMATRIX());
	bool RenderOrthographicShader(ID3D11ShaderResourceView* tex, D3DXMATRIX world = D3DXMATRIX());
	void SetBlendState(bool state);
	void SetDepthBufferState(bool state);

	D3DDevice&	GetDevice()								{return m_device;}
	void		GetProjectionMatrix(D3DXMATRIX& out)	{out = m_matrices.projection; }
	void		GetOrthoMatrix(D3DXMATRIX& out)			{out = m_matrices.ortho; }
	void		GetViewMatrix(D3DXMATRIX& out)			{out = m_matrices.view; }

	GFXMatrices& GetMatrices() {return m_matrices;}
	int	GetClientHeight() { return m_clientHeight; }
	int GetClientWidth() { return m_clientWidth; }

	void GetVideoCardInfo(char* desc, int& size);

	static CSprite DEFAULTSPRITE;
private:
	bool InitDefaultTexture();

private:
	UINT m_clientWidth, m_clientHeight;
	int m_videoCardMemory;
	char m_videoCardDesc[128];
	bool m_vsync;

	D3DDevice					m_device;

	IDXGISwapChain*				m_pSwapChain;
	ID3D11RenderTargetView*		m_pRenderTargetView;
	ID3D11Texture2D*			m_pDepthStencilBuffer;
	ID3D11DepthStencilView*		m_pDepthStencilView;
	ID3D11DepthStencilState*	m_pDepthStencilState;
	ID3D11DepthStencilState*	m_pDepthStencilStateDisabled;
	ID3D11RasterizerState*		m_pRasterState;
	ID3D11BlendState*			m_pAlphaBlendEnabled;
	ID3D11BlendState*			m_pAlphaBlendDisabled;

	CShaderOrthographic		m_orthoShader;

	ID3D11ShaderResourceView*   m_pDefaultTexture;
	ID3D11Buffer*				m_pSpriteIndexBuffer;
	GFXMatrices m_matrices;
};