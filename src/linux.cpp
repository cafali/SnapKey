// g++ -o SnapKey SnapKey.cpp -mwindows -std=c++11 -static

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <iostream>
#include <cstdint>
#include <X11/extensions/XInput2.h>

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
KeyCode keyDc;
KeyCode keyAc;
unordered_map<int, KeyState> keyStates;
KeyCode activeKey = 0;
KeyCode previousKey = 0;
int opcode;
Display* display;

// Function declarations
void MessageBox(const char* message);
void KeyboardProc();
bool LoadConfig(const std::string& filename);
void SendKey(Window win, XKeyEvent* evt, int type, KeyCode keycode);
KeyCode KeyToX11(Display* display, int ascii);

int main()
{
    // Load X11 Display
    display = XOpenDisplay(NULL);
    if (display == NULL) 
    {
        cerr << "Unable to initialize X Display." << endl;
        return 0;
    }

    // Load key bindings from config file
    if (!LoadConfig("config.cfg")) {
        MessageBox("Failed to load configuration file!");
        XCloseDisplay(display);
        return 1;
    }

    // Check for XInput2
    int event, error;
    if (!XQueryExtension(display, "XInputExtension", &opcode, &event, &error)) {
        cerr << "XInput2 extension not available\n";
        XCloseDisplay(display);
        return 1;
    }

    int major = 2, minor = 0;
    if (XIQueryVersion(display, &major, &minor) == BadRequest) {
        cerr << "XI2 not available. Server supports version " << major << "." << minor << endl;
        XCloseDisplay(display);
        return -1;
    }

    keyDc = KeyToX11(display, keyD_code);
    keyAc = KeyToX11(display, keyA_code);

    // Begin keyboard handler
    KeyboardProc();

    XCloseDisplay(display);
    return 0;
}

void MessageBox(const char* message)
{
    int screen = DefaultScreen(display);
    Window rootWindow = RootWindow(display, screen);
    Window window = XCreateSimpleWindow(display, rootWindow, 200, 200, 400, 200,1, BlackPixel(display, screen), WhitePixel(display, screen));
    XSelectInput(display, window, ExposureMask | ButtonPressMask);
    GC gc = XCreateGC(display, window, 0, nullptr);
    XSetForeground(display, gc, BlackPixel(display, screen));
    XSetBackground(display, gc, WhitePixel(display, screen));
    XFontStruct* font = XLoadQueryFont(display, "fixed");
    XSetFont(display, gc, font -> fid);
    XMapWindow(display, window);

    XEvent evt;
    bool fin = false;
    while (!fin)
    {
        XNextEvent(display, &evt);
        if (evt.type == Expose)
        {
            XDrawString(display, window, gc, 50, 100, message, strlen(message));
            XDrawRectangle(display, window, gc, 150, 130, 100, 40);
            XDrawString(display, window, gc, 190, 155, "OK", 2);
        }
        if (evt.type == ButtonPress)
        {
            if (evt.xbutton.x >= 150 && evt.xbutton.x <= 250 && 
                evt.xbutton.y >= 130 && evt.xbutton.y <= 170)
                {
                    fin = true;
                }
        }
    }
    XUnloadFont(display, font -> fid);
    XFreeGC(display, gc);
    XDestroyWindow(display, window);
}

void handleKeyDown(XIDeviceEvent* keyEvent)
{
    KeyCode keyCode = keyEvent -> detail;
    if (keyCode == keyAc || keyCode == keyDc)
    {
        // get key state
        auto& keyState = keyStates[keyCode];
        if (!keyState.pressed)
        {
            // set key state + check if key is current key
            keyState.pressed = true;
            if (activeKey == 0 || activeKey == keyCode) 
            {
                activeKey = keyCode;
            }
            else
            {
                // set previous key to current key and active to current
                previousKey = activeKey;
                activeKey = keyCode;

                // send keyup input to previous
                XTestFakeKeyEvent(display, previousKey, False, CurrentTime);
                XFlush(display);
            }
        }
    }                              
}

void handleKeyUp(XIDeviceEvent* keyEvent)
{
    KeyCode keyCode = keyEvent -> detail;
    if (keyCode == keyAc || keyCode == keyDc)
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
                XTestFakeKeyEvent(display, activeKey, True, CurrentTime);
                XFlush(display);
            }
        }
    }
}

KeyCode GetKeySym(Display* display, const char* key) {
    KeySym keysym = XStringToKeysym(key);
    if (keysym == NoSymbol) {
        std::cerr << "Invalid key symbol: " << key << std::endl;
        return 0;
    }
    return XKeysymToKeycode(display, keysym);
}

KeyCode KeyToX11(Display* display, int ascii) {
    char key_char = static_cast<char>(ascii);
    char key_string[2] = {key_char, '\0'};
    KeySym keysym = XStringToKeysym(key_string);
    if (keysym == NoSymbol) {
        std::cerr << "Invalid keysym for ASCII: " << ascii << std::endl;
        return 0;
    }
    return XKeysymToKeycode(display, keysym);
}

void SendKey(Window win, XKeyEvent* evt, int type, KeyCode keycode)
{
    XKeyEvent send = *evt;
    send.window = win;
    send.same_screen = true;
    send.keycode = keycode;

    if (type == 0) 
    {
        XSendEvent(display, win, True, KeyPressMask, (XEvent*)&send);
    }
    else if (type == 1)
    {
        XSendEvent(display, win, True, KeyReleaseMask, (XEvent*)&send);
    }
    XFlush(display);
}

void KeyboardProc()
{
    Window root = DefaultRootWindow(display);
    XIEventMask evmask;
    unsigned char mask[(XI_LASTEVENT + 7)/8] = { 0 };
    evmask.deviceid = XIAllDevices;
    evmask.mask_len = sizeof(mask);
    evmask.mask = mask;
    XISetMask(mask, XI_KeyPress);
    XISetMask(mask, XI_KeyRelease);

    XISelectEvents(display, root, &evmask, 1);
    XFlush(display);

    while (true) {
        XEvent ev;
        XNextEvent(display, &ev);

        if (ev.xcookie.type == GenericEvent && ev.xcookie.extension == opcode) {
            if (XGetEventData(display, &ev.xcookie)) {
                XIDeviceEvent* xiev = (XIDeviceEvent*)ev.xcookie.data;
                if (ev.xcookie.evtype == XI_KeyPress) {
                    std::cout << "Key pressed: " << xiev->detail << std::endl;
                    handleKeyDown(xiev);
                } else if (ev.xcookie.evtype == XI_KeyRelease) {
                    std::cout << "Key released: " << xiev->detail << std::endl;
                    handleKeyUp(xiev);
                }
                XFreeEventData(display, &ev.xcookie);
            }
        }
    }
}

/*
void InitNotifyIconData(HWND hwnd)
{
    memset(&nid, 0, sizeof(NOTIFYICONDATA));

    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = ID_TRAY_APP_ICON;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;

    // Load the icon from the current directory
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
*/

bool LoadConfig(const std::string& filename)
{
    std::ifstream configFile(filename);
    if (!configFile.is_open()) {
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
