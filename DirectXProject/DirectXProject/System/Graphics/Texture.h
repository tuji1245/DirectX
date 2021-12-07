#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <System/Graphics/DirectX.h>

class Texture
{
public:
	Texture();
	virtual ~Texture();

	ID3D11ShaderResourceView *GetResource();
protected:
	ID3D11ShaderResourceView *m_pSRV;
};


class TextureFromFile : public Texture
{
public:
	TextureFromFile();
	~TextureFromFile();

	HRESULT Load(const char *fileName);
private:
};


class TextureFromData : public Texture
{
public:
	TextureFromData();
	~TextureFromData();

	HRESULT Create(DXGI_FORMAT format, void *pData);

};

// 書込み用テクスチャ
class TextureRenderTarget : public Texture
{
public:
	TextureRenderTarget();
	~TextureRenderTarget();

	HRESULT Create(DXGI_FORMAT format, UINT width, UINT height);
	
private:
	ID3D11RenderTargetView* m_pRTV;	// 書込み用画面
};

#endif // __TEXTURE_H__