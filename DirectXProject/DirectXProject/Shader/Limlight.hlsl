/* �������C�g
*  ���̂̌�납������������ė֊s�������Č�����悤�ɋ��������\��
*/

struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 worldPos : TEXCOORD2;
};

cbuffer Light : register(b0)
{
	float4 direction;
}

cbuffer Camera : register(b1)
{
	float4 eyePos;
}

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.f, 1.f, 1.f, 1.f);
	// �v�Z�ɕK�v�ȃp�����[�^
	float3 L = -normalize(direction.xyz);		 // �����x�N�g��
	float3 V = normalize(pin.worldPos - eyePos); // �����x�N�g��
	float3 N = normalize(pin.normal);

	// �������C�g
	float lim = 1 - dot(L, N);
	lim *= 1 -dot(V, L);
	lim *= 1 -dot(V, N);

	// lim = dot L, V
	// edge =  dot N, V

    // lim = pow(lim,7)
	// lim * edge
	color.rgb = lim;


	// �l�@:
	//   �@���x�N�g���ƃJ�����x�N�g���������ɋ߂��Ƃ����邭����H
	// ���_
	// �@�@���ƌ����̓��ς������ɋ߂��@���邢
	// �@�����ƌ����̓��ς������ɋ߂��@���邢
	// �@�����Ɩ@���̓��ς������ɋ߂��@���邢�@new!!
	// �@���̓���g���Ė��邳�𓱂��o��


	return color;
}