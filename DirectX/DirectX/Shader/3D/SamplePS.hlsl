 
// input
struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

// �e�N�X�`���󂯎��
Texture2D tex : register(t0);
// �e�N�X�`���J��Ԃ�
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.f, 1.f, 1.f, 1.f);
	
	color = tex.Sample(samp, pin.uv);

// Monoclo
	// 1.�P���ȕ���
	float mono = (color.r + color.g + color.b) / 3.f;
	// 2.�F�����l��
	mono = color.r * 0.3f + color.g * 0.6f + color.b * 0.1f;

	color.rgb = mono;
	
	return color;
}