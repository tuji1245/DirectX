#include "Texture.h"
#include "DirectXTex/DirectXTex.h"

#ifdef _X86_
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/Win32/Debug/DirectXTex.lib")
#else
#pragma comment(lib, "DirectXTex/Win32/Release/DirectXTex.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/x64/Debug/DirectXTex.lib")
#else
#pragma comment(lib, "DirectXTex/x64/Release/DirectXTex.lib")
#endif
#endif


Texture::Texture()
	: m_pSRV(nullptr)
{
}
Texture::~Texture()
{
}
ID3D11ShaderResourceView* Texture::GetResource()
{
	return m_pSRV;
}

TextureFromFile::TextureFromFile()
{
}
TextureFromFile::~TextureFromFile()
{
}
HRESULT TextureFromFile::Load(const char* fileName)
{
	HRESULT hr;

	// �����ϊ�
	wchar_t wPath[MAX_PATH];
	size_t wLen = 0;
	MultiByteToWideChar(0, 0, fileName, -1, wPath, MAX_PATH);

	// �t�@�C���ʓǂݍ���
	DirectX::TexMetadata mdata;
	DirectX::ScratchImage image;
	if (strstr(fileName, ".tga"))
	{
		hr = DirectX::LoadFromTGAFile(wPath, &mdata, image);
	}
	else
	{
		hr = DirectX::LoadFromWICFile(wPath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &mdata, image);
	}
	if (FAILED(hr))
	{
		MessageBox(NULL, fileName, "Failed to load texture.", MB_OK);
		return hr;
	}

	// �V�F�[�_���\�[�X�r���[�쐬
	hr = CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), mdata, &m_pSRV);
	return hr;
}



TextureFromData::TextureFromData()
{
}
TextureFromData::~TextureFromData()
{
}

HRESULT TextureFromData::Create(DXGI_FORMAT format, void* pData)
{
	return S_OK;
}

TextureRenderTarget::TextureRenderTarget()
	:m_pRTV(nullptr)
{
}

TextureRenderTarget::~TextureRenderTarget()
{
	SAFE_DELETE(m_pRTV);
}

HRESULT TextureRenderTarget::Create(DXGI_FORMAT format, UINT width, UINT height)
{
	HRESULT hr;
	ID3D11Device* pDevice = GetDevice();

	// �e�N�X�`���̏��
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.Format = format;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	// �e�N�X�`���쐬
	ID3D11Texture2D* pTex2D;
	hr = pDevice->CreateTexture2D(&desc, nullptr, &pTex2D);
	if (FAILED(hr)) { return hr; }

	// �V�F�[�_���\�[�X�̏��
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// �V�F�[�_���\�[�X�쐬
	hr = pDevice->CreateShaderResourceView(pTex2D, &srvDesc, &m_pSRV);
	if (FAILED(hr)) { return hr; }

	// �����_�[�^�[�Q�b�g�̏��
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	// �����_�[�^�[�Q�b�g�쐬
	hr = pDevice->CreateRenderTargetView(pTex2D, &rtvDesc, &m_pRTV);
	if (FAILED(hr)) { return hr; }

	return hr;
}
