// SnapKey 1.3.0 
// github.com/cafali/SnapKey
// Forked by Fasshn 

#include <windows.h>
#include <shellapi.h>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <regex>
#include <thread>
#include <vector>
#include <filesystem>
#include <iostream>
#include <gui.h>
#include "ALL-LOGS/Logs.h"
using namespace std;
namespace fs = std::filesystem;

#define ID_TRAY_APP_ICON                1001
#define ID_TRAY_EXIT_CONTEXT_MENU_ITEM  3000
#define ID_TRAY_VERSION_INFO            3001
#define ID_TRAY_OPEN_GUI                3010 
#define ID_TRAY_REBIND_KEYS             3002
#define ID_TRAY_LOCK_FUNCTION           3003
#define ID_TRAY_RESTART_SNAPKEY         3004
#define ID_TRAY_HELP                    3005
#define ID_TRAY_CHECKUPDATE             3006
#define ID_TRAY_LAYOUTS                 3007
#define WM_TRAYICON                     (WM_USER + 1)
#define ID_LAYOUT_BASE                  4000 

struct KeyState
{
    bool registered = false;
    bool keyDown = false;
    int group = 0;
    bool simulated = false;
};

struct GroupState
{
    int previousKey = 0;
    int activeKey = 0;
};

unordered_map<int, GroupState> GroupInfo;
unordered_map<int, KeyState> KeyInfo;

HHOOK hHook = NULL;
HANDLE hMutex = NULL;
NOTIFYICONDATA nid;
bool isLocked = false; 
bool GuiRunning = false;

char Key1 = 0;
char Key2 = 0;
char Key3 = 0;
char Key4 = 0;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void InitNotifyIconData(HWND hwnd);
bool LoadConfig(const std::string& filename);
void CreateDefaultConfig(const std::string& filename);
void RestoreConfigFromBackup(const std::string& backupFilename, const std::string& destinationFilename);
std::string GetVersionInfo();
void SendKey(int target, bool keyDown);
vector<string> ListLayouts();
void ApplyLayout(const string& layoutName);
void RestartSnapKey();
bool RunUI();

int main()
{
    std::thread Gui(RunUI);
    Gui.detach();

    if (!LoadConfig("config.cfg")) {
        return 1;
    }

    hMutex = CreateMutex(NULL, TRUE, TEXT("SnapKeyMutex"));
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBox(NULL, TEXT("SnapKey is already running!"), TEXT("SnapKey"), MB_ICONINFORMATION | MB_OK);
        return 1; 
    }

    // Create a window class
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = TEXT("SnapKeyClass");

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error"), MB_ICONEXCLAMATION | MB_OK);
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
        return 1;
    }

    HWND hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        TEXT("SnapKey"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, wc.hInstance, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, TEXT("Window Creation Failed!"), TEXT("Error"), MB_ICONEXCLAMATION | MB_OK);
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
        return 1;
    }

    InitNotifyIconData(hwnd);

    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (hHook == NULL)
    {
        MessageBox(NULL, TEXT("Failed to install hook!"), TEXT("Error"), MB_ICONEXCLAMATION | MB_OK);
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hHook);

    Shell_NotifyIcon(NIM_DELETE, &nid);

    ReleaseMutex(hMutex);
    CloseHandle(hMutex);

    return 0;
}

void handleKeyDown(int keyCode)
{
    KeyState& currentKeyInfo = KeyInfo[keyCode];
    GroupState& currentGroupInfo = GroupInfo[currentKeyInfo.group];
    if (!currentKeyInfo.keyDown)
    {
        currentKeyInfo.keyDown = true;
        SendKey(keyCode, true);
        if (currentGroupInfo.activeKey == 0 || currentGroupInfo.activeKey == keyCode)
        {
            currentGroupInfo.activeKey = keyCode;
        }
        else
        {
            currentGroupInfo.previousKey = currentGroupInfo.activeKey;
            currentGroupInfo.activeKey = keyCode;

            SendKey(currentGroupInfo.previousKey, false);
        }
    }
}

void handleKeyUp(int keyCode)
{
    KeyState& currentKeyInfo = KeyInfo[keyCode];
    GroupState& currentGroupInfo = GroupInfo[currentKeyInfo.group];
    if (currentGroupInfo.previousKey == keyCode && !currentKeyInfo.keyDown)
    {
        currentGroupInfo.previousKey = 0;
    }
    if (currentKeyInfo.keyDown)
    {
        currentKeyInfo.keyDown = false;
        if (currentGroupInfo.activeKey == keyCode && currentGroupInfo.previousKey != 0)
        {
            SendKey(keyCode, false);

            currentGroupInfo.activeKey = currentGroupInfo.previousKey;
            currentGroupInfo.previousKey = 0;

            SendKey(currentGroupInfo.activeKey, true);
        }
        else
        {
            currentGroupInfo.previousKey = 0;
            if (currentGroupInfo.activeKey == keyCode) currentGroupInfo.activeKey = 0;
            SendKey(keyCode, false);
        }
    }
}

bool isSimulatedKeyEvent(DWORD flags) {
    return flags & 0x10;
}

void SendKey(int targetKey, bool keyDown)
{
    INPUT input = { 0 };
    input.ki.wVk = targetKey;
    input.ki.wScan = MapVirtualKey(targetKey, 0);
    input.type = INPUT_KEYBOARD;

    DWORD flags = KEYEVENTF_SCANCODE;
    input.ki.dwFlags = keyDown ? flags : flags | KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (!isLocked && nCode >= 0)
    {
        KBDLLHOOKSTRUCT* pKeyBoard = (KBDLLHOOKSTRUCT*)lParam;
        if (!isSimulatedKeyEvent(pKeyBoard->flags)) {
            if (KeyInfo[pKeyBoard->vkCode].registered)
            {
                if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) handleKeyDown(pKeyBoard->vkCode);
                if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) handleKeyUp(pKeyBoard->vkCode);
                return 1;
            }
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

void InitNotifyIconData(HWND hwnd)
{
    memset(&nid, 0, sizeof(NOTIFYICONDATA));

    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = ID_TRAY_APP_ICON;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;

    HICON hIcon = (HICON)LoadImage(NULL, TEXT("icon.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    if (hIcon)
    {
        nid.hIcon = hIcon;
    }
    else
    {
        nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    }

    lstrcpy(nid.szTip, TEXT("SnapKey"));

    Shell_NotifyIcon(NIM_ADD, &nid);
}

vector<string> ListLayouts() {
    vector<string> layouts;
    string path = "meta\\profiles";
    if (!fs::exists(path)) return layouts;

    for (auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            auto ext = entry.path().extension().string();
            if (ext == ".cfg") {
                layouts.push_back(entry.path().stem().string());
            }
        }
    }
    return layouts;
}

void ApplyLayout(const string& layoutName) {
    string sourcePath = "meta\\profiles\\" + layoutName + ".cfg";
    string destPath = "config.cfg";

    ifstream src(sourcePath, ios::binary);
    ofstream dst(destPath, ios::binary | ios::trunc);

    if (!src.is_open() || !dst.is_open()) {
        MessageBox(NULL, TEXT("Failed to apply layout. Please check the layout file."), TEXT("SnapKey Error"), MB_ICONERROR | MB_OK);
        return;
    }

    dst << src.rdbuf();
}

void RestartSnapKey() {
    TCHAR szExeFileName[MAX_PATH];
    GetModuleFileName(NULL, szExeFileName, MAX_PATH);
    ShellExecute(NULL, NULL, szExeFileName, NULL, NULL, SW_SHOWNORMAL);
    PostQuitMessage(0);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_TRAYICON:
        if (lParam == WM_RBUTTONDOWN)
        {
            POINT curPoint;
            GetCursorPos(&curPoint);
            SetForegroundWindow(hwnd);

            HMENU hMenu = CreatePopupMenu();

            AppendMenu(hMenu, MF_STRING, ID_TRAY_OPEN_GUI, GuiRunning ? TEXT("Close GUI") : TEXT("Open GUI"));

            AppendMenu(hMenu, MF_STRING, ID_TRAY_REBIND_KEYS, TEXT("Rebind Keys"));

            HMENU hSubMenu = CreatePopupMenu();
            vector<string> layouts = ListLayouts();
            if (!layouts.empty()) {
                int id = 0;
                for (auto& layout : layouts) {
                    AppendMenuA(hSubMenu, MF_STRING, ID_LAYOUT_BASE + id, layout.c_str());
                    id++;
                }
            }
            else {
                AppendMenu(hSubMenu, MF_GRAYED, 0, TEXT("No layouts found"));
            }
            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, TEXT("Select Profile"));

            AppendMenu(hMenu, MF_STRING, ID_TRAY_RESTART_SNAPKEY, TEXT("Restart SnapKey"));
            AppendMenu(hMenu, MF_STRING, ID_TRAY_LOCK_FUNCTION, isLocked ? TEXT("Enable SnapKey") : TEXT("Disable SnapKey"));
            AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
            AppendMenu(hMenu, MF_STRING, ID_TRAY_HELP, TEXT("Get Help"));
            AppendMenu(hMenu, MF_STRING, ID_TRAY_CHECKUPDATE, TEXT("Check Updates"));
            AppendMenu(hMenu, MF_STRING, ID_TRAY_VERSION_INFO, TEXT("Version Info (1.2.9)"));
            AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
             
            AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU_ITEM, TEXT("Exit SnapKey"));

            TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, curPoint.x, curPoint.y, 0, hwnd, NULL);
            DestroyMenu(hMenu);
        }
        else if (lParam == WM_LBUTTONDBLCLK) 
        {
            isLocked = !isLocked;

            if (isLocked)
            {
                HICON hIconOff = (HICON)LoadImage(NULL, TEXT("icon_off.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
                if (hIconOff)
                {
                    nid.hIcon = hIconOff;
                    Shell_NotifyIcon(NIM_MODIFY, &nid);
                    DestroyIcon(hIconOff);
                }
            }
            else
            {
                HICON hIconOn = (HICON)LoadImage(NULL, TEXT("icon.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
                if (hIconOn)
                {
                    nid.hIcon = hIconOn;
                    Shell_NotifyIcon(NIM_MODIFY, &nid);
                    DestroyIcon(hIconOn);
                }
            }
        }
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) >= ID_LAYOUT_BASE) {
            int layoutIndex = LOWORD(wParam) - ID_LAYOUT_BASE;
            vector<string> layouts = ListLayouts();
            if (layoutIndex >= 0 && layoutIndex < (int)layouts.size()) {
                ApplyLayout(layouts[layoutIndex]);
                RestartSnapKey(); 
            }
        }
        else {
            switch (LOWORD(wParam))
            {
            case ID_TRAY_EXIT_CONTEXT_MENU_ITEM: 
                PostQuitMessage(0);
                break;
            case ID_TRAY_VERSION_INFO: 
            {
                std::string versionInfo = GetVersionInfo();
                MessageBox(hwnd, versionInfo.c_str(), TEXT("SnapKey Version Info"), MB_OK);
            }
            break;
            case ID_TRAY_OPEN_GUI:
            {
              
                GuiRunning = !GuiRunning;
                break;
            }
            case ID_TRAY_REBIND_KEYS: 
            {
                ShellExecute(NULL, TEXT("open"), TEXT("config.cfg"), NULL, NULL, SW_SHOWNORMAL);
                break;
            }
            case ID_TRAY_HELP:
            {
                ShellExecute(NULL, TEXT("open"), TEXT("README.pdf"), NULL, NULL, SW_SHOWNORMAL);
                break;
            }
            case ID_TRAY_CHECKUPDATE: 
            {
                int result = MessageBox(NULL,
                    TEXT("You are about to visit the SnapKey update page (github.com). Continue?"),
                    TEXT("Update SnapKey"),
                    MB_YESNO | MB_ICONQUESTION);

                if (result == IDYES) {
                    ShellExecute(NULL, TEXT("open"), TEXT("https://github.com/cafali/SnapKey/releases"), NULL, NULL, SW_SHOWNORMAL);
                }
                break;
            }
            case ID_TRAY_RESTART_SNAPKEY:
            {
                RestartSnapKey();
                break;
            }
            case ID_TRAY_LOCK_FUNCTION:
            {
                isLocked = !isLocked;
                HICON hIcon = isLocked
                    ? (HICON)LoadImage(NULL, TEXT("icon_off.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE)
                    : (HICON)LoadImage(NULL, TEXT("icon.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
                if (hIcon)
                {
                    nid.hIcon = hIcon;
                    Shell_NotifyIcon(NIM_MODIFY, &nid);
                    DestroyIcon(hIcon);
                }
                break;
            }
            }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

std::string GetVersionInfo() {
    return "SnapKey v1.2.9 (R18)\n"
        "Version Date: August 8, 2025\n"
        "Repository: github.com/cafali/SnapKey\n"
        "License: MIT License\n";
}

void RestoreConfigFromBackup(const std::string& backupFilename, const std::string& destinationFilename)
{
    std::string sourcePath = "meta\\" + backupFilename;
    std::string destinationPath = destinationFilename;

    if (CopyFile(sourcePath.c_str(), destinationPath.c_str(), FALSE)) {
        MessageBox(NULL, TEXT("Default config restored from backup successfully."), TEXT("SnapKey"), MB_ICONINFORMATION | MB_OK);
    }
    else {
        DWORD error = GetLastError();
        std::string errorMsg = "Failed to restore config from backup.";
        MessageBox(NULL, errorMsg.c_str(), TEXT("SnapKey Error"), MB_ICONERROR | MB_OK);
    }
}

void CreateDefaultConfig(const std::string& filename)
{
    std::string backupFilename = "backup.snapkey";
    RestoreConfigFromBackup(backupFilename, filename);
}

bool LoadConfig(const std::string& filename)
{
    std::ifstream configFile(filename);
    if (!configFile.is_open()) {
        WriteLog("Failed to open config file");
        CreateDefaultConfig(filename);
        return false;
    }

    string line;
    int id = 0;                
    int CurrentKey = 1;        
    regex secPat(R"(\s*\[Group\]\s*)");

    while (getline(configFile, line)) {
        istringstream iss(line);
        string key;
        int value;

        if (regex_match(line, secPat))
        {
            id++;
            if (GroupInfo.find(id) == GroupInfo.end()) {
                GroupInfo[id] = GroupState();
            }
            continue;
        }

        if (getline(iss, key, '=') && (iss >> value))
        {

            if (key.find("key") != string::npos)
            {
                if (id == 0) {
                    id = 1;
                    if (GroupInfo.find(id) == GroupInfo.end()) GroupInfo[id] = GroupState();
                }

                if (!KeyInfo[value].registered)
                {
                    KeyInfo[value].registered = true;
                    KeyInfo[value].group = id;

                    if (GroupInfo.find(id) == GroupInfo.end()) {
                        GroupInfo[id] = GroupState();
                    }

                    switch (CurrentKey)
                    {
                    case 1:
                        Key1 = static_cast<char>(value);
                        break;
                    case 2:
                        Key2 = static_cast<char>(value);
                        break;
                    case 3:
                        Key3 = static_cast<char>(value);
                        break;
                    default:
                        Key4 = static_cast<char>(value);
                        break;
                    }
                    CurrentKey++;
                }
                else
                {
                    MessageBox(NULL, TEXT("The config file contains duplicate keys across various groups. Please review and correct the setup."), TEXT("SnapKey Error"), MB_ICONEXCLAMATION | MB_OK);
                    MessageBox(NULL, TEXT("For more information, please view the README file or visit github.com/cafali/SnapKey/wiki."), TEXT("SnapKey Error"), MB_ICONINFORMATION | MB_OK);
                    return false;
                }
            }
        }
    }

    for (auto& [gid, gs] : GroupInfo) {
    }

    WriteLog("Config successfully loaded");
    return true;
}

bool RunUI()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if (GuiRunning)
        {
            gui::CreateHWindow("Snapkey");
            gui::CreateDevice();
            gui::CreateImGui();

            while (gui::isRunning && GuiRunning)
            {
                gui::BeginRender();
                gui::Render();
                gui::EndRender();
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }

            gui::DestroyImGui();
            gui::DestroyDevice();
            gui::DestroyHWindow();

            GuiRunning = false;
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    return true;
}
