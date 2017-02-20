#ifdef _WIN32 || _WIN64

#pragma once

#include "CShader.h"

class CShaderOrthographic : public CShader
{
public:
	CShaderOrthographic();
	~CShaderOrthographic();

	bool Init(CD3DGraphics& gfx);
	bool Render(CD3DGraphics& gfx, D3DXMATRIX ortho, ID3D11ShaderResourceView* texture, D3DXMATRIX world = D3DXMATRIX());

private:
	//Sets the data need for the shader to render
	bool SetShaderConstants(CD3DGraphics& gfx, D3DXMATRIX ortho, 
		ID3D11ShaderResourceView* texture, D3DXMATRIX world);
	//See CShader definition
	bool CreateInputLayout(CD3DGraphics& gfx, ID3D10Blob* vs, ID3D10Blob* ps);

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11SamplerState* m_pSampleState;

	static const int m_nIndices = 6;
};
#endif
