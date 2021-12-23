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

cbuffer Light : register(b0)
{
	float4 direction;
};

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f,1.0f,1.0f,1.0f);

	float3 N = normalize(pin.normal);
	float3 L = -direction.xyz;

	float diffuse = dot(N, L);
	diffuse = (diffuse + 1.0f) / 2.f;

	diffuse = rampTex.Sample(samp, float2(diffuse, 0.5f)).r;

	color.rgb = diffuse;
	return color;
}