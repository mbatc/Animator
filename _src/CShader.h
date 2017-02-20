#ifdef _WIN32 || _WIN64

#pragma once

#define MAX_DIR_LIGHTS 4
#define MAX_SPOT_LIGHTS 4
#define MAX_POINT_LIGHTS 16

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

class CD3DGraphics;
class CGameobjectList;

class CShader
{
	struct MatrixBuffer
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
		D3DXMATRIX render;
		D3DXMATRIX ortho;
	};
public:
	CShader();
	~CShader();

protected:
	//Creates the matrix buffer for the shader (world, view, perspective, ortho)
	bool CreateMatrixBuffer(CD3DGraphics& gfx);
	//Compiles the shaders at runtime
	bool CompileShaders(CD3DGraphics& gfx, char* vs, char* ps, char* vsEntry, char* psEntry);
	//Outputs an error message to a file
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, char* shaderFilename);
	//Sets the matrix buffer in a shader
	bool SetMatrixBuffer(CD3DGraphics& gfx, D3DXMATRIX view, D3DXMATRIX projection, D3DXMATRIX world, D3DXMATRIX ortho);
	
	//Creates the layout of the vertex data passed to the shaders
	virtual bool CreateInputLayout(CD3DGraphics& gfx, ID3D10Blob* vs, ID3D10Blob* ps) = 0;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader*	m_pPixelShader;
	ID3D11InputLayout*	m_pInputLayout;
	ID3D11Buffer*		m_pMatrixBuffer;
};
#endif
