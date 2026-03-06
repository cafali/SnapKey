#include <windows.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

#include "InputState.h"
#include "resource.h"

#pragma comment(lib,"shlwapi.lib")

char g_iniPath[MAX_PATH];

void InitIniPath()
{
    GetModuleFileNameA(NULL,g_iniPath,MAX_PATH);
    PathRemoveFileSpecA(g_iniPath);
    strcat_s(g_iniPath,"\\settings.ini");
}

bool g_enabled = true;

bool g_socd_enabled = true;
bool g_socd_x_enabled = true;
bool g_socd_y_enabled = true;

bool g_mouse_override_enabled = true;
bool g_mouse_override_x_enabled = true;
bool g_mouse_override_y_enabled = true;

bool pW=false,pA=false,pS=false,pD=false;
bool pFire=false;
bool sW=false,sA=false,sS=false,sD=false;
bool lW=false,lA=false,lS=false,lD=false;
bool prevW=false,prevA=false,prevS=false,prevD=false;

char lastH=0;
char lastV=0;

#define WM_TRAYICON (WM_USER + 1)

#define ID_TOGGLE_SOCD 2001
#define ID_TOGGLE_SOCD_X 2002
#define ID_TOGGLE_SOCD_Y 2003
#define ID_TOGGLE_MOUSE 2004
#define ID_TOGGLE_MOUSE_X 2005
#define ID_TOGGLE_MOUSE_Y 2006
#define ID_HELP 2007
#define ID_UPDATE 2008
#define ID_VERSION 2009
#define ID_EDIT_HOTKEYS 2010
#define ID_RESTART 2011
#define ID_EXIT 2012

NOTIFYICONDATA nid = {};
HMENU hTrayMenu;

/* ================= SETTINGS ================= */

void LoadSettings()
{
    g_socd_enabled =
        GetPrivateProfileInt("Settings","socd",1,g_iniPath);

    g_socd_x_enabled =
        GetPrivateProfileInt("Settings","socd_x",1,g_iniPath);

    g_socd_y_enabled =
        GetPrivateProfileInt("Settings","socd_y",1,g_iniPath);

    g_mouse_override_enabled =
        GetPrivateProfileInt("Settings","mouse_override",1,g_iniPath);

    g_mouse_override_x_enabled =
        GetPrivateProfileInt("Settings","mouse_override_x",1,g_iniPath);

    g_mouse_override_y_enabled =
        GetPrivateProfileInt("Settings","mouse_override_y",1,g_iniPath);
}

void SaveSettings()
{
    WritePrivateProfileString("Settings","socd",
        g_socd_enabled?"1":"0",g_iniPath);

    WritePrivateProfileString("Settings","socd_x",
        g_socd_x_enabled?"1":"0",g_iniPath);

    WritePrivateProfileString("Settings","socd_y",
        g_socd_y_enabled?"1":"0",g_iniPath);

    WritePrivateProfileString("Settings","mouse_override",
        g_mouse_override_enabled?"1":"0",g_iniPath);

    WritePrivateProfileString("Settings","mouse_override_x",
        g_mouse_override_x_enabled?"1":"0",g_iniPath);

    WritePrivateProfileString("Settings","mouse_override_y",
        g_mouse_override_y_enabled?"1":"0",g_iniPath);
}

/* ================= HOTKEY SYSTEM ================= */

struct Hotkey
{
    UINT modifiers;
    UINT vk;
};

std::vector<Hotkey> hotkeys;
std::map<int,std::string> actions;

UINT ParseModifier(const std::string& key)
{
    if(key=="Ctrl") return MOD_CONTROL;
    if(key=="Alt") return MOD_ALT;
    if(key=="Shift") return MOD_SHIFT;
    if(key=="Win") return MOD_WIN;
    return 0;
}

UINT ParseKey(const std::string& key)
{
    if(key.length()==1)
        return VkKeyScan(key[0]) & 0xFF;

    if(key[0]=='F')
    {
        int n = atoi(key.substr(1).c_str());
        if(n>=1 && n<=24)
            return VK_F1 + n - 1;
    }

    if(key == "MouseLeft")   return VK_LBUTTON;
    if(key == "MouseRight")  return VK_RBUTTON;
    if(key == "MouseMiddle") return VK_MBUTTON;
    if(key == "MouseX1")     return VK_XBUTTON1;
    if(key == "MouseX2")     return VK_XBUTTON2;

    return 0;
}

Hotkey ParseCombo(std::string combo)
{
    Hotkey hk{};
    hk.modifiers=0;
    hk.vk=0;

    std::stringstream ss(combo);
    std::string part;

    while(getline(ss,part,'+'))
    {
        part.erase(remove_if(part.begin(),part.end(),isspace),part.end());

        UINT mod = ParseModifier(part);

        if(mod)
            hk.modifiers |= mod;
        else
            hk.vk = ParseKey(part);
    }

    return hk;
}

void LoadHotkeys(HWND hwnd)
{
    std::ifstream file("hotkeys.txt");

    if(!file.is_open())
        return;

    std::string line;
    int id=0;

    while(getline(file,line))
    {
        size_t pos=line.find('=');
        if(pos==std::string::npos) continue;

        std::string action=line.substr(0,pos);
        std::string combo=line.substr(pos+1);

        Hotkey hk = ParseCombo(combo);

        hotkeys.push_back(hk);
        actions[id]=action;

        RegisterHotKey(hwnd,6000+id,hk.modifiers,hk.vk);

        id++;
    }
}

void HandleHotkey(int id)
{
    std::string action = actions[id];

    if(action=="toggle_snapkey")
        g_socd_enabled=!g_socd_enabled;

    else if(action=="toggle_socd_x")
        g_socd_x_enabled=!g_socd_x_enabled;

    else if(action=="toggle_socd_y")
        g_socd_y_enabled=!g_socd_y_enabled;

    else if(action=="toggle_mouse_override")
        g_mouse_override_enabled=!g_mouse_override_enabled;

    else if(action=="toggle_mouse_override_x")
        g_mouse_override_x_enabled=!g_mouse_override_x_enabled;

    else if(action=="toggle_mouse_override_y")
        g_mouse_override_y_enabled=!g_mouse_override_y_enabled;

    SaveSettings();
}

/* ================= TRAY MENU ================= */

void UpdateMenuText()
{
    ModifyMenu(hTrayMenu,ID_TOGGLE_SOCD,MF_BYCOMMAND|MF_STRING,ID_TOGGLE_SOCD,
        g_socd_enabled?TEXT("Disable SnapTap"):TEXT("Enable SnapTap"));

    ModifyMenu(hTrayMenu,ID_TOGGLE_SOCD_X,MF_BYCOMMAND|MF_STRING,ID_TOGGLE_SOCD_X,
        g_socd_x_enabled?TEXT("Disable SOCD X (A/D)"):TEXT("Enable SOCD X (A/D)"));

    ModifyMenu(hTrayMenu,ID_TOGGLE_SOCD_Y,MF_BYCOMMAND|MF_STRING,ID_TOGGLE_SOCD_Y,
        g_socd_y_enabled?TEXT("Disable SOCD Y (W/S)"):TEXT("Enable SOCD Y (W/S)"));

    ModifyMenu(hTrayMenu,ID_TOGGLE_MOUSE,MF_BYCOMMAND|MF_STRING,ID_TOGGLE_MOUSE,
        g_mouse_override_enabled?TEXT("Disable Mouse Override"):TEXT("Enable Mouse Override"));

    ModifyMenu(hTrayMenu,ID_TOGGLE_MOUSE_X,MF_BYCOMMAND|MF_STRING,ID_TOGGLE_MOUSE_X,
        g_mouse_override_x_enabled?TEXT("Disable Mouse Override X (A/D)"):TEXT("Enable Mouse Override X (A/D)"));

    ModifyMenu(hTrayMenu,ID_TOGGLE_MOUSE_Y,MF_BYCOMMAND|MF_STRING,ID_TOGGLE_MOUSE_Y,
        g_mouse_override_y_enabled?TEXT("Disable Mouse Override Y (W/S)"):TEXT("Enable Mouse Override Y (W/S)"));
}

/* ================= WINDOW PROC ================= */

LRESULT CALLBACK WindowProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
    switch(msg)
    {

    case WM_HOTKEY:
        HandleHotkey(wParam-6000);
        break;

    case WM_TRAYICON:

        if(lParam==WM_RBUTTONUP)
        {
            POINT pt;
            GetCursorPos(&pt);

            SetForegroundWindow(hwnd);

            UpdateMenuText();

            TrackPopupMenu(hTrayMenu,TPM_RIGHTBUTTON,pt.x,pt.y,0,hwnd,NULL);
        }

        break;

    case WM_COMMAND:

        switch(LOWORD(wParam))
        {

        case ID_TOGGLE_SOCD:
            g_socd_enabled=!g_socd_enabled;
            SaveSettings();
            break;

        case ID_TOGGLE_SOCD_X:
            g_socd_x_enabled=!g_socd_x_enabled;
            SaveSettings();
            break;

        case ID_TOGGLE_SOCD_Y:
            g_socd_y_enabled=!g_socd_y_enabled;
            SaveSettings();
            break;

        case ID_TOGGLE_MOUSE:
            g_mouse_override_enabled=!g_mouse_override_enabled;
            SaveSettings();
            break;

        case ID_TOGGLE_MOUSE_X:
            g_mouse_override_x_enabled=!g_mouse_override_x_enabled;
            SaveSettings();
            break;

        case ID_TOGGLE_MOUSE_Y:
            g_mouse_override_y_enabled=!g_mouse_override_y_enabled;
            SaveSettings();
            break;

        case ID_EDIT_HOTKEYS:
            ShellExecuteA(NULL,
                "open",
                "hotkeys.txt",
                NULL,
                NULL,
                SW_SHOWNORMAL);
            break;

        case ID_RESTART:
        {
            char path[MAX_PATH];
            GetModuleFileNameA(NULL,path,MAX_PATH);

            ShellExecuteA(NULL,"open",path,NULL,NULL,SW_SHOWNORMAL);

            Shell_NotifyIcon(NIM_DELETE,&nid);
            ExitProcess(0);
        }
        break;

        case ID_HELP:
            MessageBox(hwnd,
            TEXT("Email: soma27245@gmail.com"),
            TEXT("Help"),
            MB_OK|MB_ICONINFORMATION);
            break;

        case ID_UPDATE:
            ShellExecute(NULL,"open",
            "https://github.com/SAPNXTDOOR/SnapKey-Pro/releases",
            NULL,NULL,SW_SHOWNORMAL);
            break;

        case ID_VERSION:
            MessageBoxA(hwnd,
                "SnapKey Pro v1.5.1\n"
                "Release Date: 6/3/26\n"
                "Repository: https://github.com/SAPNXTDOOR/SnapKey-Pro\n"
                "\n"
                "This app is made by Saptarshi Mallick.",
                "About SnapKey Pro",
                MB_OK | MB_ICONINFORMATION);
            break;

        case ID_EXIT:
            Shell_NotifyIcon(NIM_DELETE,&nid);
            PostQuitMessage(0);
            break;
        }

        break;

    case WM_DESTROY:

        Shell_NotifyIcon(NIM_DELETE,&nid);
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd,msg,wParam,lParam);
}

/* ================= WINMAIN ================= */

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,int)
{

    WNDCLASS wc={};

    wc.lpfnWndProc=WindowProc;
    wc.hInstance=hInstance;
    wc.lpszClassName=TEXT("SnapKeyProTray");

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        TEXT("SnapKey Pro"),
        WS_OVERLAPPEDWINDOW,
        0,0,0,0,
        NULL,NULL,hInstance,NULL);

    hTrayMenu=CreatePopupMenu();

    AppendMenu(hTrayMenu,MF_STRING,ID_TOGGLE_SOCD,TEXT("Disable SnapTap"));
    AppendMenu(hTrayMenu,MF_STRING,ID_TOGGLE_SOCD_X,TEXT("Disable SOCD X (A/D)"));
    AppendMenu(hTrayMenu,MF_STRING,ID_TOGGLE_SOCD_Y,TEXT("Disable SOCD Y (W/S)"));

    AppendMenu(hTrayMenu,MF_SEPARATOR,0,NULL);

    AppendMenu(hTrayMenu,MF_STRING,ID_TOGGLE_MOUSE,TEXT("Disable Mouse Override"));
    AppendMenu(hTrayMenu,MF_STRING,ID_TOGGLE_MOUSE_X,TEXT("Disable Mouse Override X (A/D)"));
    AppendMenu(hTrayMenu,MF_STRING,ID_TOGGLE_MOUSE_Y,TEXT("Disable Mouse Override Y (W/S)"));

    AppendMenu(hTrayMenu,MF_SEPARATOR,0,NULL);

    AppendMenu(hTrayMenu,MF_STRING,ID_EDIT_HOTKEYS,TEXT("Edit Hotkeys"));
    AppendMenu(hTrayMenu,MF_STRING,ID_RESTART,TEXT("Restart"));

    AppendMenu(hTrayMenu,MF_SEPARATOR,0,NULL);

    AppendMenu(hTrayMenu,MF_STRING,ID_HELP,TEXT("Get Help"));
    AppendMenu(hTrayMenu,MF_STRING,ID_UPDATE,TEXT("Check for Updates"));
    AppendMenu(hTrayMenu,MF_STRING,ID_VERSION,TEXT("About"));

    AppendMenu(hTrayMenu,MF_SEPARATOR,0,NULL);

    AppendMenu(hTrayMenu,MF_STRING,ID_EXIT,TEXT("Exit"));

    nid.cbSize=sizeof(NOTIFYICONDATA);
    nid.hWnd=hwnd;
    nid.uID=1;
    nid.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;
    nid.uCallbackMessage=WM_TRAYICON;
    nid.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_APP_ICON));

    lstrcpy(nid.szTip,TEXT("SnapKey Pro"));

    Shell_NotifyIcon(NIM_ADD,&nid);

    InitIniPath();
    LoadSettings();
    LoadHotkeys(hwnd);

    InstallKeyboardHook();
    InstallMouseHook();

    MSG msg;

    while(GetMessage(&msg,NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}