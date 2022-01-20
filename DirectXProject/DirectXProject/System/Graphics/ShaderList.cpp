#include "ShaderList.h"

using namespace Graphics;
namespace 
{
	VertexShader* g_vertexShader[static_cast<uint8_t>(VertexShaderType::MAX)];
	PixelShader*  g_pixelShader [static_cast<uint8_t>(PixelShaderType::MAX)];
}

HRESULT Graphics::InitShaderList()
{
	HRESULT hr = S_OK;

	const std::string entryPath = "Assets/Shader/";
	const std::string fileType = ".cso";
	const std::string vertexShaderPath[] =
	{
		"SampleVS"
	};
	const std::string pixelShaderPath[] =
	{
		"SamplePS"
	};

	// 頂点シェーダ読み込み
	for (uint8_t cnt = 0; auto& element : vertexShaderPath)
	{
	 	hr = g_vertexShader[cnt]->Load((entryPath + element + fileType).c_str());
		if (FAILED(hr))
			return hr;
		cnt++;
	}
	// ピクセルシェーダ読み込み
	for (uint8_t cnt = 0; auto& element : pixelShaderPath)
	{
		hr = g_pixelShader[cnt]->Load((entryPath + element + fileType).c_str());
		if (FAILED(hr))
			return hr;
		cnt++;
	}
	return hr;
}

void Graphics::UninitShaderList()
{
	for (auto& element : g_vertexShader)
		safe_delete(element);
	for (auto& element : g_pixelShader)
		safe_delete(element);
}

inline const VertexShader* Graphics::GetVertexShader(VertexShaderType type)
{
	return g_vertexShader[static_cast<uint8_t>(type)];
}

inline const PixelShader* Graphics::GetPixelShader(PixelShaderType type)
{
	return g_pixelShader[static_cast<uint8_t>(type)];
}