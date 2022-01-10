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
