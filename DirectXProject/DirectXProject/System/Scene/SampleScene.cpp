#include <System/Scene/SampleScene.h>
#include <System/Graphics/Geometory.h>
#include <System\Component\Model.h>

SampleScene::SampleScene()
	: Scene("Sample")
{
	auto buff = new Object("Enemy");
	auto com = buff->AddComponent<Model>();
	com->SetModel(ModelList::Type::Smple);
	m_listpObject.push_back(buff);
}
SampleScene::~SampleScene()
{
}
HRESULT SampleScene::Load()
{
	return S_OK;
}
void SampleScene::Release()
{
}
void SampleScene::Update(float tick)
{
}
void SampleScene::Draw()
{
	
}
