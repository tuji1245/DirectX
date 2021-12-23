#include "SceneBase.h"


Scene::Scene(const char* sceneName)
	: m_name(sceneName)
{
}
Scene::~Scene()
{
}

void Scene::EndFrame()
{
	{
		// �I�u�W�F�N�g�I������
		auto it = m_listpObject.begin();
		auto end = m_listpObject.end();
		while (it != end)
		{
			if (!(*it)->DestroyFlag())
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
}
