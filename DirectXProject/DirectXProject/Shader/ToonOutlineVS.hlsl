// input
struct VS_IN
{
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
};
// output
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
};

cbuffer Matrix : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};
cbuffer Camera : register(b1)
{
	float4 eyePos;
};

VS_OUT main(VS_IN vin)
{
	VS_OUT vout;

	vout.pos = float4(vin.pos, 1.0f);

	// カメラとの距離を調べるために、事前にワールド座標を計算
	float4 worldPos = mul(vout.pos, world);
	// カメラとの距離
	float distance = length(worldPos.xyz - eyePos.xyz);
	
	// 輪郭線として表示するため、法線方向にモデルを大きくする。
	// また、カメラとの距離に応じて、法線方向への移動量を変える事によって
	// カメラが離れても一定の太さで輪郭線が表示される
	float margin = distance / 100.f;
	vout.pos.xyz += normalize(vin.normal) * margin;

	vout.pos = mul(vout.pos, world);
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);

	return vout;
}