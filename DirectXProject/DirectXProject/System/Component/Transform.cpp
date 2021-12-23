#include "Transform.h"
#include <System/Object.h>


Transform::Transform(Object* pParent):
	Component(pParent),
	m_posittion(),
	m_scale(),
	m_rotate(),
	m_listChildren()
{
}


Transform* Transform::GetChild(int index)
{
	if (m_listChildren.size() < index)
		return nullptr;

	return m_listChildren[index];
}

Transform* Transform::FindChild(std::string name)
{
	for (auto element : m_listChildren)
		if (element->m_pParent->m_name == name)
			return element;

	return nullptr;
}

