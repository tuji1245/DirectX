#pragma once
#include "Shader.h"

namespace Graphics
{
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
	HRESULT InitShaderList();
	void UninitShaderList();


	inline const VertexShader* GetVertexShader(VertexShaderType type);
	inline const PixelShader* GetPixelShader(PixelShaderType type);
}
