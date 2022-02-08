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
	* @brief �C���f�b�N�X����q�I�u�W�F�N�g������
	* @param index
	* @return Transform* �������I�u�W�F�N�g�̃g�����X�t�H�[��
	*/
	Transform* GetChild(uint16_t index);
	/**
	* @brief ���O����q�I�u�W�F�N�g������
	* @param name
	* @return Transform* �������I�u�W�F�N�g�̃g�����X�t�H�[��
	*/
	Transform* FindChild(std::string name);

public:
	// ���[���h��Ԃɂ������ΓI�Ȓl
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_rotate;

	// local ... �e����̑��ΓI�Ȓl
	DirectX::XMFLOAT3 m_localPosition;
	DirectX::XMFLOAT3 m_localScale;
	DirectX::XMFLOAT3 m_localRotate;

private:
	std::vector<Transform*> m_listChildren;
};
