#ifdef _WIN32 || _WIN64

#include "CD3DGraphics.h"

#include "CShader.h"
#include "Helper.h"

#include <stdio.h>
#include <fstream>

CShader::CShader()
	:
	m_pPixelShader(NULL),
	m_pVertexShader(NULL),
	m_pInputLayout(NULL),
	m_pMatrixBuffer(NULL)
{}

CShader::~CShader()
{
	MEMORY::DX::SafeRelease(m_pMatrixBuffer);
	MEMORY::DX::SafeRelease(m_pInputLayout);
	MEMORY::DX::SafeRelease(m_pPixelShader);
	MEMORY::DX::SafeRelease(m_pVertexShader);}

bool CShader::CompileShaders(CD3DGraphics& gfx, char* vs, char* ps, char* vsEntry, char* psEntry)
{
	HRESULT result;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	ID3D10Blob* errorMessage = NULL;

	result = D3DX11CompileFromFile(vs,NULL,NULL,vsEntry,"vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);

	if(FAILED(result))
	{
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, vs);
		}
		else
		{
			MessageBox(NULL,"Could Not Find Vertex Shader File!", vs, MB_OK );
		}
		return false;
	}

	result = D3DX11CompileFromFile(ps,NULL,NULL,psEntry,"ps_5_0",D3D10_SHADER_ENABLE_STRICTNESS,
		0,NULL, &pixelShaderBuffer, &errorMessage, NULL);

	if(FAILED(result))
	{
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, ps);
		}
		else
		{
			MessageBox(NULL,"Could Not Find Pixel Shader File!", ps, MB_OK );
		}
		return false;
	}

	result = gfx.GetDevice().pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader );
	if(FAILED(result))
		return false;

	result = gfx.GetDevice().pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader );
	if(FAILED(result))
		return false;

	if(!CreateInputLayout(gfx,vertexShaderBuffer,pixelShaderBuffer))
		return false;

	return true;
}

void CShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, char* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream fout;

	compileErrors = (char*)(errorMessage->GetBufferPointer());
	bufferSize = errorMessage->GetBufferSize();

	fout.open("shader-error.txt");
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}
	fout.close();

	MEMORY::DX::SafeRelease(errorMessage);
	MessageBox(NULL, "Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}
	
bool CShader::CreateMatrixBuffer(CD3DGraphics& gfx)
{
	HRESULT result;
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = gfx.GetDevice().pDevice->CreateBuffer(&matrixBufferDesc,NULL, &m_pMatrixBuffer);
	if(FAILED(result))
		return false;

	return true;
}

bool CShader::SetMatrixBuffer(CD3DGraphics& gfx, D3DXMATRIX view, D3DXMATRIX projection, D3DXMATRIX world, D3DXMATRIX ortho)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBuffer dataPtr;
	D3DXMATRIX tempMat;

	D3DXMatrixMultiply(&tempMat, &world, &view);
	D3DXMatrixMultiply(&tempMat, &tempMat, &projection);
	D3DXMatrixTranspose(&tempMat,&tempMat);
	D3DXMatrixTranspose(&view,&view);
	D3DXMatrixTranspose(&projection, &projection);
	D3DXMatrixTranspose(&world, &world);
	D3DXMatrixTranspose(&ortho, &ortho);

	dataPtr.world = world;
	dataPtr.view = view;
	dataPtr.projection = projection;
	dataPtr.render = tempMat;
	dataPtr.ortho = ortho;

	result = gfx.GetDevice().pDeviceContext->Map(m_pMatrixBuffer,0,D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		return false;

	*((MatrixBuffer*)mappedResource.pData) = dataPtr;

	gfx.GetDevice().pDeviceContext->Unmap(m_pMatrixBuffer,0);
	gfx.GetDevice().pDeviceContext->VSSetConstantBuffers(0,1,&m_pMatrixBuffer);

	return true;
}

#endif