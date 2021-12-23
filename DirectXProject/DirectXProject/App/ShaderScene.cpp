#include <App/ShaderScene.h>
#include <System/Graphics/Geometory.h>
#include <system/Camera/CameraManager.hpp>
#include <System/Input.h>

ShaderScene::ShaderScene()
	: Scene("Shader")
	, m_pPrimitive(nullptr)
{
}
ShaderScene::~ShaderScene()
{
	Release();
}
HRESULT ShaderScene::Load()
{
	// 円柱オブジェクト作成
	struct CylinderVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;
	};
	const static int CylinderVtxCount = 16;
	CylinderVertex cylinderVtx[(CylinderVtxCount + 1) * 2];

	for (int i = 0; i <= CylinderVtxCount; i++)
	{
		float rad = -i * DirectX::XM_PI * 2.f / CylinderVtxCount;
		float u = static_cast<float>(i) / CylinderVtxCount;
		int index = i * 2;
		DirectX::XMFLOAT3 normal(sinf(rad), 0.f, -cosf(rad));

		// 上段
		cylinderVtx[index + 0].normal = normal;
		cylinderVtx[index + 0].pos = normal;
		cylinderVtx[index + 0].pos.y = 2.f;
		cylinderVtx[index + 0].uv = DirectX::XMFLOAT2(u, 0.f);

		// 下段
		cylinderVtx[index + 1].normal = normal;
		cylinderVtx[index + 1].pos = normal;
		cylinderVtx[index + 1].pos.y = -2.f;
		cylinderVtx[index + 1].uv = DirectX::XMFLOAT2(u, 1.f);
	}

	DXBuffer::Desc desc = {};
	desc.pVtx = cylinderVtx;
	desc.vtxSize = sizeof(CylinderVertex);
	desc.vtxCount = _countof(cylinderVtx);
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;


	HRESULT hr;
	m_pPrimitive = new DXBuffer;
	hr = m_pPrimitive->Create(desc);

	// 定数バッファ作成
	Camera* pCamera = CAMERA->Get();
	DirectX::XMStoreFloat4x4(&m_vsMatrix.world, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_vsMatrix.view, pCamera->GetLookAtMatrix());
	DirectX::XMStoreFloat4x4(&m_vsMatrix.proj, pCamera->GetProjectionMatrix());
	
	m_pMatrixCB = new ConstantBuffer;
	hr = m_pMatrixCB->Create(sizeof(Matrix));
	if (FAILED(hr)) { return hr; }

	DirectX::XMStoreFloat4
	(
		&m_lightDirection,
		DirectX::XMVector3Normalize
		(
			DirectX::XMVectorSet(1.f, -1.f, 1.f, 0)
		)
	);
	// ライトデータ
	m_pLightCB = new ConstantBuffer;
	hr = m_pLightCB->Create(sizeof(m_lightDirection));
	if (FAILED(hr)) { return hr; }

	// カメラデータ
	DirectX::XMFLOAT3 cPos = pCamera->GetPos();
	m_cameraPos =
		DirectX::XMFLOAT4
		(
			cPos.x,
			cPos.y,
			cPos.z,
			1.f
		);
	m_pCameraCB = new ConstantBuffer;
	hr = m_pCameraCB->Create(sizeof(m_cameraPos));
	if (FAILED(hr)) { return hr; }

	// 各種パラメータ
	m_parameter = DirectX::XMFLOAT4(0, 0, 0, 0);
	m_pParameterCB = new ConstantBuffer;
	hr = m_pParameterCB->Create(sizeof(m_parameter));
	if (FAILED(hr)) { return hr; }

	// シェーダデータ
	m_pSampleVS = new VertexShader;
	m_pSampleVS->Load("Assets/Shader/SampleVS.cso");
	if (FAILED(hr)) { return hr; }
	m_pBumpVS = new VertexShader;
	hr = m_pBumpVS->Load("Assets/Shader/BumpVS.cso");
	if (FAILED(hr)) { return hr; }
	m_pToonOutlineVS = new VertexShader;
	hr = m_pToonOutlineVS->Load("Assets/Shader/ToonOutlineVS.cso");
	if (FAILED(hr)) { return hr; }

	m_pSamplePS = new PixelShader();
	hr = m_pSamplePS->Load("Assets/Shader/SamplePS.cso");
	if (FAILED(hr)) { return hr; }
	m_pSepiaPS = new PixelShader();
	hr = m_pSepiaPS->Load("Assets/Shader/Sepia.cso");
	if (FAILED(hr)) { return hr; }
	m_pLambertPS = new PixelShader();
	hr = m_pLambertPS->Load("Assets/Shader/Lambert.cso");
	if (FAILED(hr)) { return hr; }
	m_pPhongPS = new PixelShader();
	hr = m_pPhongPS->Load("Assets/Shader/Phong.cso");
	if (FAILED(hr)) { return hr; }
	m_pLimlightPS = new PixelShader;
	hr = m_pLimlightPS->Load("Assets/Shader/Limlight.cso");
	if (FAILED(hr)) { return hr; }
	m_pFogPS = new PixelShader;
	hr = m_pFogPS->Load("Assets/Shader/Fog.cso");
	if (FAILED(hr)) { return hr; }
	m_pMultiTexturePS = new PixelShader;
	hr = m_pMultiTexturePS->Load("Assets/Shader/MultiTexture.cso");
	if (FAILED(hr)) { return hr; }
	m_pDissolvePS = new PixelShader;
	hr = m_pDissolvePS->Load("Assets/Shader/Dissolve.cso");
	if (FAILED(hr)) { return hr; }
	m_pBumpPS = new PixelShader;
	hr = m_pBumpPS->Load("Assets/Shader/BumpPS.cso");
	if (FAILED(hr)) { return hr; }
	m_pToonPS = new PixelShader;
	hr = m_pToonPS->Load("Assets/Shader/ToonPS.cso");
	if (FAILED(hr)) { return hr; }
	m_pToonOutlinesPS = new PixelShader;
	hr = m_pToonOutlinesPS->Load("Assets/Shader/ToonOutlinePS.cso");
	if (FAILED(hr)) { return hr; }




	// テクスチャ読み込み
	m_pTexture = new TextureFromFile();
	hr = m_pTexture->Load("Assets/Mili.png");
	if (FAILED(hr)) { return hr; }
	m_pScarTexture = new TextureFromFile();
	hr = m_pScarTexture->Load("Assets/scar.png");
	if (FAILED(hr)) { return hr; }
	m_pDissolveTexture = new TextureFromFile();
	hr = m_pDissolveTexture->Load("Assets/dissolve1.bmp");
	if (FAILED(hr)) { return hr; }
	m_pNormalMapTexture = new TextureFromFile();
	hr = m_pNormalMapTexture->Load("Assets/stoneRoadN1.png");
	if (FAILED(hr)) { return hr; }
	m_pRampTexture = new TextureFromFile;
	hr = m_pRampTexture->Load("Assets/RampTex.bmp");
	if (FAILED(hr)) { return hr; }


	// モデル読み込み
	m_pModel = new FBXModel;
	hr = m_pModel->Load("Assets/normalSample.fbx");
	if (FAILED(hr)) { return hr; }
	m_pModelTown = new FBXModel;
	hr = m_pModelTown->Load("Assets/nagoya_city.fbx");
	if (FAILED(hr)) { return hr; }

	// 接ベクトルを含むモデル
	TangentModel::MakeDesc tangentDesc;
	tangentDesc.bufDesc = desc;
	tangentDesc.posOffset = offsetof(CylinderVertex, pos);
	tangentDesc.uvOffset = offsetof(CylinderVertex, uv);
	m_pTangentModel = new TangentModel;
	hr = m_pTangentModel->Make(tangentDesc);
	if (FAILED(hr)) { return hr; }
	
	// カリングしない
	SetCulling(CullingMode::CULL_NONE);

	return S_OK;
}
void ShaderScene::Release()
{
	safe_delete(m_pTangentModel);
	safe_delete(m_pModelTown);
	safe_delete(m_pModel);

	safe_delete(m_pRampTexture);
	safe_delete(m_pNormalMapTexture);
	safe_delete(m_pDissolveTexture);
	safe_delete(m_pScarTexture);
	safe_delete(m_pTexture);

	safe_delete(m_pToonOutlinesPS);
	safe_delete(m_pToonPS);
	safe_delete(m_pBumpPS);
	safe_delete(m_pDissolvePS);
	safe_delete(m_pMultiTexturePS);
	safe_delete(m_pFogPS);
	safe_delete(m_pLimlightPS);
	safe_delete(m_pPhongPS);
	safe_delete(m_pLambertPS);
	safe_delete(m_pSepiaPS);
	safe_delete(m_pSamplePS);

	safe_delete(m_pToonOutlineVS);
	safe_delete(m_pBumpVS);
	safe_delete(m_pSampleVS);
	safe_delete(m_pMatrixCB);
	safe_delete(m_pPrimitive);
}
void ShaderScene::Update(float tick)
{
	// 定数バッファ更新
	Camera* pCamera = CAMERA->Get();
	
	DirectX::XMStoreFloat4x4(&m_vsMatrix.world, DirectX::XMMatrixTranspose(DirectX::XMMatrixIdentity()));
	DirectX::XMStoreFloat4x4(&m_vsMatrix.view, DirectX::XMMatrixTranspose(pCamera->GetLookAtMatrix()));
	DirectX::XMStoreFloat4x4(&m_vsMatrix.proj, DirectX::XMMatrixTranspose(pCamera->GetProjectionMatrix()));
	m_pMatrixCB->Write(&m_vsMatrix);

	// ライト回転
	static float theta = 0.f;
	float rad = DirectX::XMConvertToRadians(theta);
	m_lightDirection.x = 1.0f * sinf(rad);
	m_lightDirection.z = 1.0f * cosf(rad);
	theta += 1.0f;
	(theta < 360) ?  true: theta -= 360.f;
	m_pLightCB->Write(&m_lightDirection);

	DirectX::XMFLOAT3 cPos = pCamera->GetPos();
	m_cameraPos = DirectX::XMFLOAT4(cPos.x, cPos.y, cPos.z, 1.0f);
	m_pCameraCB->Write(&m_cameraPos);

	// フレーム計算
	const float fadeTime = 2.f;
	m_parameter.x += 1.f / (60 * fadeTime);
	if (m_parameter.x > 1.2f)
	{
		m_parameter.x = 0.f;
	}
	m_pParameterCB->Write(&m_parameter);


	// デバッグ表示用設定
	SetGeometoryVP(m_vsMatrix.view, m_vsMatrix.proj);
}
void ShaderScene::Draw()
{
	static int select = 0;
	const int max = 9;
	// TODO:動的にシェーダ読み込み 
	// 同時にmapを作成して特定のシェーダであるときにテクスチャを設定したりする
	// constancebufferとShaderをラップしないと実装できない。工数が多いので今回は割愛
	if (IsKeyTrigger(VK_LEFT))
		select = (select -1 + max) % max;
	else if(IsKeyTrigger(VK_RIGHT))
		select = (select + 1) % max;
	
	if (IsKeyPress('1'))
		select = 1;
	else if (IsKeyPress('2'))
		select = 2;
	else if (IsKeyPress('3'))
		select = 3;
	else if (IsKeyPress('4'))
		select = 4;
	else if (IsKeyPress('5'))
		select = 5;
	else if (IsKeyPress('6'))
		select = 6;
	else if (IsKeyPress('7'))
		select = 7;
	else if (IsKeyPress('8'))
		select = 8;
	else if(0)
		select = 0;
	

	switch (select)
	{
	case 0:
		m_pSamplePS->Bind();
		break;
	case 1:
		m_pSepiaPS->Bind();
		break;
	case 2:
		m_pLambertPS->Bind();
		break;
	case 3:
		m_pPhongPS->Bind();
		break;
	case 4:
		m_pLimlightPS->Bind();
		break;
	case 5:
		m_pFogPS->Bind();
		break;
	case 6:
		m_pMultiTexturePS->Bind();
		SetTexture(m_pScarTexture->GetResource(), 1);
		break;
	case 7:
		m_pDissolvePS->Bind();
		SetTexture(m_pDissolveTexture->GetResource(), 1);
		break;
	case 8:
		m_pToonPS->Bind();
		SetTexture(m_pRampTexture->GetResource(), 1);
		break;
	default:
		break;
	}

	m_pSampleVS->Bind();
	m_pMatrixCB->BindVS(0);
	m_pLightCB->BindPS(0);
	m_pCameraCB->BindPS(1);
	m_pParameterCB->BindPS(2);
	SetTexture(m_pTexture->GetResource());

	//m_pPrimitive->Draw();
	m_pModel->Draw();
	m_pModelTown->Draw();

	m_pBumpVS->Bind();
	m_pBumpPS->Bind();
	m_pLightCB->BindVS(1);
	SetTexture(m_pNormalMapTexture->GetResource(),1);
	m_pTangentModel->Draw();
	
	/* 輪郭線の表示
	* 元のモデルから少し大きくしたモデルをかぶせることで輪郭線として表現する
	* 裏面を表示する設定に変える。(大きくしただけだと元のモデルが隠れてしまう)
	*/
	if (IsKeyPress('8'))
	{
		SetCulling(CULL_FRONT);
		m_pCameraCB->BindVS(1);
		m_pToonOutlineVS->Bind();
		m_pToonOutlinesPS->Bind();

#if 0 // モデルの中心から離れた頂点ほど大きくずれた位置に表示されてしまう
		const float scale = 1.05f;
		DirectX::XMStoreFloat4x4
		(
			&m_vsMatrix.world,
			DirectX::XMMatrixTranspose
			(
				DirectX::XMMatrixScaling(scale, scale, scale)
			)
		);

		m_pMatrixCB->Write(&m_vsMatrix);
		m_pMatrixCB->BindVS(0);
#endif
		m_pModel->Draw();
		SetCulling(CULL_BACK);
	}

	AddLine(DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT3(100.f, 0.f, 0.f), DirectX::XMFLOAT4(1.f, 0.f, 0.f, 1.f));
	AddLine(DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT3(0.f, 100.f, 0.f), DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f));
	AddLine(DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT3(0.f, 0.f, 100.f), DirectX::XMFLOAT4(0.f, 0.f, 1.f, 1.f));
	DirectX::XMFLOAT3 pseudoLightPos
	(
		-m_lightDirection.x * 100.f,
		-m_lightDirection.y * 100.f,
		-m_lightDirection.z * 100.f
	);
	
	AddLine(pseudoLightPos, DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT4(0.5f, 0.5f, 0.f, 1.f));

	DrawLines();
}
