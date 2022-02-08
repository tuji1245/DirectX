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
	// シェーダと必要なリソースを紐づけ
	m_registerGPU.Bind();	// レジスタ情報
	SetTexture(m_texture->GetResource(), 0);	// テクスチャ
	m_vertexShader.lock()->Bind();	// シェーダ
	m_pixelShader.lock()->Bind();
}
