#include "FrameManager.h"
#include "imgui/imgui.h"
#pragma comment(lib,"winmm.lib")

FrameManagerWindow::FrameManagerWindow(std::string windowName, bool flag, ImGuiWindowFlags windowFlags)
	:DebugWindow(windowName, flag, windowFlags)
{
	// フレームカウント初期化
	timeBeginPeriod(1);				// 分解能を設定
	m_dwUpdateLastTime = m_dwExecLastTime = m_dwFPSLastTime = timeGetTime();
	m_dwCurTime = m_dwFrameCount = 0;
	m_nCountFPS = 0;

	m_FrameRate = 60;
	m_updateFlag = false;
	m_deltaTime = 0;
	m_fixedDeltaTime = 0;
}
FrameManagerWindow::~FrameManagerWindow()
{

}

void FrameManagerWindow::Update()
{
	m_dwCurTime = timeGetTime();
	if ((m_dwCurTime - m_dwFPSLastTime) >= 250)
	{	// 0.25秒ごとに実行
		m_deltaTime = m_dwCurTime - m_dwUpdateLastTime;
		
		m_nCountFPS = m_dwFrameCount * 1000 / (m_dwCurTime - m_dwFPSLastTime);
		m_dwFPSLastTime = m_dwCurTime;
		m_dwFrameCount = 0;
	}
	if (m_FrameRate <= 0);	// 仮で0をはじく
	else if ((m_dwCurTime - m_dwExecLastTime) >= (1000 / m_FrameRate))
	{ // FixUpdate制御
		m_fixedDeltaTime = m_dwCurTime - m_dwExecLastTime;
		m_dwExecLastTime = m_dwCurTime;
		m_updateFlag = true;
	}
	else
	{
		m_updateFlag = false;
	}
	m_dwFrameCount++;
	m_dwUpdateLastTime = m_dwCurTime;

}
void FrameManagerWindow::Draw()
{
	ImGui::Text("FrameRate     : %d", m_nCountFPS);
	ImGui::Text("DeltaTime     : %.3fms", m_deltaTime);
	ImGui::Text("FixedDeltaTime: %.3fms", m_fixedDeltaTime);
	ImGui::SliderInt("FixedFlameRate", &m_FrameRate, FRAMERATE_MIN, FRAMERATE_MAX);
}


void FrameManagerWindow::SetFrameRate(int val)
{
	if (val < FRAMERATE_MIN) { val = FRAMERATE_MIN; }
	else if (val > FRAMERATE_MAX) { val = FRAMERATE_MAX; }

	m_FrameRate = val;
}
bool FrameManagerWindow::GetFlag()
{
	return m_updateFlag;
}
