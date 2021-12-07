#ifndef __SCENE_MANAGER_HPP__
#define __SCENE_MANAGER_HPP__

#include <System/Scene/SceneBase.h>
#include <System/Singleton.hpp>

#define SCENE SceneManager::GetInstance()

class SceneManager : public Singleton<SceneManager>
{
public:
	SceneManager();
	~SceneManager();

public:
	void Update(float tick);
	void Draw();

	template<class T> HRESULT Load();

private:
	static SceneManager* m_pInstance;
	Scene *m_pScene;
};


template<class T>
HRESULT SceneManager::Load()
{
	// テンプレートのデータ型チェック
	static_assert(std::is_base_of<Scene, T>::value,
		"[SceneManager::Create<T>] template T not inherit from 'Scene'");

	if (m_pScene)
	{
		m_pScene->Release();
		delete m_pScene;
	}
	m_pScene = new T;
	return m_pScene->Load();
}


#endif