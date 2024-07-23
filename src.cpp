#include <windows.h>
#include <shellapi.h>
#include <fstream>
#include <string>

// ID definitions
#define ID_TRAY_APP_ICON 1001
#define ID_TRAY_EXIT_CONTEXT_MENU_ITEM 3000
#define ID_TRAY_TOGGLE_DEBUG 3001
#define WM_TRAYICON (WM_USER + 1)

// Global variables
bool keyA_pressed = false;
bool keyD_pressed = false;
bool keyA_was_first = false;
bool keyD_was_first = false;
bool keyA_released_by_program = false;
bool keyD_released_by_program = false;
bool debugMode = false;
HHOOK hHook = NULL;
NOTIFYICONDATA nid;
std::ofstream logFile("debug.log");
char keyA = 'A'; // Default value
char keyD = 'D'; // Default value

// Function to log debug messages with variable states
void DebugLog(const char *message)
{
    if (debugMode)
    {
        logFile << message;
        logFile.flush(); // Ensures messages are written immediately
    }
}

// Function to log key states
void LogKeyStates(const char *action)
{
    if (debugMode)
    {
        logFile << action
                << " keyA_pressed=" << keyA_pressed
                << " keyD_pressed=" << keyD_pressed
                << " keyA_was_first=" << keyA_was_first
                << " keyD_was_first=" << keyD_was_first
                << " keyA_released_by_program=" << keyA_released_by_program
                << " keyD_released_by_program=" << keyD_released_by_program
                << "\n";
        logFile.flush();
    }
}

// Function declarations
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void InitNotifyIconData(HWND hwnd);
void LoadKeyBindings();

int main()
{
    DebugLog("Program started\n");

    // Create a named mutex
    HANDLE hMutex = CreateMutex(NULL, TRUE, TEXT("SnapKeyMutex"));
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        DebugLog("SnapKey is already running!\n");
        MessageBox(NULL, TEXT("SnapKey is already running!"), TEXT("Error"), MB_ICONINFORMATION | MB_OK);
        return 1; // Exit the program
    }

    DebugLog("Mutex created\n");

    // Load key bindings
    LoadKeyBindings();

    // Create a window class
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = TEXT("SnapKeyClass");

    if (!RegisterClassEx(&wc))
    {
        DebugLog("Window Registration Failed!\n");
        MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error"), MB_ICONEXCLAMATION | MB_OK);
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
        return 1;
    }

    DebugLog("Window class registered\n");

    // Create a window
    HWND hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        TEXT("SnapKey"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, wc.hInstance, NULL);

    if (hwnd == NULL)
    {
        DebugLog("Window Creation Failed!\n");
        MessageBox(NULL, TEXT("Window Creation Failed!"), TEXT("Error"), MB_ICONEXCLAMATION | MB_OK);
        ReleaseMutex(hMutex); // Release the mutex before exiting
        CloseHandle(hMutex);  // Close the handle
        return 1;
    }

    DebugLog("Window created\n");

    // Initialize and add the tray icon
    InitNotifyIconData(hwnd);

    // Set the hook
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (hHook == NULL)
    {
        DebugLog("Failed to install hook!\n");
        MessageBox(NULL, TEXT("Failed to install hook!"), TEXT("Error"), MB_ICONEXCLAMATION | MB_OK);
        ReleaseMutex(hMutex); // Release the mutex before exiting
        CloseHandle(hMutex);  // Close the handle
        return 1;
    }

    DebugLog("Keyboard hook installed\n");

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DebugLog("Message loop exited\n");

    // Uninstall the hook
    UnhookWindowsHookEx(hHook);

    // Delete the tray icon
    Shell_NotifyIcon(NIM_DELETE, &nid);

    // Release and close the mutex
    ReleaseMutex(hMutex);
    CloseHandle(hMutex);

    DebugLog("Program ended\n");

    // Close log file
    logFile.close();

    return 0;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT *pKeyBoard = (KBDLLHOOKSTRUCT *)lParam;
        switch (wParam)
        {
        case WM_KEYDOWN:
            if (pKeyBoard->vkCode == keyA)
            {
                if (!keyA_pressed)
                {
                    keyA_pressed = true;
                    keyA_was_first = !keyD_pressed;
                    DebugLog("Key A pressed\n");
                    if (keyD_pressed)
                    {
                        // Release D if A is pressed and D was previously pressed
                        keyD_released_by_program = true;
                        keyD_pressed = false;
                        INPUT input = {0};
                        input.type = INPUT_KEYBOARD;
                        input.ki.wVk = keyD;
                        input.ki.dwFlags = KEYEVENTF_KEYUP;
                        SendInput(1, &input, sizeof(INPUT));
                        LogKeyStates("Key D released by program\n");
                    }
                }
            }
            else if (pKeyBoard->vkCode == keyD)
            {
                if (!keyD_pressed)
                {
                    keyD_pressed = true;
                    keyD_was_first = !keyA_pressed;
                    DebugLog("Key D pressed\n");
                    if (keyA_pressed)
                    {
                        // Release A if D is pressed and A was previously pressed
                        keyA_released_by_program = true;
                        keyA_pressed = false;
                        INPUT input = {0};
                        input.type = INPUT_KEYBOARD;
                        input.ki.wVk = keyA;
                        input.ki.dwFlags = KEYEVENTF_KEYUP;
                        SendInput(1, &input, sizeof(INPUT));
                        LogKeyStates("Key A released by program\n");
                    }
                }
            }
            break;

        case WM_KEYUP:
            if (pKeyBoard->vkCode == keyA)
            {
                if (keyA_pressed)
                {
                    keyA_pressed = false;
                    keyA_released_by_program = false;
                    LogKeyStates("Key A released by user\n");

                    if (keyD_released_by_program && !keyD_pressed && keyD_was_first)
                    {
                        // Reactivate D if it was the first key and A is still pressed
                        INPUT input = {0};
                        input.type = INPUT_KEYBOARD;
                        input.ki.wVk = keyD;
                        input.ki.dwFlags = 0;
                        SendInput(1, &input, sizeof(INPUT));
                        keyD_pressed = true;
                        DebugLog("Key D reactivated\n");
                    }
                }
            }
            else if (pKeyBoard->vkCode == keyD)
            {
                if (keyD_pressed)
                {
                    keyD_pressed = false;
                    keyD_released_by_program = false; // Reset this flag
                    LogKeyStates("Key D released by user\n");

                    if (keyA_released_by_program && !keyA_pressed && keyA_was_first)
                    {
                        // Reactivate A if it was the first key and D is still pressed
                        INPUT input = {0};
                        input.type = INPUT_KEYBOARD;
                        input.ki.wVk = keyA;
                        input.ki.dwFlags = 0;
                        SendInput(1, &input, sizeof(INPUT));
                        keyA_pressed = true;
                        keyA_released_by_program = false; // Reset this flag
                        DebugLog("Key A reactivated\n");
                    }
                }
            }
            break;

        default:
            return CallNextHookEx(hHook, nCode, wParam, lParam);
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

    // Load the icon from the current directory
    HICON hIcon = (HICON)LoadImage(NULL, TEXT("icon.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
    if (hIcon == NULL)
    {
        DebugLog("Failed to load icon.\n");
        hIcon = LoadIcon(NULL, IDI_APPLICATION);
    }

    nid.hIcon = hIcon;
    lstrcpy(nid.szTip, TEXT("SnapKey"));
    Shell_NotifyIcon(NIM_ADD, &nid);
}

void LoadKeyBindings()
{
    std::ifstream configFile("config.txt");
    if (configFile.is_open())
    {
        std::string line;
        while (std::getline(configFile, line))
        {
            if (line.find("keyA=") == 0)
            {
                keyA = line[5]; // Read the value after "keyA="
            }
            else if (line.find("keyD=") == 0)
            {
                keyD = line[5]; // Read the value after "keyD="
            }
        }
        configFile.close();
    }
    else
    {
        DebugLog("Failed to load key bindings\n");
        // Default values if the file cannot be read
        keyA = 'A';
        keyD = 'D';
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_TRAYICON:
        if (LOWORD(lParam) == WM_RBUTTONDOWN)
        {
            // Show context menu
            HMENU hMenu = CreatePopupMenu();
            AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU_ITEM, TEXT("Exit"));
            POINT pt;
            GetCursorPos(&pt);
            SetForegroundWindow(hwnd);
            TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL);
            DestroyMenu(hMenu);
        }
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_TRAY_TOGGLE_DEBUG)
        {
            // Toggle debug mode
            debugMode = !debugMode;
            DebugLog(debugMode ? "Debug mode enabled\n" : "Debug mode disabled\n");
        }
        else if (LOWORD(wParam) == ID_TRAY_EXIT_CONTEXT_MENU_ITEM)
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
