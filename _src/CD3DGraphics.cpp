#include "CD3DGraphics.h"
#include "CWindow.h"
#include "CSystem.h"
#include "Helper.h"
#include "CSprite.h"
#include "CGameobjectList.h"

CSprite CD3DGraphics::DEFAULTSPRITE;

CD3DGraphics::CD3DGraphics()
	:
	m_pRenderTargetView(NULL),
	m_pRasterState(NULL),
	m_pDepthStencilBuffer(NULL),
	m_pDepthStencilView(NULL),
	m_pDepthStencilState(NULL),
	m_pSwapChain(NULL),
	m_pAlphaBlendEnabled(NULL),
	m_pAlphaBlendDisabled(NULL),
	m_pDefaultTexture(NULL)
{
	m_device.pDevice = NULL;
	m_device.pDeviceContext = NULL;

	D3DXMatrixIdentity(&m_matrices.ortho);
	D3DXMatrixIdentity(&m_matrices.projection);

	m_vsync = false;
}

CD3DGraphics::~CD3DGraphics()
{
	MEMORY::DX::SafeRelease(m_pRasterState);
	MEMORY::DX::SafeRelease(m_pDepthStencilView);
	MEMORY::DX::SafeRelease(m_pDepthStencilState);
	MEMORY::DX::SafeRelease(m_pDepthStencilBuffer);
	MEMORY::DX::SafeRelease(m_pRenderTargetView);
	MEMORY::DX::SafeRelease(m_device.pDeviceContext);
	MEMORY::DX::SafeRelease(m_device.pDevice);
	MEMORY::DX::SafeRelease(m_pSwapChain);
	MEMORY::DX::SafeRelease(m_pAlphaBlendEnabled);
	MEMORY::DX::SafeRelease(m_pAlphaBlendDisabled);
	MEMORY::DX::SafeRelease(m_pDefaultTexture);
}

bool CD3DGraphics::Init(HWND hWnd, UINT clientWidth, UINT clientHeight )
{
	cout << "CD3DGraphics: Client Height	" << clientHeight <<"\n";
	cout << "CD3DGraphics: Client Width		" << clientWidth << "\n";
	m_clientHeight = clientHeight;
	m_clientWidth = clientWidth;
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* pBackBuffer;
	IDXGIFactory* pFactory;
	IDXGIAdapter* pAdapter;
	IDXGIOutput* pAdapterOutput;
	D3D11_VIEWPORT viewport;

	DXGI_MODE_DESC* pDisplayModeList;
	DXGI_ADAPTER_DESC adapterDesc;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDisabledDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_BLEND_DESC blendDesc;

	UINT nAdapterModes;

	int RRNumerator = 0, RRDenominator = 1;

	if(FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory),(void**)&pFactory))){return false;}
	if(FAILED(pFactory->EnumAdapters(0,&pAdapter))){return false;}
	if(FAILED(pAdapter->EnumOutputs(0,&pAdapterOutput))){return false;}
	if(FAILED(pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &nAdapterModes, NULL))){return false;}
	
	pDisplayModeList = new DXGI_MODE_DESC[nAdapterModes];
	if(!pDisplayModeList){ return false; }
	
	if(FAILED(pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &nAdapterModes, pDisplayModeList ))){return false;}

	if(m_vsync)
	{
		for(UINT i = 0; i < nAdapterModes; i++)
		{
			if( pDisplayModeList[i].Width == screenWidth &&
				pDisplayModeList[i].Height == screenHeight )
			{
				RRNumerator = pDisplayModeList[i].RefreshRate.Numerator;
				RRDenominator = pDisplayModeList[i].RefreshRate.Denominator;
				break;
			}
		}
	}

	if(FAILED(pAdapter->GetDesc(&adapterDesc))){return false;}
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024 );
	if(wcstombs_s(NULL, m_videoCardDesc,128,adapterDesc.Description,128) != 0){return false;}

	MEMORY::DX::SafeRelease(pAdapterOutput);
	MEMORY::DX::SafeRelease(pAdapter);
	MEMORY::DX::SafeRelease(pFactory);

	ZeroMemory(&swapChainDesc,sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.Flags = 0;
	swapChainDesc.Windowed = true;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Height = clientHeight;
	swapChainDesc.BufferDesc.Width = clientWidth;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = RRDenominator;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = RRNumerator;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	featureLevel = D3D_FEATURE_LEVEL_11_0;
	UINT flags = 0;

#ifdef _DEBUG
	//flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	if(FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,NULL, flags,&featureLevel,1, 
		D3D11_SDK_VERSION,&swapChainDesc,&m_pSwapChain,
		&m_device.pDevice,NULL,&m_device.pDeviceContext))) {return false;}

	if(FAILED(m_pSwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),(void**)&pBackBuffer))){return false;}
	if(FAILED(m_device.pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView))){return false;}
	MEMORY::DX::SafeRelease(pBackBuffer);

	ZeroMemory(&depthBufferDesc,sizeof(D3D11_TEXTURE2D_DESC));
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.Height = clientHeight;
	depthBufferDesc.Width = clientWidth;
	depthBufferDesc.MiscFlags = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	
	if(FAILED(m_device.pDevice->CreateTexture2D(&depthBufferDesc,NULL,&m_pDepthStencilBuffer))){return false;}
	
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;

	if(FAILED(m_device.pDevice->CreateDepthStencilState(&depthStencilDesc,&m_pDepthStencilState))){return false;}

	ZeroMemory(&depthStencilDisabledDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDisabledDesc.DepthEnable = false;
	depthStencilDisabledDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDisabledDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDisabledDesc.StencilEnable = true;
	depthStencilDisabledDesc.StencilReadMask = 0xFF;
	depthStencilDisabledDesc.StencilWriteMask = 0xFF;

	depthStencilDisabledDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDisabledDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDisabledDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDisabledDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;

	depthStencilDisabledDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDisabledDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDisabledDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDisabledDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;

	if (FAILED(m_device.pDevice->CreateDepthStencilState(&depthStencilDisabledDesc, &m_pDepthStencilStateDisabled))) { return false; }

	m_device.pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState,1);

	ZeroMemory(&depthStencilViewDesc,sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if(FAILED(m_device.pDevice->CreateDepthStencilView(m_pDepthStencilBuffer,
		&depthStencilViewDesc,&m_pDepthStencilView ))) {return false; }

	ZeroMemory(&blendDesc,sizeof(D3D11_BLEND_DESC));
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	if(FAILED(m_device.pDevice->CreateBlendState(&blendDesc,&m_pAlphaBlendEnabled)))
		return false;

	blendDesc.RenderTarget[0].BlendEnable = false;
	if(FAILED(m_device.pDevice->CreateBlendState(&blendDesc, &m_pAlphaBlendDisabled)))
		return false;

	rasterDesc.AntialiasedLineEnable = true;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = true;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	if(FAILED(m_device.pDevice->CreateRasterizerState(&rasterDesc,&m_pRasterState))){return false;}
	m_device.pDeviceContext->RSSetState(m_pRasterState);

	viewport.Height = (float)clientHeight;
	viewport.Width = (float)clientWidth;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	m_device.pDeviceContext->OMSetRenderTargets(1,&m_pRenderTargetView,m_pDepthStencilView);
	m_device.pDeviceContext->RSSetViewports(1,&viewport);
	
	float fov, screenAspect;
	fov = (float)D3DX_PI / 4.0f;
	screenAspect = (float)clientWidth / (float)clientHeight;

	D3DXMatrixPerspectiveFovLH(&m_matrices.projection,fov,screenAspect,0.1f,1000.0f);
	D3DXMatrixOrthoLH(&m_matrices.ortho,(float)clientWidth,(float)clientHeight,0.1f,1000.0f);

	if (!InitDefaultTexture())
		return false;
	if(!m_orthoShader.Init(*this))
		return false;

	return true;
}

bool CD3DGraphics::BeginFrame()
{
	D3DXCOLOR c(0xFF222266);

	m_device.pDeviceContext->ClearRenderTargetView(m_pRenderTargetView,c);
	m_device.pDeviceContext->ClearDepthStencilView(m_pDepthStencilView,D3D11_CLEAR_DEPTH, 1.0f,0);

	return true;
}

bool CD3DGraphics::EndFrame()
{
	if(m_vsync)
	{
		if(FAILED(m_pSwapChain->Present(0,1)))
		{
			return false;
		}
	}
	else
	{
		if(FAILED(m_pSwapChain->Present(0,0)))
		{
			return false;
		}
	}
	return true;
}

bool CD3DGraphics::RenderSprite(ID3D11ShaderResourceView* tex, D3DXMATRIX world)
{
	return RenderOrthographicShader(tex,world);
}

bool CD3DGraphics::RenderOrthographicShader(ID3D11ShaderResourceView* tex, D3DXMATRIX world)
{
	return m_orthoShader.Render(*this,m_matrices.ortho,tex, world);
}

void CD3DGraphics::GetVideoCardInfo(char* desc, int& size)
{
	strcpy_s(desc, 128, m_videoCardDesc);
	size = m_videoCardMemory;
}

bool CD3DGraphics::InitDefaultTexture()
{
	MEMORY::DX::SafeRelease(m_pDefaultTexture);

	ID3D11Texture2D * p2DTex;
	D3D11_TEXTURE2D_DESC desc;
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	D3D11_SUBRESOURCE_DATA initialData;

	initialData.pSysMem = new D3DXVECTOR4(0.7f, 0.7f, 0.7f, 1.0f);
	initialData.SysMemPitch = sizeof(D3DXVECTOR4);
	initialData.SysMemSlicePitch = 0;

	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.Height = 1;
	desc.Width = 1;
	desc.MipLevels = 1;
	desc.MiscFlags = 0;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;

	viewDesc.Texture2D.MipLevels = 1;
	viewDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

	m_device.pDevice->CreateTexture2D(&desc, &initialData, &p2DTex);
	m_device.pDevice->CreateShaderResourceView(p2DTex, NULL, &m_pDefaultTexture);

	MEMORY::SafeDelete(initialData.pSysMem);

	if (m_pDefaultTexture)
		return true;
	else
		return false;
}


void CD3DGraphics::SetBlendState(bool state)
{
	float blendFactor[4] = {0.0f,0.0f,0.0f,0.0f};
	if(state)
		m_device.pDeviceContext->OMSetBlendState(m_pAlphaBlendEnabled,blendFactor,0xFFFFFF);
	else
		m_device.pDeviceContext->OMSetBlendState(m_pAlphaBlendDisabled,blendFactor,0xFFFFFF);
}

void CD3DGraphics::SetDepthBufferState(bool state)
{
	if (state)
	{
		m_device.pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
	}
	else
	{
		m_device.pDeviceContext->OMSetDepthStencilState(m_pDepthStencilStateDisabled, 1);
	}
}
