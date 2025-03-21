#include "CNotepad.h"
#include <intrin.h>
//#include "Colors.h"

#pragma comment (lib , "Urlmon.lib")
uint64_t fDataMemUsage() // Work Function !!! Check Sym RAM to Current Program //
{
    PROCESS_MEMORY_COUNTERS pmc;
    pmc.cb = sizeof(pmc);
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    return pmc.WorkingSetSize;
}

#pragma once



struct WGL_WindowData { HDC hDC; };
static int64_t fwop = 0;
// Data
static HGLRC            g_hRC;
static WGL_WindowData   g_MainWindow;
static int              g_Width;
static int              g_Height;
ImVec4 clear_color = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);

// Forward declarations of helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data);
void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data);

//
void ResetDeviceWGL();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


int main(int, char** argv)
{


    ImDrawListSplitter JEApp;

    char fInputBuffer;
    ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_OWNDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"CNotepad OpenGL3.3", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"CNotepad build 0.2_x64_OpenGL3.3 (pre-alpha)", WS_OVERLAPPEDWINDOW | WS_EX_TOOLWINDOW | WS_EX_NOPARENTNOTIFY, 100, 80, 800, 600, nullptr, nullptr, wc.hInstance, nullptr);
   // ::SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_SIZEBOX);
    // Initialize OpenGL
    if (!CreateDeviceWGL(hwnd, &g_MainWindow))
    {
        CleanupDeviceWGL(hwnd, &g_MainWindow);
        ::DestroyWindow(hwnd);
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }
    wglMakeCurrent(g_MainWindow.hDC, g_hRC);
    CImageC.InitCImage("logo.png");
    // Show the window
   // ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::AnimateWindow(hwnd, 100, AW_BLEND);
    ::ShowWindow(GetConsoleWindow(), 2);
    ::UpdateWindow(hwnd);
    std::string fStrParam;
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_InitForOpenGL(hwnd);
    ImGui_ImplOpenGL3_Init();
    io.Fonts->AddFontFromFileTTF("Roboto_Mono.ttf", 20.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());//
    ImFontAtlas* fonts = ImGui::GetIO().Fonts;
    fonts->AddFontFromFileTTF("Roboto_Mono.ttf", 20.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    ImFont* font1 = io.Fonts->AddFontFromFileTTF("Roboto_Mono.ttf", 40.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    ImFont* font2 = io.Fonts->AddFontFromFileTTF("Roboto_Mono.ttf", 25.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    ImFont* font3 = io.Fonts->AddFontFromFileTTF("Roboto_Mono.ttf", 20.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    std::cout << "[JE_ENGINE] Loaded Font WhiteRabbit.ttf" << std::endl;
    struct GPU_DATA {
        std::string E_Brand = (const char*)glGetString(GL_VENDOR);
        std::string E_Model = (const char*)glGetString(GL_RENDERER);
        std::string E_GLVer = (const char*)glGetString(GL_VERSION);
    };
    GPU_DATA GPU;
    std::string fD_gpuBrand = GPU.E_Brand;
    std::string fD_gpuModel = GPU.E_Model;
    std::string fD_gpuGLVer = GPU.E_GLVer;
    std::cout << "[JE_ENGINE] OpenGL Driver Loaded" << std::endl;
    //SetColorAMD64(240);
    std::cout << "" << std::endl;
    std::cout << "" << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "" << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "" << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    // std::cout << "" << "OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "" << std::endl;
    int CPUInfo[4] = { -1 };
    unsigned   nExIds, i = 0;
    char CPUBrandString[0x40];
    // Get the information associated with each extended ID.
    __cpuid(CPUInfo, 0x80000000);
    nExIds = CPUInfo[0];
    for (i = 0x80000000; i <= nExIds; ++i)
    {
        __cpuid(CPUInfo, i);
        // Interpret CPU brand string
        if (i == 0x80000002)
            memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
        else if (i == 0x80000003)
            memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
        else if (i == 0x80000004)
            memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
    }
    std::string dCPUBrandString = CPUBrandString;
    // Main loop
    bool main_logo = true;
    bool done = false;
    //ND.ConfigParse("vsync=0", &ND.fBvsync, 0);
    //ND.ConfigParse("vsync=1", &ND.fBvsync, 1);
    //ND.ConfigParse("nLine=0", &ND.fNBufferLine, 0);
    //ND.ConfigParse("nLine=1", &ND.fNBufferLine, 1);
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        bool fB_loadFrame = false;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                exit(0);
            //  done = true;
            if (msg.message == WM_DESTROY)
                exit(0);
        }

        if (done)
            break;
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            bool fJEFrame = true;
            // std::cout << "[JE_ENGINE] Frame Cra" << std::endl;
            std::exception* d;
            int64_t a, b;
            //  std::cout << "ADDR:" << &d << "->" << d << std::endl;
            //  ImGui::Begin("\tJE x64_OpenGL3_SSE4.2 C++20",&fJEFrame, ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_NoTitleBar);  
                  // Create a window called "Hello, world!" and append into it
            JEApp.ClearFreeMemory();
            ImGuiStyle& style = ImGui::GetStyle();
            style.WindowRounding = 0.3f;
            style.FrameRounding = 0.0f;
            style.ScrollbarRounding = 0;
            style.TabRounding = 5;
            style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            style.Colors[ImGuiCol_WindowBg] = ImVec4(0.02f, 0.02f, 0.02f, 1.0f);//01409
            style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
            style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
            style.Colors[ImGuiCol_Border] = ImVec4(0.55f, 0.59f, 0.61f, 1.0f);
            style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            style.Colors[ImGuiCol_FrameBg] = ImVec4(0.02f, 0.02f, 0.02f, 1.0f);
            style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
            style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
            style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.08f, 0.80f);
            style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
            style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
            style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
            style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
            style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
            style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
            style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.02f, 0.02f, 0.02f, 1.0f);
            style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
            style.Colors[ImGuiCol_Button] = ImVec4(0.02f, 0.02f, 0.02f, 1.0f);
            style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.0f, 0.0f, 0.30f, 1.0f);
            style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
            style.Colors[ImGuiCol_Header] = ImVec4(0.02f, 0.02f, 0.02f, 1.0f);
            style.Colors[ImGuiCol_HeaderHovered] = ImVec4(1.0f, 0.0f, 0.30f, 1.0f);
            style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
            style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
            style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
            style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
            style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
            style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
            style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
            style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
            style.FrameBorderSize = 1;

        }

       
        JEApp.ClearFreeMemory();
     
        if (ND.fBvsync) {
            Sleep(13); ND.ConfigEdit("vsync=1");

        }
        else {
            Sleep(0); ND.ConfigEdit("vsync=0");

        }
        static int64_t dx = 0;
        static int64_t dy = 0;
        int64_t sztx[] = { 450,280,499 };
        ImGui::Begin("Notepad", &ND.fCNotepadWindow,ND.fW_noTitle | ND.fW_noMove);
        ND.dsx(&dx, &dy, hwnd);
        ND.fNavButtons = false;
        ND.FileCreateState = true;
        ImGui::SetWindowSize(ImVec2(dx, dy));
        ImGui::SetWindowPos(ImVec2(1, 1));
        if (ND.fNBufferLine) {
            ND.ConfigEdit("nLine=1");
        }
        else {
            ND.ConfigEdit("nLine=0");
        }
        
        if (ND.MakeConfig) {

            ND.SwapBuffer(&fC_commandStr);
            ImGui::Begin("КОНФИГ", &ND.MakeConfig, ND.fW_noMove | ND.fW_noColiapse | ND.fW_noResize);
            ImGui::SetWindowPos(ImVec2((dx - ImGui::GetWindowWidth()) / 2, (dy - ImGui::GetWindowHeight()) / 2));
            ImGui::SetCursorPosX(30);
            ImGui::PushFont(font1);
            ImGui::Text("CNotepad::Настроики");
            ImGui::PopFont();
            ImGui::SetCursorPosX(30);
            ImGui::MenuItem("Включить принудильный перенос строки", NULL, &ND.fNBufferLine, 1);
            ImGui::SetCursorPosX(30);
            ImGui::MenuItem("Включить Синхронизацию кадров", NULL, &ND.fBvsync, 1);
            ImGui::SetCursorPosX(30);
            ImGui::PushFont(font1);
            ImGui::Text("Запуск и компиляция");
            ImGui::PopFont();
            ImGui::SetCursorPosX(30);
            ImGui::InputText("Команда.", &fC_commandStr);
            ImGui::SetCursorPosX(30);
            ImGui::Checkbox("Выполнение кода в Runtime", &cbRuntimeBatch);
            if (ImGui::Button("ОК")) {
                ND.MakeConfig = false;
                if (ND.FileCreateState) {
                    ND.fNavButtons = false;
                    ND.OpenFileA(&ND.fCSTR_Buffer);
                }
                else {
                    ND.fNavButtons = true;
                }
            }
            ImGui::End();
        }
        if (ND.FileOpen) {
            ImGui::Begin("СОЗДАНИЕ ФАЙЛА", &ND.FileOpen, ND.fW_noMove | ND.fW_noColiapse | ND.fW_noResize);
            ImGui::SetWindowPos(ImVec2((dx - ImGui::GetWindowWidth()) / 2, (dy - ImGui::GetWindowHeight()) / 2));
            ImGui::SetCursorPosX(30);
            ImGui::Text("Введите Название файла");
            ImGui::SetCursorPosX(30);
            ImGui::InputText("Имя\n", &ND.FileNameOffset);
            ImGui::SetCursorPosX(30);
            if (ImGui::Button("Создать")) {
                ND.FileInit(&ND.FileCreateState);
                ND.FileOpenClose();
                ND.NavWindowClose();
            }ImGui::SameLine();
            if (ImGui::Button("Выбрать файл")) {
                fileCounts++;
                    ND.FileInit(&ND.FileCreateState);
                    ND.OpenFileN(&ND.fCSTR_Buffer);
                    ND.FileOpenClose();
                    ND.NavWindowClose();
                
            }
            ImGui::SameLine();
            if (ImGui::Button("Открыть")) {
                fileCounts++;
                ND.FileInit(&ND.FileCreateState);
                ND.OpenFileA(&ND.fCSTR_Buffer);
                ND.FileOpenClose();
                ND.NavWindowClose();

            }
            
            ImGui::SetCursorPosX(30);
            if (ND.FileNameOffset.size() < 1) {
                ImGui::Text("Ошибка введите имя или выберите файл!!");
            }
            ImGui::End();
        }
       
            if (ND.FileCreateState) {
                ND.Init();
                ImGui::SameLine();
                if (ImGui::Button("Открыть файл", ImVec2(140, 40))) {
                    ND.FileOpen = true;

                }
                ImGui::SameLine();
                if (ImGui::Button("Конфиг", ImVec2(140, 40))) {
                    ND.MakeConfig = true;
                }
                ImGui::SameLine();
                if (ImGui::Button("Сохранить", ImVec2(140, 40))) {
                    if (ND.FileNameOffset == "") {
                        ND.FileOpen = true;
                    }
                    else {
                        ND.SaveFile(ND.FileNameOffset, ND.fCSTR_Buffer);
                    }
                } ImGui::SameLine();
                if (ImGui::Button("Выйти", ImVec2(140, 40))) {
                    ND.FileCreateState = false;
                    ND.SaveFile(ND.FileNameOffset, ND.fCSTR_Buffer);
                    Sleep(1000);
                    exit(0);
                } ImGui::SameLine();
                if (ImGui::Button("Сборка", ImVec2(140, 40))) {
                    if (!cbRuntimeBatch) {
                        ND.StartCompile(fC_commandStr);
                        ND.MessageWindowInit = true;
                    }
                    if (cbRuntimeBatch) {
                        ND.BatchRT(ND.fCSTR_Buffer);
                    }
                }
                ImGui::PushFont(font3);
                ND.CSetTextColor(ImVec4(0.20f, 1.0f, 0.40f, 1.0f));
                ImGui::SetCursorPosX(16);
                ImGui::InputTextMultiline(".", &ND.fCSTR_Buffer, ImVec2(io.DisplaySize.x - 100, io.DisplaySize.y - 100), ND.fTx_multiLineFlags);
                ND.CSetColorAssept();
                ImGui::PopFont();
                ImGui::PushFont(font3);
                ImGui::Text(("Статус:" + ND.msg).c_str());
                ImGui::PopFont();
            }
        
        ImGui::End();
        //ImGui::End();

    // Rendering
        ImGui::Render();
        glViewport(0, 0, 400, 800);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Present
        ::SwapBuffers(g_MainWindow.hDC);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceWGL(hwnd, &g_MainWindow);
    wglDeleteContext(g_hRC);
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    HDC hDc = ::GetDC(hWnd);
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;

    const int pf = ::ChoosePixelFormat(hDc, &pfd);
    if (pf == 0)
        return false;
    if (::SetPixelFormat(hDc, pf, &pfd) == FALSE)
        return false;
    ::ReleaseDC(hWnd, hDc);

    data->hDC = ::GetDC(hWnd);
    if (!g_hRC)
        g_hRC = wglCreateContext(data->hDC);
    return true;
}

void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    wglMakeCurrent(nullptr, nullptr);
    ::ReleaseDC(hWnd, data->hDC);
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            g_Width = LOWORD(lParam);
            g_Height = HIWORD(lParam);
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        exit(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}