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
	* @brief �V�F�[�_�ɕK�v�ȃf�[�^���\������N���X
	* @details �V�F�[�_�Ɏg�����W�X�^��ɂ���p��
	*/
	struct RegisterGPU
	{
		enum class BindShaderType
		{
			vertex = 0,
			pixel
		};

		WeakPtr<DirectX::XMFLOAT4> m_data;// ���W�X�^�Ɋi�[����f�[�^(�O���ŗp�ӂ��ĎQ�Ƃ��s��)
		UniquePtr<ConstantBuffer> m_constantBuffer;
		BindShaderType m_bindShaderType;
		uint8_t m_slot;
		
		RegisterGPU(WeakPtr<DirectX::XMFLOAT4> data, BindShaderType bindShaderType, uint8_t slot):
			m_data(data),
			m_bindShaderType(bindShaderType),
			m_slot(slot)
		{
			m_constantBuffer = std::make_unique<ConstantBuffer>();
			m_constantBuffer->Write(m_data.lock().get()); // maybe: Write�֐��̂Ȃ����X�}�[�g�|�C���^�ň����H
		}
		RegisterGPU(RegisterGPU& ref)
		{
			*this = ref;
		}

		RegisterGPU& operator=(const RegisterGPU& ref)
		{
			m_data = ref.m_data;
			m_constantBuffer.reset(new ConstantBuffer(*ref.m_constantBuffer));	// �l�����n�� // maybe: �������v
			m_bindShaderType = ref.m_bindShaderType;
			m_slot = ref.m_slot;
		}

		/** 
		 *	@ brief �R���X�^���X�o�b�t�@��������
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
				// maybe: �G���[����
				break;
			}
		}
	};

public:
	Material(Object* pObject, RegisterGPU registerGPU, VertexShaderType vertexShader, PixelShaderType pixelShader);
	~Material();
	/**
	* @brief �`��ɕK�v�ȃf�[�^�����т���
	* @details �e�N�X�`���A���W�X�^�f�[�^�A�V�F�[�_�̎��
	*/
	void Bind();

private:
	RegisterGPU m_registerGPU;	// maybe: �����̃��W�X�^�������Ȃ��̂ŁA�����I�Ƀ��X�g���H
	UniquePtr<Texture> m_texture;	// maybe: �e�N�X�`�����ꌳ�Ǘ��B�ꕔ�ʊǗ����K�v���H
	WeakPtr<const VertexShader> m_vertexShader;
	WeakPtr<const PixelShader> m_pixelShader;
};
