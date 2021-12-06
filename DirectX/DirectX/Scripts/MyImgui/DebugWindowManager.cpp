#include "DebugWindowManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

DebugWindowManager::DebugWindowManager(HWND hwnd, ID3D11Device* pdevice, ID3D11DeviceContext* pdeviceContext) :
    m_hwnd(hwnd), m_pDevice(pdevice), m_pDeviceContext(pdeviceContext)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
   
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
 
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(m_hwnd);
    ImGui_ImplDX11_Init(m_pDevice, m_pDeviceContext);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);
    
    m_listDebugWindow.clear();
}

DebugWindowManager::~DebugWindowManager()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void DebugWindowManager::Draw()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // ----- ManagerWindow �`��J�n
    ImGui::Begin("WindowManager",0, ImGuiWindowFlags_MenuBar);
    for (auto& it : m_listDebugWindow)
    {
        // ���X�g���E�B���h�E�\���t���O�̃`�F�b�N�{�b�N�X
        ImGui::Checkbox(it->m_windowName.c_str(), &it->m_showFlag);
    }
    // ----- ManagerWindow �`��I��
    ImGui::End();
    
    // �\���t���O�Ɋ�Â��ăE�B���h�E�\��
    for (const auto& it : m_listDebugWindow)
    {
        if (!it->m_showFlag) continue;  // �����ڃt���O��false�Ȃ�`�悵�Ȃ�
        it->DrawBegin();
        it->Draw();
        it->DrawEnd();
    }

    // Imgui�̏���������
    ImGui::Render();
    /** ImGui::EndFrame(); 
     * ImGui::Render()���Ă΂Ȃ��Ƃ��͌ĂԕK�v����
     * �g�p���邩�͌����� 
     */

    // ImGui�`��
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void DebugWindowManager::AddDebugWindow(DebugWindow* window)
{
    for (const auto& it : m_listDebugWindow)
    {
        if (it == window)
            return; // ���łɒ�`����Ă���Ƃ��͒ǉ����Ȃ�
    }
    m_listDebugWindow.push_back(window);
}

void DebugWindowManager::RemoveDebugWindow(DebugWindow* window)
{
    m_listDebugWindow.remove(window);
}
