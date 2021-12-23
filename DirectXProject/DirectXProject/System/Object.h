#pragma once

#include <vector>
#ifndef __COMPONENT_H__
#include "Component/Component.h"
#endif
#include "Component/Transform.h"
#include "Graphics/DirectX.h"
#include <string>

class Component;
class Transform;

class Object
{
public:
	Object(std::string name);
	virtual ~Object();


	virtual void Update() = 0;
	virtual void Draw() = 0;

	template<typename T>
	T* AddComponent();

	template<typename T>
	inline T* GetComponent();

	template<typename T>
	inline std::vector<T*> GetComponents();

	static void Destoroy(Object* const& obj);
	
	bool ActiveFlag();
	void ActiveFlag(bool flag);
	bool DestroyFlag();
	void DestroyFlag(bool flag);


public:
	Transform m_transform;
	std::string m_name;

private:
	bool m_active;
	bool m_destoroy;
	std::vector<Component*> m_listComponent;
};

template<typename T>
T* Object::AddComponent()
{
	// コンポーネント
	for (const auto& element : m_listComponent)
	{
		if (typeid(*element) != typeid(T)) continue;
		return nullptr;
	}

	T* buff = new T;
	buff->m_pParent = this;
	m_listComponent.push_back(buff);
	return buff;
}
template<typename T>
T* Object::GetComponent()
{
	for (auto& element : m_listComponent)
	{
		T* buff = dynamic_cast<T*>(element);
		if (buff != nullptr)
			return buff;
	}
	return nullptr;
}

template<typename T>
std::vector<T*> Object::GetComponents()
{
	std::vector<T*> returnBuff;
	for (auto& element : m_listComponent)
	{
		T* buff = dynamic_cast<T*>(element);
		if (buff != nullptr)
			returnBuff.push_back(buff);
	}
	return returnBuff;
}