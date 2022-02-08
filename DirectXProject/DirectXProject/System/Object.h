#pragma once

#include <vector>
#ifndef __COMPONENT_H__
#include "Component/Component.h"
#endif
#include "Component/Transform.h"
#include "Graphics/DirectX.h"
#include <string>
#include <typeinfo>

class Component;
class Transform;

class Object
{
public:
	Object(std::string name);
	virtual ~Object();

	// �t���[�������֐��Q
	/**
	* @brief ��ɕϐ��̍X�V���s��
	* @details 1�t���[����1�x�����Ă΂��
	*/
	virtual void Update();
	/**
	* @brief �`��̐ݒ���s��
	* @details 1�t���[����1�x�����Ă΂��
	*/
	virtual void Draw();
	/**
	* @brief �������j�󂳂ꂽ���̏���
	* @details �j�󂳂ꂽ�t���[���̏I�����ɌĂ΂��
	*/
	virtual void OnDestroy();

	// �R���|�[�l���g�֐��Q
	/**
	* @brief �I�u�W�F�N�g�ɃR���|�[�l���g��ǉ�����
	* @return T �����Œǉ������R���|�[�l���g�A���s��nullptr���Ԃ�
	* @note �����R���|�[�l���g�͕������ĂȂ��B
	*		���㕡�����Ă�R���|�[�l���g���ǉ�����\��
	*/
	template<typename T> T* AddComponent();
	/**
	* @brief �I�u�W�F�N�g�ɂ��Ă���R���|�[�l���g���擾����
	* @return �����Ŏw�肵���R���|�[�l���g�A���s��nullptr���Ԃ�
	*/
	template<typename T> inline T* GetComponent();
	/**
	* @brief �I�u�W�F�N�g�ɂ��Ă���Ή�����R���|�[�l���g�����ׂĎ擾����
	* @return �w�肵���R���|�[�l���g�����X�g�Ɋi�[���A������Ȃ��ꍇnullptr���Ԃ�
	* @note �����R���|�[�l���g�͕������ĂȂ��B
	*		���㕡�����Ă�R���|�[�l���g���ǉ�����\��
	*/
	template<typename T> inline std::vector<T*> GetComponents();

	// ���g�̏�ԂɊւ���֐��Q
	virtual bool ActiveFlag() final;
	virtual void ActiveFlag(bool flag) final;
	virtual bool DestroyFlag() final;
	virtual void DestroyFlag(bool flag) final;

	// �I�u�W�F�N�g�֐��Q
	static void Destoroy(Object* const& obj);


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
	// �R���|�[�l���g�łȂ����̂�r��
	// guess: �R���p�C�����G���[�̖@�������H
	if (std::is_base_of(Component, T) == false)
		return nullptr;

	for (const auto& element : m_listComponent)
	{// �����R���|�[�l���g��r��
		if (typeid(*element) != typeid(T)) continue;
		return nullptr;
	}

	// �R���|�[�l���g�ǉ�
	T* buff = new T(this);
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