/* PhongShadeing
* Lambertに鏡面反射を加えたもの
* 光が反射してまぶしく見えるためには観測者が必要
* 3Dゲームではカメラが観測者となる。
* 光の反射なので光は一度オブジェクトに当たって反射しなければならない
* 跳ね返り方はオブジェクトに光が当たる角度によって変わる
* ピクセルごとに反射した光を求める
* 
* この時反射した光とカメラの視線のベクトルの角度が浅いときまぶしく見える 
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
	// 注目しているベクトルへのベクトル
	float3 V = normalize(eyePos - pin.worldPos);
	// 反射ベクトル(組み込み関数で計算
	// 反射地点の法線ベクトルで計算する
	float3 R = reflect(L, N);
	R = normalize(R);

	// ---鏡面反射の計算
	// 光が当たってまぶしく見える部分 - スぺキュラ
	float s = dot(V, R) * 0.5f + 0.5f;
	// そのままだと広範囲が明るく見えてしまうので、抑える必要がある
	// 線形で明るさが変化するのではなく、カーブを描くような変化が望ましい
	// 累乗のグラフでこれを解決する
	// 20乗位がちょうどいい？
	s = pow(s, 20);

	// ランバート(影)の計算
	float d = dot(N, -L) + 1.0f * 0.5f;
	
	//ランバートと鏡面反射を合成
	color.rgb = d + s;
	return color;
}