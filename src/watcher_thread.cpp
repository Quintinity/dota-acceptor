#include "watcher_thread.h"
#include "screenpoints.h"
#include <cstring>
#include <string>
#include <TlHelp32.h>

#define DOTA_PROCESS_NAME "dota2.exe"

static bool dota_running_status = false;

HANDLE get_dota_process() {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return nullptr;
    }
    
    PROCESSENTRY32 entry;
    ZeroMemory(&entry, sizeof(PROCESSENTRY32));
    entry.dwSize = sizeof(PROCESSENTRY32);
    
    if (!Process32First(snapshot, &entry)) {
        std::cout << "Failed to read first thread in list." << std::endl;
        CloseHandle(snapshot);
        return nullptr;
    }
    
    while (Process32Next(snapshot, &entry)) {
        if (std::strcmp(DOTA_PROCESS_NAME, entry.szExeFile) == 0) {
            CloseHandle(snapshot);
            return OpenProcess(PROCESS_ALL_ACCESS, false, entry.th32ProcessID);
        }
    }

    CloseHandle(snapshot);
    return nullptr;
}

BOOL CALLBACK enum_windows_callback(HWND hwnd, LPARAM lparam) {
    std::pair<DWORD, HWND> *data = reinterpret_cast<std::pair<DWORD, HWND>*>(lparam);
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid == data->first) {
        data->second = hwnd;
        return false;
    }
    return true;
}

HWND get_dota_window(DWORD dota_pid) {
    std::pair<DWORD, HWND> data = std::make_pair(dota_pid, nullptr);
    EnumWindows(enum_windows_callback, (LPARAM) &data);
    return data.second;
}

std::pair<UINT32, UINT32> get_window_size(HWND hwnd) {
    RECT rect;
    GetWindowRect(hwnd, &rect);
    return std::make_pair((UINT32) (rect.right - rect.left), (UINT32) (rect.bottom - rect.top));
}

bool cmprgb(COLORREF c1, COLORREF c2) {
    return GetRValue(c1) == GetRValue(c2) && GetGValue(c1) == GetGValue(c2) && GetBValue(c1) == GetBValue(c2);
}

DWORD WINAPI watcher_thread_function(LPVOID /* data */) {
    init_screen_points();

    HANDLE dota_process_handle = nullptr;
    DWORD dota_pid = 0;

    while (true) {
        std::cout << "Waiting for Dota process..." << std::endl;
        while ((dota_process_handle = get_dota_process()) == nullptr) {
            Sleep(5000);
        }
        dota_running_status = true;
        dota_pid = GetProcessId(dota_process_handle);
        std::cout << "Dota process found! (" << dota_pid << ")" << std::endl;
        std::cout << "Waiting for window..." << std::endl;

        HWND dota_window = nullptr;
        while ((dota_window = get_dota_window(dota_pid)) == nullptr) {
            Sleep(1000);
        }
        std::cout << "Dota window found!" << std::endl;

        HDC dota_dc = GetDC(dota_window);
        if (dota_dc == NULL) {
            break;
        }

        std::cout << "Dota device context found!" << std::endl;

        while (IsWindow(dota_window)) {
            std::pair<UINT32, UINT32> window_res = get_window_size(dota_window);
            std::vector<ScreenPoint *> points = get_points_for_resolution(window_res.first, window_res.second);

            unsigned num_match = 0;
            for (ScreenPoint *point : points) {
                COLORREF color = GetPixel(dota_dc, point->getX(), point->getY());
                if (cmprgb(color, point->getColor())) {
                    num_match++;
                }
                
                /*BYTE red = GetRValue(color);
                BYTE green = GetGValue(color);
                BYTE blue = GetBValue(color);
                std::cout << "At (" << point->getX() << ", " << point->getY() << "): " << std::to_string(red) << ", " << std::to_string(green) << ", " << std::to_string(blue) << std::endl;
                */
            }

            if (num_match == points.size()) {
                std::cout << "Game is ready!" << std::endl;
                SetForegroundWindow(dota_window);
                Sleep(100);

                INPUT input;
                WORD vkey = VK_RETURN;
                input.type = INPUT_KEYBOARD;
                input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;
                input.ki.wVk = vkey;
                input.ki.dwFlags = 0;
                SendInput(1, &input, sizeof(INPUT));
            }

            Sleep(2000);
        }

        dota_pid = 0;
        dota_process_handle = nullptr;
    }

    return 0;
}



bool is_dota_running() {
    return dota_running_status;
}