#ifndef __DIRECT_X_H__
#define __DIRECT_X_H__

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

template <class T>
inline void safe_release(T*& p)
{
	if (p) {
		p->Release();
		p = nullptr;
	}
}
template <class T>
inline void safe_delete(T*& p) 
{
	if (p) {
		delete p;
		p = nullptr;
	}
}

template <class T>
inline void safe_delete_array(T*& p) 
{
	if (p) {
		delete[] p;
		p = nullptr;
	}
}

enum BlendMode
{
	BLEND_ALPHA,
	BLEND_ADD,
	BLEND_MAX
};
enum CullingMode
{
	CULL_NONE,
	CULL_FRONT,
	CULL_BACK,
	CULL_MAX
};

HRESULT InitDX(HWND hWnd, UINT width, UINT height, bool fullscreen);
void UninitDX();
void BeginDrawDX();
void EndDrawDX();

ID3D11Device *GetDevice();
ID3D11DeviceContext *GetContext();

void SetBlendMode(BlendMode blend);
void SetCulling(CullingMode cull);
void SetRenderTarget(UINT width, UINT height, ID3D11RenderTargetView** ppView, UINT numView, ID3D11DepthStencilView* pDepth, float* pColor = NULL);

#endif // __DIRECT_X_H__