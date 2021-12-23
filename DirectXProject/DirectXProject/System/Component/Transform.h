#pragma once

#include <DirectXMath.h>
#include <vector>
#include <string>
#ifndef __COMPONENT_H__
#include "Component.h"
#endif

class Object;

class Transform : public Component
{
public:
	Transform(Object* pParent);
	~Transform() = default;

	void Init() override;
	void Update() override;

	Transform* GetChild(int index);
	Transform* FindChild(std::string name);

public:
	DirectX::XMFLOAT3 m_posittion;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_rotate;

private:
	std::vector<Transform*> m_listChildren;
};
