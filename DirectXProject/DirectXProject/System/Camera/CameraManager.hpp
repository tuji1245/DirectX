#ifndef __CAMERA_MANAGER_HPP__
#define __CAMERA_MANAGER_HPP__

#include "CameraBase.h"
#include <vector>

#define CAMERA CameraManager::GetInstance()

class CameraManager
{
private:
	using CameraList = std::vector<Camera*>;

public:
	static CameraManager* GetInstance();
	static void Instantiate();
	static void Destroy();
private:
	CameraManager();
	~CameraManager();

public:	
	void Update();
	void Switch(int no);
	template<class T> int Create();
	Camera* Get(int no = -1);
	
private:
	static CameraManager* m_pInstance;
	CameraList m_list;
	int m_mainCamera;
};


template<class T>
int CameraManager::Create()
{
	// テンプレートのデータ型チェック
	static_assert(std::is_base_of<Camera, T>::value,
		"[CameraManager::Create<T>] template T not inherit from 'Camera'");
	
	m_list.push_back(new T);
	return static_cast<int>(m_list.size() - 1);
}


#endif