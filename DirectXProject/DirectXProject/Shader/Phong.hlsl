/* PhongShadeing
* Lambert�ɋ��ʔ��˂�����������
* �������˂��Ă܂Ԃ��������邽�߂ɂ͊ϑ��҂��K�v
* 3D�Q�[���ł̓J�������ϑ��҂ƂȂ�B
* ���̔��˂Ȃ̂Ō��͈�x�I�u�W�F�N�g�ɓ������Ĕ��˂��Ȃ���΂Ȃ�Ȃ�
* ���˕Ԃ���̓I�u�W�F�N�g�Ɍ���������p�x�ɂ���ĕς��
* �s�N�Z�����Ƃɔ��˂����������߂�
* 
* ���̎����˂������ƃJ�����̎����̃x�N�g���̊p�x���󂢂Ƃ��܂Ԃ��������� 
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

	float3 N = normalize(pin.normal);
	float3 L = normalize(direction.xyz);
	// ���ڂ��Ă���x�N�g���ւ̃x�N�g��
	float3 V = normalize(eyePos - pin.worldPos);
	// ���˃x�N�g��(�g�ݍ��݊֐��Ōv�Z
	// ���˒n�_�̖@���x�N�g���Ōv�Z����
	float3 R = reflect(L, N);
	R = normalize(R);

	// ---���ʔ��˂̌v�Z
	// �����������Ă܂Ԃ��������镔�� - �X�؃L����
	float s = dot(V, R) * 0.5f + 0.5f;
	// ���̂܂܂��ƍL�͈͂����邭�����Ă��܂��̂ŁA�}����K�v������
	// ���`�Ŗ��邳���ω�����̂ł͂Ȃ��A�J�[�u��`���悤�ȕω����]�܂���
	// �ݏ�̃O���t�ł������������
	// 20��ʂ����傤�ǂ����H
	s = pow(s, 20);

	// �����o�[�g(�e)�̌v�Z
	float d = dot(N, -L) + 1.0f * 0.5f;
	
	//�����o�[�g�Ƌ��ʔ��˂�����
	color.rgb = d + s;
	return color;
}