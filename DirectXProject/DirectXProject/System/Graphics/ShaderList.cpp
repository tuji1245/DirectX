#include "ShaderList.h"

ShaderList::ShaderList():
	m_listVertexShader(),
	m_listPixelShader()
{
}

ShaderList::~ShaderList()
{
	Release();
}

HRESULT ShaderList::Load()
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
	for (uint8_t cnt = 0; auto & element : vertexShaderPath)
	{
		m_listVertexShader[cnt] = std::make_shared<VertexShader>();
		hr = m_listVertexShader[cnt]->Load((entryPath + element + fileType).c_str());
		if (FAILED(hr))
			return hr;
		cnt++;
	}
	// ピクセルシェーダ読み込み
	for (uint8_t cnt = 0; auto & element : pixelShaderPath)
	{
		m_listPixelShader[cnt] = std::make_shared<PixelShader>();
		hr = m_listPixelShader[cnt]->Load((entryPath + element + fileType).c_str());
		if (FAILED(hr))
			return hr;
		cnt++;
	}
	return hr;
}

void ShaderList::Release()
{
	// 解放
	for (auto& element : m_listVertexShader)
		element.reset();
	for (auto& element : m_listPixelShader)
		element.reset();
}

std::weak_ptr<const VertexShader> ShaderList::GetVertexShader(VertexShaderType type) const
{
	// guess: shard -> weak cast?
	return m_listVertexShader[static_cast<uint8_t>(type)];
}

std::weak_ptr<const PixelShader> ShaderList::GetPixelShader(PixelShaderType type) const
{
	// guess: shard -> weak cast?
	return m_listPixelShader[static_cast<uint8_t>(type)];
}
