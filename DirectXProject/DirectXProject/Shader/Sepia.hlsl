struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 color : TEXCOORD2;
};

// CPUから渡されたテクスチャを
// 利用するための宣言
Texture2D tex : register(t0);
// テクスチャの繰り返し設定も必要
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_Target
{
	float4 color = float4(1, 0, 0, 1);
	color = tex.Sample(samp, pin.uv);
	
	// セピア変換
	// モノクロ変換後、赤色成分が多くなるように数値を変換する
	float mono = color.r * 0.3f + color.g * 0.6f + color.b * 0.1f;
	color.rgb = mono;

	color.r *= 1.f;
	color.g *= 0.7f;
	color.b *= 0.5f;
	//color.r *= 1.f;
	//color.g *= 0.7f;
	//color.b *= 0.4f;


	return color;	
}
