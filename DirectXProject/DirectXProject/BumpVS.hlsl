// input
struct VS_IN
{
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float3 tangent : TANGENT0;
};
// output
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 texSpaceLight : TEXCOORD2;
};

cbuffer Matrix : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};

cbuffer Light : register(b1)
{
	float4 direction;
};


VS_OUT main(VS_IN vin)
{
	VS_OUT vout;

	// 3D -> 2D
	vout.pos = float4(vin.pos, 1.0f);
	vout.pos = mul(vout.pos, world);
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);


	vout.uv = vin.uv;

	vout.normal = vin.normal;
	// ワールド空間のライトをテクスチャ空間に置きなおす
	// 法線と接ベクトルから従法線を求める
	float3 N = normalize(vin.normal);
	float3 T = normalize(vin.tangent);
	float3 B = normalize(cross(N,T));
	// テクスチャ空間からワールド空間に変換する姿勢行列を求め、ライトに対してその逆行列をかける
	float3x3 invTexToWorld =
		float3x3
		(
			T.x, B.x, N.x,
			T.y, B.y, N.y,
			T.z, B.z, N.z
		);

	float3 L = normalize(direction.xyz);
	vout.texSpaceLight = 0;//aaa(L,invTexToWorld)
	return vout;
}