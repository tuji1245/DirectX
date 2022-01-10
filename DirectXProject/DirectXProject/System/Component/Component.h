#pragma once


class Object;

class Component abstract
{
public:
	Component(Object* pObject);
	virtual ~Component();

	virtual void Init() {};
	virtual void Update() = 0;
	virtual void Draw() {}

public:
	Object* m_pObject;

protected:
	bool m_destoroy;
};

#define __COMPONENT_H__