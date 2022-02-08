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
	Transform(Object* pObject);
	~Transform() = default;

	// 
	void Update() override;

	/**
	* @brief インデックスから子オブジェクトを検索
	* @param index
	* @return Transform* 見つけたオブジェクトのトランスフォーム
	*/
	Transform* GetChild(uint16_t index);
	/**
	* @brief 名前から子オブジェクトを検索
	* @param name
	* @return Transform* 見つけたオブジェクトのトランスフォーム
	*/
	Transform* FindChild(std::string name);

public:
	// ワールド空間における絶対的な値
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_rotate;

	// local ... 親からの相対的な値
	DirectX::XMFLOAT3 m_localPosition;
	DirectX::XMFLOAT3 m_localScale;
	DirectX::XMFLOAT3 m_localRotate;

private:
	std::vector<Transform*> m_listChildren;
};
