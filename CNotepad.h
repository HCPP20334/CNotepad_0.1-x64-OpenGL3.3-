

#define CRT_NO_WARNINGS 1
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <GL/GL.h>
#include <tchar.h>
#include <string>
//#include "JoyStick.h"
#include <iostream>
#include <conio.h> 
#include <fstream>
#include <string>
#include <urlmon.h>
#include <clocale>
#include <thread>
#include "imgui_stdlib.h"
#include "dSys.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <wtypes.h>
#include <commdlg.h>
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height, unsigned char* imgBuffer) {
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    *imgBuffer = *image_data;
    if (image_data == NULL)
        return false;

    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}//
uint64_t dx = 0;
uint64_t dy = 0;
class CImage {
public:
    int fCImageCtx = 0;
    int fCImageCty = 0;
    GLuint fCImageGL;
    bool CreateImage(int64_t w, int64_t h);
    bool CImageOpenSt;
    uint8_t CImageBuffer;
    uint64_t InitCImage(std::string png_file);
    uint64_t displaySizeA(uint64_t* wx, uint64_t* hy);
};
uint64_t CImage::displaySizeA(uint64_t *wx, uint64_t *hy)
{
    RECT desktop;
    // Get a handle to the desktop window
    const HWND hDesktop = GetDesktopWindow();
    // Get the size of screen to the variable desktop
    GetWindowRect(hDesktop, &desktop);
    // The top left corner will have coordinates (0,0)
    // and the bottom right corner will have coordinates
    // (horizontal, vertical)
    *wx = desktop.right;
    *hy = desktop.bottom;
    return 1;
}

bool CImage::CreateImage(int64_t w,int64_t h) {
    ImGui::Image((void*)(intptr_t)CImage::fCImageGL, ImVec2(w, h));
    return true;
}
uint64_t CImage::InitCImage(std::string png_file) {
    CImage::CImageOpenSt = LoadTextureFromFile((png_file).c_str(), &fCImageGL, &fCImageCtx, &fCImageCty, &CImageBuffer);
    return true;
}
void openFileDialog(std::string& fileName) {
    // common dialog box structure, setting all fields to 0 is important
    OPENFILENAMEA ofn;
    char szFileName[MAX_PATH];

    ZeroMemory(&ofn, sizeof(ofn));
    szFileName[0] = 0;

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = 520;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    GetOpenFileNameA(&ofn);
    fileName = std::string((char*)ofn.lpstrFile);
}
int64_t fileCounts = 0;
static bool cbRuntimeBatch;
CImage CImageC;
std::string fC_commandStr;
class CNotepad {
    public:
        bool fCNotepadWindow = true;
        uint64_t fW_noTitle = ImGuiWindowFlags_NoTitleBar;
        uint64_t fW_noMove = ImGuiWindowFlags_NoMove;
        uint64_t fW_noColiapse = ImGuiWindowFlags_NoCollapse;
        uint64_t fW_noResize = ImGuiWindowFlags_NoResize;
        //
        std::string FileNameOffset;
        std::string fCSTR_Buffer;
        //
        bool FileOpen = false;
        bool MakeConfig = false;
        bool About = false;
        bool fNavButtons = true;
        bool MessageWindowInit = false;
        //
        bool NButton(const char* text, bool* state);
        int64_t NInput(std::string text, std::string* out);
        int64_t Message_log(std::string text, std::string title);
        bool Init();
        bool FileInit(bool* bFileInit);
        bool FileOpenClose();
        bool FileCreateState = false;
        bool NavWindowClose();
        bool SaveFile(std::string fileName, std::string text);
        bool fileIsOpen;
        std::string fCsrText;
        bool OpenFileN( std::string* outdata);
        bool OpenFileA(std::string* outdata);
        int64_t fp_lineCount = 0;
        int64_t cLine = 0;
        std::string msg;
        bool fNBufferLine = true;
        bool fBvsync = true;
        bool ConfigEdit(std::string sCfgItem);
        bool ConfigParse(std::string sCfgItem, bool* b_enable, bool b_st);
        uint64_t CSetTextColor(const ImVec4& col);
        uint64_t CSetColorAssept();
        bool StartCompile(std::string command);
        uint64_t SwapBuffer(std::string* buf0);
        std::string commandCompiler;
        bool BatchRT(std::string TextBuffer);
        uint64_t dsx(int64_t* x, int64_t* y, HWND hwnd);
        uint64_t fTx_multiLineFlags = 1 << 26;
        uint64_t bufferSize = 0;
        bool WindowFullScreen = false;
        uint64_t Clog(std::string text);
           
};
uint64_t CNotepad::Clog(std::string text) {
    CNotepad::msg = text;
    return 0;
}
bool CNotepad::NButton(const char* text, bool* state) {
    if (ImGui::Button(text)) {
        *state = true;
        return true;
    }
    else {
        *state = false;
        return false;
    }

}
bool CNotepad::BatchRT(std::string TextBuffer) {
    std::system((TextBuffer).c_str());
}
uint64_t CNotepad::SwapBuffer(std::string* buf0) {
    *buf0 = commandCompiler;
    return commandCompiler.size();
}

bool CNotepad::SaveFile(std::string fileName, std::string text) {
    std::ofstream fileHandle((fileName).c_str());
    CNotepad::fileIsOpen = fileHandle.is_open();
    if (CNotepad::fileIsOpen) {
        
    }
    fileHandle << text << std::endl;
    fileHandle.close();
    CNotepad::Clog("Файл:" + CNotepad::FileNameOffset + ": сохранен успешно!");
}
bool CNotepad::NavWindowClose() {
    CNotepad::fNavButtons = false;
    return true;
}
bool CNotepad::FileOpenClose() {
    CNotepad::FileOpen = false;
        return true;
}
bool CNotepad::OpenFileN(std::string* outdata) {
    *outdata = "";
    openFileDialog(FileNameOffset);
    std::ifstream fileRead2((CNotepad::FileNameOffset).c_str());
    fileRead2.is_open();
    CNotepad::Clog("Файл:" + CNotepad::FileNameOffset + ": открыт");
    while (std::getline(fileRead2, fCsrText)) {
        CNotepad::bufferSize = CNotepad::fCsrText.size();
        std::cout << CNotepad::fp_lineCount << std::endl;
        CNotepad::fp_lineCount++;
        if (CNotepad::fp_lineCount > 5) {
            CNotepad::fp_lineCount = 0;
        }
        if (CNotepad::fNBufferLine) {
            if (CNotepad::bufferSize += 37) {
                CNotepad::cLine++;
                std::cout << CNotepad::cLine << std::endl;
                CNotepad::fCsrText = CNotepad::fCsrText + "\n";
            }
        }
        *outdata += CNotepad::fCsrText;
    }
    fileRead2.close();
    return fileRead2.is_open();
}
uint64_t CNotepad::dsx(int64_t* x, int64_t* y,HWND hwnd) {

    RECT desktop;
    const HWND hDesktop = hwnd;
    GetWindowRect(hDesktop, &desktop);
    *x = desktop.right;
    *y = desktop.bottom;
    return 1;
}
uint64_t CNotepad::CSetTextColor(const ImVec4& col) {
    ImGui::PushStyleColor(ImGuiCol_Text, col);
    return 1;
}
uint64_t CNotepad::CSetColorAssept() {
    ImGui::PopStyleColor();
    return 1;
}
bool CNotepad::StartCompile(std::string command) {
    std::system((command+" "+ FileNameOffset).c_str());
    return 0;
}
bool CNotepad::OpenFileA(std::string* outdata) {
    *outdata = "";
    std::ifstream fileRead2((CNotepad::FileNameOffset).c_str());
    CNotepad::Clog("Файл:" + CNotepad::FileNameOffset + ": открыт");
    if (!fileRead2.is_open()) {
        CNotepad::MessageWindowInit = !fileRead2.is_open();
        if (CNotepad::MessageWindowInit) {
            CNotepad::Clog("Fatal Error!! Файл не открыт!!");
        }
    }
    while (std::getline(fileRead2, fCsrText)) {
        CNotepad::bufferSize = CNotepad::fCsrText.size();
        std::cout << CNotepad::fp_lineCount << std::endl;
        CNotepad::fp_lineCount++;
        if (CNotepad::fp_lineCount > 5) {
            CNotepad::fp_lineCount = 0;
        }
        if (CNotepad::fNBufferLine) {
            if (CNotepad::bufferSize += 37) {
                CNotepad::cLine++;
                std::cout << CNotepad::cLine << std::endl;
                CNotepad::fCsrText = CNotepad::fCsrText + "\n";
            }
        }
        *outdata += CNotepad::fCsrText;
    }
    fileRead2.close();
    return fileRead2.is_open();
}
bool CNotepad::ConfigEdit(std::string sCfgItem) {
    std::ofstream cncfg("CNotepad.cfg");
    if (cncfg.is_open()) {

    }
    cncfg << sCfgItem << std::endl;
    return true;
}
bool CNotepad::ConfigParse(std::string sCfgItem, bool *b_enable,bool b_st) {
    std::string m_str;
    std::ifstream cncfg2("CNotepad.cfg");
    if (cncfg2.is_open()) {
        while (std::getline(cncfg2, m_str)) {
            if (m_str == sCfgItem) {
                *b_enable = b_st;
            }
        }
    }
    return b_st;
}
bool CNotepad::FileInit(bool *bFileInit) {
    *bFileInit = true;
    return true;
}
int64_t CNotepad::NInput(std::string text, std::string* out) {
   // ImGui::InputText(".",*out);
}
int64_t CNotepad::Message_log(std::string text, std::string title) {
    if (MessageWindowInit) {
        ImGui::Begin((title).c_str(), &MessageWindowInit);
        ImGui::Text((text).c_str());
        if (ImGui::Button("OK")) {
            MessageWindowInit = false;
        }
        ImGui::End();
    }
}
bool CNotepad::Init() {
    ShowWindow(GetConsoleWindow(), 0);
    return 1;
}
CNotepad ND;


