#include <windows.h>
#include <shellapi.h>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

#define ID_TRAY_APP_ICON                1001
#define ID_TRAY_EXIT_CONTEXT_MENU_ITEM  3000
#define WM_TRAYICON                     (WM_USER + 1)

struct KeyState
{
    bool pressed = false;
};

// Global variables
int keyA_code = 'A'; // Default to 'A'
int keyD_code = 'D'; // Default to 'D'
unordered_map<int, KeyState> keyStates;
int activeKey = 0;
int previousKey = 0;
HHOOK hHook = NULL;
NOTIFYICONDATA nid;

// Function declarations
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void InitNotifyIconData(HWND hwnd);
bool LoadConfig(const std::string& filename);
void CreateDefaultConfig(const std::string& filename); // Declaration
void RestoreConfigFromBackup(const std::string& backupFilename, const std::string& destinationFilename); // Declaration

int main()
{
    // Load key bindings from config file
    if (!LoadConfig("config.cfg")) {
        MessageBox(NULL, TEXT("A default config file has been created."), TEXT("SnapKey Information"), MB_ICONINFORMATION | MB_OK);
    }

    // Create a named mutex
    HANDLE hMutex = CreateMutex(NULL, TRUE, TEXT("SnapKeyMutex"));
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBox(NULL, TEXT("SnapKey is already running!"), TEXT("SnapKey Error"), MB_ICONINFORMATION | MB_OK);
        return 1; // Exit the program
    }

    // Create a window class
    WNDCLASSEX wc = {0};
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

    // Create a window
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

    // Initialize and add the system tray icon
    InitNotifyIconData(hwnd);

    // Set the hook
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (hHook == NULL)
    {
        MessageBox(NULL, TEXT("Failed to install hook!"), TEXT("Error"), MB_ICONEXCLAMATION | MB_OK);
        ReleaseMutex(hMutex); // Release the mutex before exiting
        CloseHandle(hMutex); // Close the handle
        return 1;
    }

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Unhook the hook
    UnhookWindowsHookEx(hHook);

    // Remove the system tray icon
    Shell_NotifyIcon(NIM_DELETE, &nid);

    // Release and close the mutex
    ReleaseMutex(hMutex);
    CloseHandle(hMutex);

    return 0;
}

void handleKeyDown(int keyCode)
{
    if (keyCode == keyA_code || keyCode == keyD_code)
    {
        // get key state
        auto& keyState = keyStates[keyCode];
        if (!keyState.pressed)
        {
            // set key state + check if key is current key
            keyState.pressed = true;
            if (activeKey == 0 || activeKey == keyCode) activeKey = keyCode;
            else 
            {
                // set previous key to current key and active to current
                previousKey = activeKey;
                activeKey = keyCode;

                // send keyup input to previous
                INPUT input = {0};
                input.type = INPUT_KEYBOARD;
                input.ki.wVk = previousKey;
                input.ki.dwFlags = KEYEVENTF_KEYUP;
                SendInput(1, &input, sizeof(INPUT));
            }
        }
    }
}

void handleKeyUp(int keyCode)
{
    if (keyCode == keyA_code || keyCode == keyD_code)
    {
        auto& keyState = keyStates[keyCode];
        // to prevent previous key getting stuck, check if it is the called key as well as if it's (not) pressed
        if (previousKey == keyCode && !keyState.pressed) previousKey = 0;
        if (keyState.pressed) 
        {
            // set key state to released
            keyState.pressed = false;
            // check if previous key exists and the current key is the one we just released
            if (activeKey == keyCode && previousKey != 0)
            {
                // set active to previous and clear previous
                activeKey = previousKey;
                previousKey = 0;

                // send keydown input to active
                INPUT input = {0};
                input.type = INPUT_KEYBOARD;
                input.ki.wVk = activeKey;
                SendInput(1, &input, sizeof(INPUT));
            }
        }
    }
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT *pKeyBoard = (KBDLLHOOKSTRUCT *)lParam;
        switch (wParam)
        {
            case WM_KEYDOWN:
                handleKeyDown(pKeyBoard->vkCode);
                break;
            case WM_KEYUP:
                handleKeyUp(pKeyBoard->vkCode);
                break;
            default:
                break;
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

    // Load the tray icon from the current directory
    HICON hIcon = (HICON)LoadImage(NULL, TEXT("icon.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    if (hIcon)
    {
        nid.hIcon = hIcon;
    }
    else
    {
        // If loading the icon fails, fallback to a default icon
        nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    }

    lstrcpy(nid.szTip, TEXT("SnapKey"));

    Shell_NotifyIcon(NIM_ADD, &nid);
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

            // Create a context menu
            HMENU hMenu = CreatePopupMenu();
            AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU_ITEM, TEXT("Exit SnapKey"));

            // Display the context menu
            TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, curPoint.x, curPoint.y, 0, hwnd, NULL);
            DestroyMenu(hMenu);
        }
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_TRAY_EXIT_CONTEXT_MENU_ITEM)
        {
            PostQuitMessage(0);
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

// Function to copy the backup file from the meta folder to the main directory
void RestoreConfigFromBackup(const std::string& backupFilename, const std::string& destinationFilename)
{
    std::string sourcePath = "meta\\" + backupFilename;
    std::string destinationPath = destinationFilename;

    if (CopyFile(sourcePath.c_str(), destinationPath.c_str(), FALSE)) {
        // Copy successful
        MessageBox(NULL, TEXT("Config restored from backup successfully."), TEXT("SnapKey Information"), MB_ICONINFORMATION | MB_OK);
    } else {
        // backup.snapkey copy failed
        DWORD error = GetLastError();
        std::string errorMsg = "Failed to restore config from backup. Error code: " + std::to_string(error);
        MessageBox(NULL, errorMsg.c_str(), TEXT("SnapKey Error"), MB_ICONERROR | MB_OK);
    }
}

// Restore config.cfg from backup.snapkey
void CreateDefaultConfig(const std::string& filename)
{
    std::string backupFilename = "backup.snapkey";
    RestoreConfigFromBackup(backupFilename, filename);
}

// Check for config.cfg
bool LoadConfig(const std::string& filename)
{
    std::ifstream configFile(filename);
    if (!configFile.is_open()) {
        CreateDefaultConfig(filename);  // Restore config from backup if file doesn't exist
        return false;
    }

    std::string line;
    while (std::getline(configFile, line)) {
        std::istringstream iss(line);
        std::string key;
        int value;
        if (std::getline(iss, key, '=') && (iss >> value)) {
            if (key == "keyA") {
                keyA_code = value;
            } else if (key == "keyD") {
                keyD_code = value;
            }
        }
    }

    return true;
}
