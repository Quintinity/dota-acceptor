#include "watcher_thread.h"
#define WM_TASKBAR_ICON (WM_USER + 1)
#define MENU_EXIT 1000


static const char className[] = "DotaAcceptorWindowClass";
static NOTIFYICONDATA nid;
static HMENU menu;

void Cleanup() {
    Shell_NotifyIcon(NIM_DELETE, &nid);
}

BOOL WINAPI ConsoleHandler(DWORD event) {
    if (event == CTRL_C_EVENT) {
        Cleanup();
        return FALSE;
    }
    return TRUE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_TASKBAR_ICON && lParam == WM_RBUTTONDOWN) {
        POINT cursor;
        GetCursorPos(&cursor);
        SetForegroundWindow(hwnd);
        UINT selected = TrackPopupMenu(menu, TPM_RETURNCMD | TPM_NONOTIFY, cursor.x, cursor.y, 0, hwnd, NULL);
        if (selected == MENU_EXIT) {
            Cleanup();
            ExitProcess(0);
        }
    }
    else if (uMsg == WM_CREATE) {
        menu = CreatePopupMenu();
        AppendMenu(menu, MF_STRING, MENU_EXIT, TEXT("Exit"));
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);

    WNDCLASS wndClass = {};
    wndClass.lpfnWndProc = WindowProc;
    wndClass.style = 0;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = NULL;
    wndClass.hCursor = NULL;
    wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = className;

    if(!RegisterClass(&wndClass)) {
        std::cout << "Window registration failed" << std::endl;
        return 0;
    }

    HWND window = CreateWindow(className, "", WS_CAPTION, 0, 0, 1, 1, NULL, NULL, hInstance, NULL);
    if (window == NULL) {
        std::cout << "Failed to make window"<< std::endl;
        return 0;
    }

    HICON icon = (HICON) LoadImage(hInstance, TEXT("ICON_MAIN"), IMAGE_ICON, 48, 48, 0);

   
    nid.cbSize = sizeof(nid);
    nid.hWnd = window;
    nid.uID = 1;
    nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    nid.uCallbackMessage = WM_TASKBAR_ICON;
    nid.hIcon = icon;
    strcpy(nid.szTip, "Dota Acceptor");

    if (!Shell_NotifyIcon(NIM_ADD, &nid)) {
        std::cout << "Failed to add icon" << std::endl;
        return 0;
    }

    CreateThread(NULL, 0, watcher_thread_function, NULL, 0, NULL);
    
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0) > 0){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}