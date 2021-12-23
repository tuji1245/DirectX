#pragma once


class Object;

class Component abstract
{
public:
	Component(Object* pParent);
	virtual ~Component();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() {}

public:
	Object* m_pParent;

protected:
	bool m_destoroy;
};

#define __COMPONENT_H__