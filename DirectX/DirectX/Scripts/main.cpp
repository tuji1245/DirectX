/**
* @file main.h
* @breif 
*
* @author 辻悠希
* @date 2021/06/28
* @details
*/
#include "main.h"
#include "DX11Manager.h"

#include "Scripts/System/input.h"
#include "Scripts/MyImgui/DebugWindowManager.h"
#include "FrameManager.h"
#include "DemoImgui.h"

#include "Sample/SampleScene.h"

// macro
#ifdef _DEBUG
#define AllowImGui  // リリースビルドでImGuiを表示しない
#endif // _DEBUG

//VertexShader* g_VS;
//PixelShader* g_PS;
//InputLayout* g_InputLayout;
//
//VertexBuffer* g_VB;
////vector<UINT> idxs = { 0,1,2,0,2,3 };
//vector<UINT> idxs = { 0, 3, 2, 0, 2, 1 };
//IndexBuffer* g_IndexBuffer;

// Setup Dear ImGui context
DebugWindowManager* debugWindowManager;
FrameManagerWindow frameManager("frameManager");
DemoImgui demo("demo", false);

// 頂点情報を作成
//struct Vertex
//{
//    XMFLOAT3 pos;
//    XMFLOAT4 col;
//};
//vector<Vertex> vertexs = {
//    { XMFLOAT3(-0.5f,-0.5f,0.0f),XMFLOAT4(1,0,0,1) },
//    { XMFLOAT3(0.5f,-0.5f,0.0f),XMFLOAT4(0,1,0,1) },
//    { XMFLOAT3(0.5f, 0.5f,0.0f),XMFLOAT4(0,0,1,1) },
//    { XMFLOAT3(-0.5f, 0.5f,0.0f),XMFLOAT4(0,0,0,1) },
//};

SampleScene* g_sampleScene;

HRESULT Initialize(HWND hwnd)
{
    // directxの初期化
    if (FAILED(DXInit(hwnd)))
    {
        DXUninit();
        return E_FAIL;
    }
    debugWindowManager = new DebugWindowManager(hwnd, GetDevice(), GetContext());
    debugWindowManager->AddDebugWindow(&frameManager);
    debugWindowManager->AddDebugWindow(&demo);

    //// Shadeerを作成
    //string ShaderFilePath = "Shader/2DPipeLine.hlsl";
    //g_VS = CreateVertexShader(ShaderFilePath, "vsMain");
    //g_PS = CreatePixelShader(ShaderFilePath, "psMain");

    //// InputLayoutの作成
    //D3D11_INPUT_ELEMENT_DESC elem[] = {
    //    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0,  0,                           D3D11_INPUT_PER_VERTEX_DATA, 0},
    //    {"TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    //};

    //g_InputLayout = CreateInputLayout(elem, 2, ShaderFilePath, "vsMain");


    //g_VB = CreateVertexBuffer(vertexs.data(), static_cast<UINT>(vertexs.size()));

    //// インデックス情報の設定
    //g_IndexBuffer = CreateIndexBuffer(idxs.data(), static_cast<UINT>(idxs.size()));

    g_sampleScene = new SampleScene;
    g_sampleScene->Init();

}
void Terminate()
{
    g_sampleScene->Uninit();
    delete g_sampleScene;
    delete debugWindowManager;
    DXUninit();
}
void Update()
{
    frameManager.Update();
    KeyUpdate();    // Updateの最初に呼ぶ
    // 以下にオブジェクト処理

}
void FixedUpdate()
{
    if (!frameManager.GetFlag()) return;
    // 以下に処理
}
void Draw()
{
    DrawBegin();

    //// ポリゴン描画パラメータセット
    //SetVertexShader(g_VS);
    //SetPixelShader(g_PS);
    //SetInputLayout(g_InputLayout);

    //SetVertexBuffer(g_VB, sizeof(Vertex));
    //SetIndexBuffer(g_IndexBuffer);

    ////DrawCall
    //DrawIndexed(static_cast<UINT>(idxs.size()));
    g_sampleScene->Draw();

#ifdef AllowImGui
    debugWindowManager->Draw();
#endif // AllowImGui
    DrawEnd();

}