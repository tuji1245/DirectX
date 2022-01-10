#include <App/Main.h>
#include <Defines.h>
#include <System/Graphics/DirectX.h>
#include <System/Input.h>
#include <System/Graphics/Geometory.h>
#include <System/FBX/FBXLoader.h>
#include <System/Camera/CameraManager.hpp>
#include <System/Scene/SceneManager.hpp>
#include <System\Scene\SampleScene.h>

#define ASSERT_FAILED(fn) do {\
hr = fn; \
if(FAILED(hr)) { \
	MessageBox(nullptr, "Init failed : "#fn, "Error", MB_OK); \
	return hr; \
}} while(0)

HRESULT Init(HWND hWnd, UINT width, UINT height)
{
	HRESULT hr;
	ASSERT_FAILED(InitDX(hWnd, width, height, false));
	ASSERT_FAILED(InitGeometory());
	ASSERT_FAILED(InitInput());
	ggfbx::Initialize();
	CameraManager::Instantiate();
	SceneManager::CreateInstance();
	SCENE->Load<SampleScene>();

	return hr;
}

void Uninit()
{
	SceneManager::DestroyInstance();
	CameraManager::Destroy();
	ggfbx::Terminate();
	UninitInput();
	UninitDX();
}

void Update()
{
	UpdateInput();
	CAMERA->Update();
	SCENE->Update(1.0f);
}

void Draw()
{
	BeginDrawDX();
	SCENE->Draw();
	EndDrawDX();
}

// EOF