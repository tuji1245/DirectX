#include "Transform.h"
#include <System/Object.h>


Transform::Transform(Object* pObject):
	Component(pObject),
	m_position(),
	m_scale(),
	m_rotate(),
	m_localPosition(),
	m_localScale(),
	m_localRotate(),
	m_listChildren()
{
}


void Transform::Update()
{
	
}

Transform* Transform::GetChild(uint16_t index)
{
	if (m_listChildren.size() < index)
		return nullptr;

	return m_listChildren[index];
}

Transform* Transform::FindChild(std::string name)
{
	for (const auto& element : m_listChildren)
		if (element->m_pObject->m_name == name)
			return element;

	return nullptr;
}

