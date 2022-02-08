#pragma once
#include "Component.h"
#include <System\Graphics\Shader.h>
#include <memory>
#include <unordered_map>
#include <System\Graphics\Texture.h>
#include <DirectXMath.h>
#include <System\Graphics\ShaderList.h>

class Material final : public Component
{
public:
	template<typename T> using UniquePtr = std::unique_ptr<T>;
	template<typename T> using WeakPtr = std::weak_ptr<T>;
	
	/**
	* @brief シェーダに必要なデータを構成するクラス
	* @details シェーダに使うレジスタ一つにつき一つ用意
	*/
	struct RegisterGPU
	{
		enum class BindShaderType
		{
			vertex = 0,
			pixel
		};

		WeakPtr<DirectX::XMFLOAT4> m_data;// レジスタに格納するデータ(外部で用意して参照を行う)
		UniquePtr<ConstantBuffer> m_constantBuffer;
		BindShaderType m_bindShaderType;
		uint8_t m_slot;
		
		RegisterGPU(WeakPtr<DirectX::XMFLOAT4> data, BindShaderType bindShaderType, uint8_t slot):
			m_data(data),
			m_bindShaderType(bindShaderType),
			m_slot(slot)
		{
			m_constantBuffer = std::make_unique<ConstantBuffer>();
			m_constantBuffer->Write(m_data.lock().get()); // maybe: Write関数のなかもスマートポインタで扱う？
		}
		RegisterGPU(RegisterGPU& ref)
		{
			*this = ref;
		}

		RegisterGPU& operator=(const RegisterGPU& ref)
		{
			m_data = ref.m_data;
			m_constantBuffer.reset(new ConstantBuffer(*ref.m_constantBuffer));	// 値だけ渡す // maybe: 多分大丈夫
			m_bindShaderType = ref.m_bindShaderType;
			m_slot = ref.m_slot;
		}

		/** 
		 *	@ brief コンスタンスバッファ書き換え
		 */
		void Bind()
		{
			m_constantBuffer->Write(m_data.lock().get());
			switch (m_bindShaderType)
			{
				using enum BindShaderType;
			case vertex:
				m_constantBuffer->BindVS(m_slot);
				break;
			case pixel:
				m_constantBuffer->BindVS(m_slot);
				break;
			default:
				// maybe: エラー処理
				break;
			}
		}
	};

public:
	Material(Object* pObject, RegisterGPU registerGPU, VertexShaderType vertexShader, PixelShaderType pixelShader);
	~Material();
	/**
	* @brief 描画に必要なデータを結びつける
	* @details テクスチャ、レジスタデータ、シェーダの種類
	*/
	void Bind();

private:
	RegisterGPU m_registerGPU;	// maybe: 複数のレジスタが扱えないので、将来的にリスト化？
	UniquePtr<Texture> m_texture;	// maybe: テクスチャも一元管理。一部個別管理も必要か？
	WeakPtr<const VertexShader> m_vertexShader;
	WeakPtr<const PixelShader> m_pixelShader;
};
