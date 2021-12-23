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

	// �J�����Ƃ̋����𒲂ׂ邽�߂ɁA���O�Ƀ��[���h���W���v�Z
	float4 worldPos = mul(vout.pos, world);
	// �J�����Ƃ̋���
	float distance = length(worldPos.xyz - eyePos.xyz);
	
	// �֊s���Ƃ��ĕ\�����邽�߁A�@�������Ƀ��f����傫������B
	// �܂��A�J�����Ƃ̋����ɉ����āA�@�������ւ̈ړ��ʂ�ς��鎖�ɂ����
	// �J����������Ă����̑����ŗ֊s�����\�������
	float margin = distance / 100.f;
	vout.pos.xyz += normalize(vin.normal) * margin;

	vout.pos = mul(vout.pos, world);
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);

	return vout;
}