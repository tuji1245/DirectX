#include "Material.h"

Material::Material(Object* pObject, RegisterGPU registerGPU, VertexShaderType vertexShader, PixelShaderType pixelShader) :
	Component(pObject),
	m_registerGPU(registerGPU)
{
	auto shaderInst = ShaderList::GetInstance();
	m_vertexShader = shaderInst->GetVertexShader(vertexShader);
	m_pixelShader = shaderInst->GetPixelShader(pixelShader);
}

Material::~Material()
{
}

void Material::Bind()
{
	// �V�F�[�_�ƕK�v�ȃ��\�[�X��R�Â�
	m_registerGPU.Bind();	// ���W�X�^���
	SetTexture(m_texture->GetResource(), 0);	// �e�N�X�`��
	m_vertexShader.lock()->Bind();	// �V�F�[�_
	m_pixelShader.lock()->Bind();
}
