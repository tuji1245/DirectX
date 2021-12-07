struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
};

// CPUから渡されたテクスチャを
// 利用するための宣言
Texture2D tex : register(t0);
// テクスチャの繰り返し設定も必要
SamplerState samp : register(s0);

cbuffer Light : register(b0)
{
	float4 direction;
}


float4 main(PS_IN pin) : SV_Target
{
	float4 color = float4(1, 1, 1, 1);
	//color = tex.Sample(samp, pin.uv);

	float3 N = normalize(pin.normal);
	float3 L = -direction.xyz; // 太陽の向きを反転しておくと内積結果が反転するのでこうしておくといい。影はマイナスのほうが便利

//	color.rgb = pin.normal;
//	color.a = 1.0f;
//	color.rgb = (color.rgb + 1.f) * 0.5f;

//	color.rgb = direction;
//	color.a = 1.0f;
	float diffuse = dot(N, L);
	diffuse = (diffuse + 1.0f) * 0.5f;


	color.rgb *= diffuse;


	return color;
}
