#include "Object.h"

Object::Object(std::string name):
	m_transform(this),
	m_name(name),
	m_active(true),
	m_destoroy(false),
	m_listComponent()
{	
}

Object::~Object()
{
	// コンポーネント解放
	for (auto& it : m_listComponent)
		safe_delete(it);
	m_listComponent.clear();
}

void Object::Update()
{
}

void Object::Draw()
{
}

void Object::OnDestroy()
{
}

bool Object::ActiveFlag()
{
	return m_active;
}

void Object::ActiveFlag(bool flag)
{
	m_active = flag;
}

bool Object::DestroyFlag()
{
	return m_destoroy;
}

void Object::DestroyFlag(bool flag)
{
	m_destoroy = flag;
}

void Object::Destoroy(Object* const& obj)
{
	obj->m_destoroy = true;
}
