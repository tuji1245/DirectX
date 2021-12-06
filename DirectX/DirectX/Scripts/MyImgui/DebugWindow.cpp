#include "DebugWindow.h"

DebugWindow::DebugWindow(std::string windowName, bool showFlag, ImGuiWindowFlags windowFlags)
{
	m_windowName = windowName;
	m_showFlag = showFlag;
	m_WindowFlags = windowFlags;
}


void DebugWindow::DrawBegin()
{
	ImGui::Begin(m_windowName.c_str(), &m_showFlag, m_WindowFlags);
}
void DebugWindow::DrawEnd()
{
	ImGui::End();
}