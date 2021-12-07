struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 texSpaceLight : TEXCOORD2;
};

// �e�N�X�`��
Texture2D baseTex : register(t0);
Texture2D normalTex : register(t1);
SamplerState samp : register(s1);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	// �o���v�}�b�s���O�̓e�N�X�`���ɕۑ�����Ă���f�[�^��@���x�N�g���Ƃ��Ĉ���
	float3 N = normalTex.Sample(samp, pin.uv) * 2.0f - 1.0f;

	// �{���ł���΁A�@���}�b�v�̖@���������h��Ԃɕϊ����邪�A
	// �s�N�Z���V�F�[�_�ōs���ƌv�Z�ʂ�������B
	// ���̂��߁A���[���h��Ԃ̃��C�g���e�N�X�`����ԂɈړ������Čv�Z���s���B
	// ���C�g���e�N�X�`����ԂɈړ����鏈���𒸓_�V�F�[�_�ōs�����ߌv�Z�ʂ����Ȃ��Ȃ�

	float diffuse = dot(N, texSpaceLight);
	diffuse = (diffuse + 1.0f) * 0.5f;
	color *= diffuse;

	return color;
}