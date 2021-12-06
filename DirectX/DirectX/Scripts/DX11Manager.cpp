/*
* -- Task --
* #1
* void DirectX11Manager::ReSize(WPARAM wParam, LPARAM lParam)
* WindowsAPIのHookとか使ってこの.cpp上で呼び出せるとベスト(外部で呼び忘れを防ぐため)
*/

#pragma warning(disable:4996)

#include "DX11Manager.h"
#include <iostream>

/** Windowsのハンドル */
HWND  hWnd = NULL;

RECT windowRECT = defWindowRECT;

ID3D11Device*			g_pDevice = nullptr;
ID3D11DeviceContext*	g_pImContext = nullptr;
IDXGISwapChain*			g_pSwapChain = nullptr;
ID3D11Texture2D*		g_pRTTex = nullptr;
ID3D11RenderTargetView* g_pRTView = nullptr;
D3D11_VIEWPORT			g_Viewport = { 0,0,0,0,0,0 };


HRESULT DXInit(HWND hWnd)
{
#pragma region HardWareCheck
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes = 0;
	size_t stringLength;
	DXGI_ADAPTER_DESC adapterDesc;
	
	// グラフィック インターフェース ファクトリ 作成
	auto hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(hr)) {
		return hr;
	}

	int GPUNumber = 0;
	int GPUMaxMem = 0;
	// 一番強いGPUアダプタを検索
	for (int i = 0;i < 100;i++) 
	{
		IDXGIAdapter* add;
		hr = factory->EnumAdapters(i, &add);
		if (FAILED(hr)) {
			break;
		}
		hr = add->GetDesc(&adapterDesc);

		char VideoCardDescriptopn[128];
		// ビデオカード名 取得
		int error = wcstombs_s(&stringLength, VideoCardDescriptopn, 128, adapterDesc.Description, 128);
		if (error != 0) {
			break;
		}
		cout << "ビデオカード名 : " << VideoCardDescriptopn << endl;

		// ビデオカードメモリ 取得(MB単位)
		int VideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
		cout << "ビデオメモリ : " << VideoCardMemory << endl;

		// アウトプット(モニター)に番号IDをつける
		hr = add->EnumOutputs(0, &adapterOutput);
		if (FAILED(hr)) {
			continue;
		}
	
		//DXGI_FORMAT_R8G8B8A8_UNORM の表示形式数を取得する
		hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if (FAILED(hr)) {
			continue;
		}
		cout << "RBGA8_UNORM Count : " << numModes << endl;

		if (VideoCardMemory > GPUMaxMem) {
			GPUMaxMem = VideoCardMemory;
			GPUNumber = i;
		}
		add->Release();
		// アダプタアウトプット 解放
		adapterOutput->Release();
		adapterOutput = 0;
	}

	// グラフィック インターフェース アダプタ 作成
	hr = factory->EnumAdapters(GPUNumber, &adapter);
	if (FAILED(hr)) {
		return hr;
	}
#pragma endregion

#pragma region DirectX11Init
	UINT cdev_flags = 0;
#ifdef _DEBUG
	cdev_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// スワップチェイン設定
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = windowRECT.right;
	sd.BufferDesc.Height = windowRECT.bottom;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
	};

	// DirectX11デバイスとスワップチェイン 作成
	hr = D3D11CreateDeviceAndSwapChain(
		adapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		cdev_flags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		&sd,
		&g_pSwapChain,
		&g_pDevice,
		NULL,
		&g_pImContext);
	if (FAILED(hr)) {
		return hr;
	}

	// アダプタ 解放
	adapter->Release();
	adapter = 0;

	// ファクトリ 解放
	factory->Release();
	factory = 0;

	// レンダーターゲット作成
	CreateRenderTarget();

	// viewport
	g_Viewport.Width = static_cast<FLOAT>(windowRECT.right - windowRECT.left);
	g_Viewport.Height = static_cast<FLOAT>(windowRECT.bottom - windowRECT.top);
	g_Viewport.MinDepth = 0.0f;
	g_Viewport.MaxDepth = 1.0f;
	g_Viewport.TopLeftX = 0;
	g_Viewport.TopLeftY = 0;
#pragma endregion

	return hr;
}

ID3D11VertexShader* CreateVertexShader(const string& filename, const string& entrypath, bool error)
{
	ID3D11VertexShader* Shader;

#if defined(_DEBUG)
	// グラフィックデバッグツールによるシェーダーのデバッグを有効に
	UINT	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT	compileFlags = 0;
#endif
	ComPtr<ID3DBlob> blob;
	wchar_t ws[512];

	setlocale(LC_CTYPE, "jpn");
	mbstowcs(ws, filename.c_str(), 512);
	ComPtr<ID3DBlob> pErrorBlob = NULL;
	HRESULT hr = D3DCompileFromFile(
		ws,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entrypath.c_str(),
		"vs_5_0",
		compileFlags,
		0,
		&blob,
		&pErrorBlob);

	// エラーチェック
	if (FAILED(hr))
	{
		if (error) 
		{
			// エラーメッセージ出力
			if (pErrorBlob != NULL)
			{
				MessageBoxA(NULL, (char*)pErrorBlob->GetBufferPointer(), "", 0);
				return nullptr;
			}
		}
		else
		{
			string er = (char*)pErrorBlob->GetBufferPointer();
			if (er.find("entrypoint not found") == string::npos)
			{
				MessageBoxA(NULL, (char*)pErrorBlob->GetBufferPointer(), "", 0);
			}
			cout << filename << "(" << entrypath << ") is not found" << endl;
			return nullptr;
		}
	}
	hr = g_pDevice->CreateVertexShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		NULL,
		&Shader);
	assert(SUCCEEDED(hr));

	return Shader;
}

ID3D11PixelShader* CreatePixelShader(const string& filename, const string& entrypath, bool error)
{
	ID3D11PixelShader* Shader;

#if defined(_DEBUG)
	// グラフィックデバッグツールによるシェーダーのデバッグを有効に
	UINT	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT	compileFlags = 0;
#endif
	ComPtr<ID3DBlob> blob;
	wchar_t ws[512];

	setlocale(LC_CTYPE, "jpn");
	mbstowcs(ws, filename.c_str(), 512);
	ComPtr<ID3DBlob> pErrorBlob = NULL;
	HRESULT hr = D3DCompileFromFile(
		ws,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entrypath.c_str(),
		"ps_5_0",
		compileFlags,
		0,
		&blob,
		&pErrorBlob);

	// エラーチェック
	if (FAILED(hr))
	{
		if (error)
		{
			// エラーメッセージを出力
			if (pErrorBlob != NULL)
			{
				MessageBoxA(NULL, (char*)pErrorBlob->GetBufferPointer(), "", 0);
				return nullptr;
			}
			else
			{
				string er = (char*)pErrorBlob->GetBufferPointer();
				if (er.find("entrypoint not found") == string::npos)
				{
					MessageBoxA(NULL, (char*)pErrorBlob->GetBufferPointer(), "", 0);
				}
				cout << filename << "(" << entrypath << ") is not found" << endl;
				return nullptr;
			}
		}
	}
	hr = g_pDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &Shader);
	assert(SUCCEEDED(hr));

	return Shader;
}

ID3D11InputLayout* CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layout, UINT elem_num, const string& filename, const string& entrypath)
{
	ID3D11InputLayout* pVertexLayout;

#if defined(_DEBUG)
	// グラフィックデバッグツールによるシェーダーのデバッグを有効に
	UINT	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT	compileFlags = 0;
#endif
	ComPtr<ID3DBlob> blob;
	wchar_t ws[512];

	setlocale(LC_CTYPE, "jpn");
	mbstowcs(ws, filename.c_str(), 512);
	ComPtr<ID3DBlob> pErrorBlob = NULL;
	HRESULT hr = D3DCompileFromFile(ws, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entrypath.c_str(), "vs_5_0", compileFlags, 0, &blob, &pErrorBlob);

	// エラーチェック
	if (FAILED(hr))
	{
		// エラーメッセージを出力
		if (pErrorBlob != NULL)
		{
			MessageBoxA(NULL, (char*)pErrorBlob->GetBufferPointer(), "", 0);
		}			
	}
	hr = g_pDevice->CreateInputLayout(layout,elem_num,blob->GetBufferPointer(),blob->GetBufferSize(),&pVertexLayout);
	assert(SUCCEEDED(hr));

	return pVertexLayout;

}
void DXUninit()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pImContext) { g_pImContext->Release(); g_pImContext = NULL; }
	if (g_pDevice) { g_pDevice->Release(); g_pDevice = NULL; }
}

ID3D11Buffer* CreateIndexBuffer(UINT* Index, UINT IndexNum)
{
	// インデックスバッファ作成
	D3D11_BUFFER_DESC hBufferDesc;
	ZeroMemory(&hBufferDesc, sizeof(hBufferDesc));
	hBufferDesc.ByteWidth = sizeof(UINT) * IndexNum;
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA hSubResourceData;
	ZeroMemory(&hSubResourceData, sizeof(hSubResourceData));
	hSubResourceData.pSysMem = Index;

	ID3D11Buffer* hpBuffer;
	if (FAILED(g_pDevice->CreateBuffer(&hBufferDesc, &hSubResourceData, &hpBuffer)))
	{
		return nullptr;
	}
	return hpBuffer;
}

void SetInputLayout(ID3D11InputLayout* VertexLayout)
{
	g_pImContext->IASetInputLayout(VertexLayout);
}
void SetVertexShader(ID3D11VertexShader* vs)
{
	g_pImContext->VSSetShader(vs, nullptr, 0);
}
void SetPixelShader(ID3D11PixelShader* ps)
{
	g_pImContext->PSSetShader(ps, nullptr, 0);
}
void SetVertexBuffer(ID3D11Buffer* VertexBuffer, UINT VertexSize)
{
	UINT hOffsets = 0;
	g_pImContext->IASetVertexBuffers(0, 1, &VertexBuffer, &VertexSize, &hOffsets);
}
void SetIndexBuffer(ID3D11Buffer* IndexBuffer)
{
	g_pImContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}
void SetTexture2D(UINT RegisterNo, ID3D11ShaderResourceView* Texture)
{
	g_pImContext->PSSetShaderResources(RegisterNo, 1, &Texture);
}
void DrawBegin()
{
	// ポリゴン生成方法の指定
	g_pImContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 指定色で画面クリア
	float ClearColor[4] = { 0.0f,0.0f,0.0f,1.0f };
	g_pImContext->ClearRenderTargetView(g_pRTView, ClearColor);
	g_pImContext->RSSetViewports(1, &g_Viewport);

	// RenderTargetをバックバッファ
	ID3D11RenderTargetView* rtv[1] = { g_pRTView };
	g_pImContext->OMSetRenderTargets(1, rtv, nullptr);
}
void DrawEnd()
{
	g_pSwapChain->Present(0, 0);
}
void DrawIndexed(UINT VertexNum)
{
	g_pImContext->DrawIndexed(VertexNum, 0, 0);
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRTView);
	pBackBuffer->Release();
}
void CleanupRenderTarget()
{
	if (g_pRTView) { g_pRTView->Release(); }// DX11Manager.m_pRTView = NULL;
}

void ReSize(WPARAM wParam, LPARAM lParam)
{
	if (g_pDevice != NULL && wParam != SIZE_MINIMIZED)
	{
		CleanupRenderTarget();
		g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
		CreateRenderTarget();
	}
}

HWND GetWindow() { return hWnd; }
ID3D11Device* GetDevice() { return g_pDevice; }
ID3D11DeviceContext* GetContext() { return g_pImContext; }