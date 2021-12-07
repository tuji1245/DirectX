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
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	float cameraDistance = length(eyePos.xyz - pin.worldPos);

	float fogStartDistance = 100.f;
	float fogRange = 100.f;

	cameraDistance -= fogStartDistance;
	cameraDistance /= fogRange;

	float fog = 1.0 - cameraDistance;
	float4 backColor = float4(0.8, 0.8, 0.9, 1.0);
	color = lerp(backColor, color, saturate(fog));

	
	return color;
}