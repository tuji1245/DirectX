struct PS_IN
{
	float4 Pos : SV_POSITION;
	float4 Col : TEXCOORD;
};

float4 main(PS_IN pin) : SV_TARGET
{
	return pin.Col;
}