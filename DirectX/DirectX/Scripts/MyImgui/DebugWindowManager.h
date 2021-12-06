#pragma once
/**
* @file DebugWindowManager.h
* @breif �f�o�b�O�p�E�B���h�E���Ǘ�
*
* @author �җI��
* @date 2021/06/21
* @detial DebugWindow���p�����Ċe�E�B���h�E�𐶐�
*			DebugwindowManager�ŊǗ�
*/
#include <Windows.h>
#include <list>
#include "Scripts/DX11Manager.h"
#include "DebugWindow.h"

class DebugWindowManager
{
public:
	DebugWindowManager(HWND hwnd, ID3D11Device* pdevice, ID3D11DeviceContext* pdeviceContext);
	~DebugWindowManager();

	void Draw();
	void AddDebugWindow(DebugWindow* window);
	void RemoveDebugWindow(DebugWindow* window);
	
private:
	HWND m_hwnd;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	std::list<DebugWindow*> m_listDebugWindow;

};