#include "CameraManager.hpp"
#include "CameraDebug.h"

CameraManager* CameraManager::m_pInstance = nullptr;

CameraManager* CameraManager::GetInstance()
{
	return m_pInstance;
}
void CameraManager::Instantiate()
{
	Destroy();
	m_pInstance = new CameraManager;
}
void CameraManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

CameraManager::CameraManager()
{
	m_mainCamera = Create<CameraDebug>();
	m_list[m_mainCamera]->Enable(true);
}
CameraManager::~CameraManager()
{
	CameraList::iterator it = m_list.begin();
	while (it != m_list.end())
	{
		delete (*it);
		++it;
	}
}

void CameraManager::Update()
{
	CameraList::iterator it = m_list.begin();
	while (it != m_list.end())
	{
		(*it)->Execute();
		++it;
	}
}
void CameraManager::Switch(int no)
{
	if (0 <= no && no < m_list.size())
	{
		int old = m_mainCamera;
		m_list[old]->Enable(false);
		m_list[no]->Enable(true);
		m_list[no]->OnActive(*m_list[old]);
		m_mainCamera = no;
	}
}
Camera*CameraManager::Get(int no)
{
	if (no == -1)
	{
		no = m_mainCamera;
	}
	return m_list[m_mainCamera];
}