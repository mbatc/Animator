#ifdef _WIN32 || _WIN64

#include "CShaderOrthographic.h"
#include "CD3DGraphics.h"
#include "Helper.h"

CShaderOrthographic::CShaderOrthographic()
	:
	m_pVertexBuffer(NULL),
	m_pIndexBuffer(NULL)
{}

CShaderOrthographic::~CShaderOrthographic()
{}

bool CShaderOrthographic::Init(CD3DGraphics& gfx)
{
	char ps[] = "Engine/Shaders/PixelOrthographicShader.hlsl";
	char vs[] = "Engine/Shaders/VertexOrthographicShader.hlsl";

	if(!CompileShaders(gfx,vs,ps,"VertexOrthoShader","PixelOrthoShader"))
		return false;

	CUSTOMVERTEX* t_vertexBuffer = new CUSTOMVERTEX[4];
	unsigned long* t_indexBuffer = new unsigned long[6];

	if(!t_vertexBuffer)
		return false;
	if(!t_indexBuffer)
		return false;

	ZeroMemory(t_vertexBuffer,sizeof(CUSTOMVERTEX)*4);

	t_vertexBuffer[0].position = D3DXVECTOR3(-1.0f,-1.0f,0.0f); //Bottom Left
	t_vertexBuffer[1].position = D3DXVECTOR3(-1.0f,1.0f,0.0f); //Top Left
	t_vertexBuffer[2].position = D3DXVECTOR3(1.0f,1.0f,0.0f); //Top Right
	t_vertexBuffer[3].position = D3DXVECTOR3(1.0f,-1.0f,0.0f); //Bottom Right

	t_vertexBuffer[0].UV = D3DXVECTOR2(0.0f,1.0f); //1.0f,1.0f
	t_vertexBuffer[1].UV = D3DXVECTOR2(0.0f,0.0f); //1.0f,0.0f
	t_vertexBuffer[2].UV = D3DXVECTOR2(1.0f,0.0f); //0.0f,0.0f
	t_vertexBuffer[3].UV = D3DXVECTOR2(1.0f,1.0f); //1.0f,1.0f

	t_indexBuffer[0] = 0;
	t_indexBuffer[1] = 1;
	t_indexBuffer[2] = 2;
	t_indexBuffer[3] = 0;
	t_indexBuffer[4] = 2;
	t_indexBuffer[5] = 3;

	MEMORY::DX::SafeRelease(m_pVertexBuffer);
	MEMORY::DX::SafeRelease(m_pIndexBuffer);

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	vertexBufferDesc.BindFlags =  D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(CUSTOMVERTEX) * 4;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	
	vertexData.pSysMem = t_vertexBuffer;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * 6;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = t_indexBuffer;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if(FAILED(gfx.GetDevice().pDevice->CreateBuffer(&vertexBufferDesc,&vertexData,&m_pVertexBuffer)))
	{
		return false;
	}
	if(FAILED(gfx.GetDevice().pDevice->CreateBuffer(&indexBufferDesc,&indexData,&m_pIndexBuffer)))
	{
		return false;
	}

	D3D11_SAMPLER_DESC sampleDesc;
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampleDesc.MipLODBias = 0.0f;
	sampleDesc.MaxAnisotropy = 1;
	sampleDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampleDesc.MinLOD = 0;
	sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;
	sampleDesc.BorderColor[0] = 0;
	sampleDesc.BorderColor[1] = 0;
	sampleDesc.BorderColor[2] = 0;
	sampleDesc.BorderColor[3] = 0;

	if(FAILED(gfx.GetDevice().pDevice->CreateSamplerState(&sampleDesc,&m_pSampleState)))
		return false;

	MEMORY::SafeDeleteArr(t_vertexBuffer);
	MEMORY::SafeDeleteArr(t_indexBuffer);

	if(!CreateMatrixBuffer(gfx))
		return false;

	return true;
}

bool CShaderOrthographic::CreateInputLayout(CD3DGraphics& gfx, ID3D10Blob* vs, ID3D10Blob* ps)
{
	unsigned int numElements;
	
	D3D11_INPUT_ELEMENT_DESC polygonLayout[] = 
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	numElements = sizeof(polygonLayout)/sizeof(polygonLayout[0]);

	gfx.GetDevice().pDevice->CreateInputLayout(polygonLayout,numElements,vs->GetBufferPointer(),vs->GetBufferSize(),
		&m_pInputLayout);

	return true;
}

bool CShaderOrthographic::Render(CD3DGraphics& gfx, D3DXMATRIX ortho, ID3D11ShaderResourceView* texture, D3DXMATRIX world)
{
	if(!SetShaderConstants(gfx,ortho,texture, world))
		return false;
	UINT stride = sizeof(CUSTOMVERTEX);
	UINT offset = 0;

	gfx.GetDevice().pDeviceContext->PSSetShader(m_pPixelShader,NULL,0);
	gfx.GetDevice().pDeviceContext->VSSetShader(m_pVertexShader,NULL,0);
	gfx.GetDevice().pDeviceContext->PSSetSamplers(0,1,&m_pSampleState);
	gfx.GetDevice().pDeviceContext->PSSetShaderResources(0,1,&texture);
	gfx.GetDevice().pDeviceContext->IASetInputLayout(m_pInputLayout);

	gfx.GetDevice().pDeviceContext->IASetIndexBuffer(m_pIndexBuffer,DXGI_FORMAT_R32_UINT,0);

	gfx.SetBlendState(true);
	gfx.SetDepthBufferState(false);
	gfx.GetDevice().pDeviceContext->DrawIndexed(m_nIndices,0,0);
	gfx.SetDepthBufferState(true);
	gfx.SetBlendState(false);
	return true;
}

bool CShaderOrthographic::SetShaderConstants(CD3DGraphics& gfx, D3DXMATRIX ortho, 
		ID3D11ShaderResourceView* texture, D3DXMATRIX world)
{
	return SetMatrixBuffer(gfx,D3DXMATRIX(),D3DXMATRIX(),world,ortho);
}

#endif