#include "Model.h"

Model::Model(Object* pObject):
	Component(pObject)
{
}

Model::~Model()
{
}

void Model::Draw()
{
	m_model.lock()->Draw();
}

void Model::SetModel(ModelList::Type type)
{
	m_model = ModelList::GetInstance()->GetModel(type);
}
