/* リムライト
*  物体の後ろから光が当たって輪郭が光って見えるように強調される表現
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
	// 計算に必要なパラメータ
	float3 L = -normalize(direction.xyz);		 // 光源ベクトル
	float3 V = normalize(pin.worldPos - eyePos); // 視線ベクトル
	float3 N = normalize(pin.normal);

	// リムライト
	float lim = 1 - dot(L, N);
	lim *= 1 -dot(V, L);
	lim *= 1 -dot(V, N);

	// lim = dot L, V
	// edge =  dot N, V

    // lim = pow(lim,7)
	// lim * edge
	color.rgb = lim;


	// 考察:
	//   法線ベクトルとカメラベクトルが直交に近いとき明るくする？
	// 結論
	// 　法線と光源の内積が直交に近い　明るい
	// 　視線と光源の内積が直交に近い　明るい
	// 　視線と法線の内積が直交に近い　明るい　new!!
	// 　この二つを使って明るさを導き出す


	return color;
}