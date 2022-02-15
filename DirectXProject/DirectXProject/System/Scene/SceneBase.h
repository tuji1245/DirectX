#ifndef __SCENE_BASE_H__
#define __SCENE_BASE_H__

#include <Windows.h>
#include <string>
#include <vector>
#include <System/Object.h>

#define SCENE_DECLARATION(scene) \
public:\
	scene();\
	virtual ~scene();\
	HRESULT Load();\
	void Release();\
	void Update(float tick);\
	void Draw() // ; �����Ă��ĂȂ�

class Scene abstract
{
public:
	Scene(const char *sceneName);
	virtual ~Scene();

	// ��{����
	/**
	* @brief �V�[���̓ǂݍ���
	* @return HRESULT ������ 0, ���s�� 0��菬�����l
	* @details ���̃V�[���ɕK�v�Ȃ��̂����ׂēǂݍ���
	*/
	virtual HRESULT Load() = 0;
	/**
	* @brief �V�[���̔j��
	* @details Load()�œǂݍ��񂾂��̂����ׂĉ��
	*/
	virtual void Release() = 0;
	/**
	* @brief 1�t���[����1�x�����Ă΂��
	* @deteils �I�u�W�F�N�g�����ׂčX�V����
	*/
	virtual void Update(float tick) = 0;
	/**
	* @brief �`����s��
	* @details �`��ݒ���s��
	*/
	virtual void Draw() = 0;
	/**
	* @brief �t���[���I��������
	* @detail DestroyFlag���Q�Ƃ��ăI�u�W�F�N�g�j��
	*/
	virtual void EndFrame() final;

protected:
	std::vector<Object*> m_listpObject;

private:
	std::string m_name;
};

#endif // __SCENE_BASE_H__