/*�f�B�]���u
* A�̉摜��B�֕ω�������\��
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

Texture2D baseTex : register(t0); // �ʏ�e�N�X�`�� ,Viewport�̃e�N�X�`���̂ق�����������
Texture2D dissolveTex : register(t1);

SamplerState samp: register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.f, 1.f, 1.f, 1.f);

	color = baseTex.Sample(samp, pin.uv);

	// �t�F�[�h�̂� = ���ߐF���ݕω�������
	//color.a = time;

	// �e�N�X�`���̐F��(0->1,��->��)�Ƃ����Ƃ��A�o�ߎ��ԂƔ�r���ď������l�͕\������đ傫���l��
	// �\������Ȃ��Ɣ���o����΁A�摜��p�����t�F�[�h���ł���
	float dissolve = dissolveTex.Sample(samp, pin.uv).r;
	// step(a,b) = (a <= b ? 1 : 0);
	color.a = step(dissolve, time);//value.x);	

	return color;
}