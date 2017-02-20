cbuffer MatrixBuffer
{
	float4x4 world;
	float4x4 view;
	float4x4 projection;
	float4x4 render;
	float4x4 ortho;
};

struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position	: SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
};


PixelInputType VertexOrthoShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;
	output.position = mul(input.position, world);
	
	output.tex = input.tex;
	output.color = input.color;

	return output;
}