#pragma once

class Object;

class Component abstract
{
public:
	Component(Object* pObject);
	virtual ~Component();

	// 
	virtual void Init();
	virtual void Update();
	virtual void Draw();

	// 
	virtual bool ActiveFlag() final;
	virtual void ActiveFlag(bool flag) final;
	virtual bool DestroyFlag() final;
	virtual void DestroyFlag(bool flag) final;

public:
	Object* m_pObject;

protected:
	bool m_active;
	bool m_destoroy;
};

#define __COMPONENT_H__