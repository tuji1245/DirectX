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
    // Shadeer���쐬
    m_pDefaultVS = new VertexShader;
    hr = m_pDefaultVS->Load("Assets/shader/DefaultVS.cso");
    if (FAILED(hr)) { assert(0); }
    m_pDefaultPS = new PixelShader;
    hr = m_pDefaultPS->Load("Assets/shader/DefaultPS.cso");
    if (FAILED(hr)) { assert(0); }
    
    struct Vertex
    {
        XMFLOAT3 pos;
        XMFLOAT4 col;
    };
    Vertex vertexs[] = {
        { XMFLOAT3(-0.5f,-0.5f,0.0f),XMFLOAT4(1,0,0,1) },
        { XMFLOAT3( 0.5f,-0.5f,0.0f),XMFLOAT4(0,1,0,1) },
        { XMFLOAT3(-0.5f, 0.5f,0.0f),XMFLOAT4(0,0,0,1) },
        { XMFLOAT3( 0.5f, 0.5f,0.0f),XMFLOAT4(0,0,1,1) },
    };

    // �C���f�b�N�X���̐ݒ�
    DXBuffer::Desc desc = {};
    desc.pVtx = vertexs;
    desc.vtxSize = sizeof(vertexs);
    desc.vtxCount = _countof(vertexs);
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
    // �|���S���`��p�����[�^�Z�b�g
    m_pDefaultVS->Bind();
    m_pDefaultPS->Bind();

    m_pPrimitive->Draw();

    //DrawCall
	Scene::Draw();
}