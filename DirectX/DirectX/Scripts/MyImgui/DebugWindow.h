#pragma once
/**
* @file DebugWindow.h
* @breif デバッグ用のウィンドウはこれを継承して作る
*
* @date 2021/06/21
* @author 辻悠希
*/
#include "imgui/imgui.h"
#include <string>

class DebugWindow
{
public:
	DebugWindow(std::string windowName, bool flag = true, ImGuiWindowFlags windowFlags = 0);
	
	virtual void DrawBegin() final;
	virtual void Draw() = 0;
	virtual void DrawEnd() final;
	virtual void Update() = 0;

public:
	bool m_showFlag;
	std::string m_windowName;
	ImGuiWindowFlags m_WindowFlags;

};