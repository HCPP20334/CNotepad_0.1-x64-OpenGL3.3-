


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
class CImage {
public:
    int fCImageCtx = 0;
    int fCImageCty = 0;
    GLuint fCImageGL;
    bool CreateImage(int64_t w, int64_t h);
    bool CImageOpenSt;
    uint8_t CImageBuffer;
    uint64_t InitCImage(std::string png_file);
};
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
CImage CImageC;
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
        bool MessageWindowInit = true;
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
        bool fNBufferLine = true;
        bool fBvsync = false;
        bool ConfigEdit(std::string sCfgItem);
        bool ConfigParse(std::string sCfgItem, bool* b_enable, bool b_st);
};
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
bool CNotepad::SaveFile(std::string fileName, std::string text) {
    std::ofstream fileHandle((fileName).c_str());
    CNotepad::fileIsOpen = fileHandle.is_open();
    if (CNotepad::fileIsOpen) {
        
    }
    fileHandle << text << std::endl;
    fileHandle.close();
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
    std::ifstream fileRead((CNotepad::FileNameOffset).c_str());
    fileRead.is_open();
    while (std::getline(fileRead, fCsrText)) {
        std::cout << CNotepad::fp_lineCount << std::endl;
        CNotepad::fp_lineCount++;
        if (CNotepad::fp_lineCount > 5) {
            CNotepad::fp_lineCount = 0;
        }
        if (CNotepad::fNBufferLine) {
            if (fp_lineCount == 5) {
                CNotepad::cLine++;
                std::cout << CNotepad::cLine << std::endl;
                CNotepad::fCsrText = CNotepad::fCsrText + "\n";
            }
        }
        *outdata += CNotepad::fCsrText;
    }
    fileRead.close();
    return fileRead.is_open();
}
bool CNotepad::OpenFileA(std::string* outdata) {
    *outdata = "";
    std::ifstream fileRead2((CNotepad::FileNameOffset).c_str());
    fileRead2.is_open();
    while (std::getline(fileRead2, fCsrText)) {
        std::cout << CNotepad::fp_lineCount << std::endl;
        CNotepad::fp_lineCount++;
        if (CNotepad::fp_lineCount > 5) {
            CNotepad::fp_lineCount = 0;
        }
        if (CNotepad::fNBufferLine) {
            if (fp_lineCount == 5) {
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
    ImGui::Begin((title).c_str(), &MessageWindowInit);
    ImGui::Text((text).c_str());   
    if (ImGui::Button("OK")) {
       MessageWindowInit = false;
    }
    ImGui::End();
}
bool CNotepad::Init() {

}
CNotepad ND;


