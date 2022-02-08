#include"Component.h"
#include <System\Graphics\DirectX.h>

Component::Component(Object* pObject):
	m_pObject(pObject),
	m_destoroy(false)
{
}

Component::~Component()
{
	safe_delete(m_pObject);
}

void Component::Init()
{
}

void Component::Update()
{
}

void Component::Draw()
{
}

bool Component::ActiveFlag()
{
	return m_active;
}

void Component::ActiveFlag(bool flag)
{
	m_active = flag;
}

bool Component::DestroyFlag()
{
	return m_destoroy;
}

void Component::DestroyFlag(bool flag)
{
	m_destoroy = flag;
}
