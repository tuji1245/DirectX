#pragma once
#include "Shader.h"
#include <System\Singleton.hpp>
#include <memory>

enum class VertexShaderType
{
	Normal,
	MAX
}; 

enum class PixelShaderType
{
	Normal,
	MAX
};

class ShaderList : public Singleton<ShaderList>
{
public:
	ShaderList();
	~ShaderList();

	[[nodiscard]] HRESULT Load();
	void Release();

	std::weak_ptr<const VertexShader> GetVertexShader(VertexShaderType type) const;
	std::weak_ptr<const PixelShader> GetPixelShader(PixelShaderType type) const;
	
private:
	std::shared_ptr<VertexShader> m_listVertexShader[static_cast<uint8_t>(VertexShaderType::MAX)];
	std::shared_ptr<PixelShader> m_listPixelShader[static_cast<uint8_t>(VertexShaderType::MAX)];
};
