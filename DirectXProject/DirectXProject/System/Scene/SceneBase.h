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

class Scene
{
public:
	Scene(const char *sceneName);
	virtual ~Scene();

	virtual HRESULT Load() = 0;
	virtual void Release() = 0;
	virtual void Update(float tick) = 0;
	virtual void Draw() = 0;
	/**
	* @fn
	* �����Ɋ֐��̐���������
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