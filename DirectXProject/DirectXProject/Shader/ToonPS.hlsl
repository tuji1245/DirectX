struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 texSpaceLight : TEXCOORD2;
};

// テクスチャ
Texture2D baseTex : register(t0);
Texture2D rampTex : register(t1);
SamplerState samp : register(s1);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f,1.0f,1.0f,1.0f);

	diffuse = rampTex.Sample(samp, float2(diffuse, 0.5f)).r;

	color.rgb = diffuse;
	return color;
}