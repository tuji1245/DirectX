struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 texSpaceLight : TEXCOORD2;
};

// テクスチャ
Texture2D baseTex : register(t0);
Texture2D normalTex : register(t1);
SamplerState samp : register(s1);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	// バンプマッピングはテクスチャに保存されているデータを法線ベクトルとして扱う
	float3 N = normalTex.Sample(samp, pin.uv) * 2.0f - 1.0f;

	// 本来であれば、法線マップの法線をワルド空間に変換するが、
	// ピクセルシェーダで行うと計算量が増える。
	// そのため、ワールド空間のライトをテクスチャ空間に移動させて計算を行う。
	// ライトをテクスチャ空間に移動する処理を頂点シェーダで行うため計算量が少なくなる

	float diffuse = dot(N, texSpaceLight);
	diffuse = (diffuse + 1.0f) * 0.5f;
	color *= diffuse;

	return color;
}