Texture2D shaderTexture;
SamplerState SampleType;

struct PixelInputType
{
	float4 position	: SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
};

float4 PixelOrthoShader(PixelInputType IN) : SV_TARGET
{
	return shaderTexture.Sample(SampleType,IN.tex) * IN.color;
}