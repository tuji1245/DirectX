#pragma once
/**
* @file DebugWindow.h
* @breif デバッグ用のウィンドウはこれを継承して作る
*
* @date 2021/06/21
* @author 辻悠希
*/
#include "Scripts/MyImgui/DebugWindow.h"

class DemoImgui final : public DebugWindow
{
public:
	DemoImgui(std::string windowName, bool flag = true, ImGuiWindowFlags windowFlags = 0);
	void Draw();
	void Update();

private:
	bool show_demo_window;
	ImVec4 clear_color;
};