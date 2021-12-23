#include"Component.h"
#include <System\Graphics\DirectX.h>

Component::Component(Object* pParent):
	m_pParent(pParent),
	m_destoroy(false)
{
}

Component::~Component()
{
	safe_delete(m_pParent);
}
