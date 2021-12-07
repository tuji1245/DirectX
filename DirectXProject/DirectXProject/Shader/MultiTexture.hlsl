/*1つのモデルに複数のテクスチャ
傷や汚れを表現する際、新しいく傷や汚れのテクスチャを貼り付ける
シェーダ内で複数のテクスチャから色を読み取り、それぞれ合成した結果を種強くする。

*/




struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;	
};

Texture2D baseTex : register(t0); // 通常テクスチャ
Texture2D scarTex : register(t1); // 上から貼るテクスチャ

SamplerState samp: register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.f, 1.f, 1.f, 1.f);

	color = baseTex.Sample(samp, pin.uv);

	// 正しく重ね合わせを行うために、自前でアルファブレンドを行う
	// アルファブレンド dst.rgb * (1.f - src.a) + src.rgb * src.a;
	float4 blendColor = scarTex.Sample(samp, pin.uv);
	color.rgb = color.rgb * (1.f - blendColor.a) + blendColor.rgb * blendColor.a;

	color.a = 1.f;
	return color;
}