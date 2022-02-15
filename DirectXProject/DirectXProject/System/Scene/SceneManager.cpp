#include <System/Scene/SceneManager.hpp>
#include <System/Scene/SampleScene.h>

SceneManager::SceneManager()
	: m_pScene(nullptr)
{
	Load<SampleScene>();
}
SceneManager::~SceneManager()
{
	if (m_pScene)
	{
		delete m_pScene;
		m_pScene = nullptr;
	}
}


void SceneManager::Update(float tick)
{
	m_pScene->Update(tick);
}
void SceneManager::Draw()
{
	m_pScene->Draw();
}

void SceneManager::EndFrame()
{
	m_pScene->EndFrame();
}
