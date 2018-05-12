#include "watcher_thread.h"
#define WM_TASKBAR_ICON (WM_APP + 1)

static const char className[] = "DotaAcceptorWindowClass";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {

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

    NOTIFYICONDATA nid;
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

    HANDLE thread = CreateThread(NULL, 0, watcher_thread_function, NULL, 0, NULL);
    if (thread) {
        WaitForSingleObject(thread, INFINITE);
    }

    return 0;
}