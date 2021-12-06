#pragma once
/**
* @file DebugWindowManager.h
* @breif デバッグ用ウィンドウを管理
*
* @author 辻悠希
* @date 2021/06/21
* @detial DebugWindowを継承して各ウィンドウを生成
*			DebugwindowManagerで管理
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