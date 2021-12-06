#include "Scripts/Sample/SampleScene.h"

SampleScene::SampleScene()
{
	
}
SampleScene::~SampleScene()
{

}
void SampleScene::Init()
{
	Scene::Init();

    HRESULT hr = S_OK;
    // Shadeerを作成
    m_pDefaultVS = new VertexShader;
    hr = m_pDefaultVS->Load("Assets/shader/DefaultVS.cso");
    if (FAILED(hr)) { assert(0); }
    m_pDefaultPS = new PixelShader;
    hr = m_pDefaultPS->Load("Assets/shader/DefaultPS.cso");
    if (FAILED(hr)) { assert(0); }

    m_pSampleVS = new VertexShader;
    hr = m_pSampleVS->Load("Assets/shader/SampleVS.cso");
    if (FAILED(hr)) { assert(0); }
    m_pSamplePS = new PixelShader;
    hr = m_pSamplePS->Load("Assets/shader/SamplePS.cso");
    if (FAILED(hr)) { assert(0); }
    
    
    //struct Vertex
    //{
    //    XMFLOAT3 pos;
    //    XMFLOAT4 col;
    //};
    //Vertex vertexs[] = {
    //    { XMFLOAT3(-0.5f,-0.5f,0.0f),XMFLOAT4(1,0,0,1) },
    //    { XMFLOAT3( 0.5f,-0.5f,0.0f),XMFLOAT4(0,1,0,1) },
    //    { XMFLOAT3(-0.5f, 0.5f,0.0f),XMFLOAT4(0,0,0,1) },
    //    { XMFLOAT3( 0.5f, 0.5f,0.0f),XMFLOAT4(0,0,1,1) },
    //};

    //// インデックス情報の設定
    //DXBuffer::Desc desc = {};
    //desc.pVtx = vertexs;
    //desc.vtxSize = sizeof(Vertex);
    //desc.vtxCount = _countof(vertexs);
    //desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
    //
    //m_pPrimitive = new DXBuffer;
    //hr = m_pPrimitive->Create(desc);

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

    m_pPrimitive = new DXBuffer;
    hr = m_pPrimitive->Create(desc);

}
void SampleScene::Uninit()
{
	Scene::Uninit();
}
void SampleScene::Update()
{
	Scene::Update();
}
void SampleScene::Draw()
{
    // ポリゴン描画パラメータセット
    //m_pDefaultVS->Bind();
    //m_pDefaultPS->Bind();
    m_pSampleVS->Bind();
    m_pSamplePS->Bind();
    m_pPrimitive->Draw();

    //DrawCall
	Scene::Draw();
}