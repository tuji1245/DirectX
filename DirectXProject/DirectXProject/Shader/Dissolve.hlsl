/*ディゾルブ
* Aの画像をBへ変化させる表現
*/



struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

cbuffer Parameter : register(b2)
{
	float time;
	float3 NotUsed; // NoUse!!!!
};

Texture2D baseTex : register(t0); // 通常テクスチャ ,Viewportのテクスチャのほうがいいかも
Texture2D dissolveTex : register(t1);

SamplerState samp: register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.f, 1.f, 1.f, 1.f);

	color = baseTex.Sample(samp, pin.uv);

	// フェードのみ = 透過色飲み変化させる
	//color.a = time;

	// テクスチャの色を(0->1,黒->白)としたとき、経過時間と比較して小さい値は表示されて大きい値は
	// 表示されないと判定出来れば、画像を用いたフェードができる
	float dissolve = dissolveTex.Sample(samp, pin.uv).r;
	// step(a,b) = (a <= b ? 1 : 0);
	color.a = step(dissolve, time);//value.x);	

	return color;
}