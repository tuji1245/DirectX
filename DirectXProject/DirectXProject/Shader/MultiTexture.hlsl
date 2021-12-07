/*1�̃��f���ɕ����̃e�N�X�`��
���≘���\������ہA�V���������≘��̃e�N�X�`����\��t����
�V�F�[�_���ŕ����̃e�N�X�`������F��ǂݎ��A���ꂼ�ꍇ���������ʂ��틭������B

*/




struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;	
};

Texture2D baseTex : register(t0); // �ʏ�e�N�X�`��
Texture2D scarTex : register(t1); // �ォ��\��e�N�X�`��

SamplerState samp: register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.f, 1.f, 1.f, 1.f);

	color = baseTex.Sample(samp, pin.uv);

	// �������d�ˍ��킹���s�����߂ɁA���O�ŃA���t�@�u�����h���s��
	// �A���t�@�u�����h dst.rgb * (1.f - src.a) + src.rgb * src.a;
	float4 blendColor = scarTex.Sample(samp, pin.uv);
	color.rgb = color.rgb * (1.f - blendColor.a) + blendColor.rgb * blendColor.a;

	color.a = 1.f;
	return color;
}