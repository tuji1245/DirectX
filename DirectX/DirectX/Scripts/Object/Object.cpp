#include "Object.h"

Object::Object()
{

}
Object::~Object()
{

}

void Object::Awake()
{
}

void Object::OnEnable()
{
}

void Object::Start()
{

}
void Object::Update()
{
	for (const auto& temp : m_listComponent)
	{
		temp->Update();
	}
}
void Object::FixedUpdate()
{
}
void Object::OnDisEnable()
{
}
void Object::Terminate()
{

}