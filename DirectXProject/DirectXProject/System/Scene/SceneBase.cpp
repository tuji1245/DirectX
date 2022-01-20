#include "SceneBase.h"


Scene::Scene(const char* sceneName)
	: m_name(sceneName)
{
}
Scene::~Scene()
{
	auto it = m_listpObject.begin();
	auto end = m_listpObject.end();
	for (const auto& element : m_listpObject)
	{
		delete element;
	}
	m_listpObject.clear();
}

void Scene::EndFrame()
{
	// オブジェクト終了処理
	for (auto it = m_listpObject.begin(); it != m_listpObject.end();)
	{
		if (!(*it)->DestroyFlag()) [[unlikely]]
		{
			delete *it;
			it = m_listpObject.erase(it);
		}
		else
		{
			++it;
		}
	}
}
