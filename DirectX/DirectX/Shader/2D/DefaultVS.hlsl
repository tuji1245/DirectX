struct VS_IN
{
	float3 Pos : POSITION;
	float4 Col : TEXCOORD;
};
struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float4 Col : TEXCOORD;
};
VS_OUT main( VS_IN vin)
{
	VS_OUT vout = (VS_OUT)0;
	vout.Pos = float4(vin.Pos, 1);
	vout.Col = vin.Col;
	return vout;
}