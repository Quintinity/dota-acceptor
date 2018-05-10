#ifndef DOTA_WATCHER_THREAD
#define DOTA_WATCHER_THREAD

#include <windows.h>
#include <iostream>


bool isDotaRunning();
DWORD WINAPI watcher_thread_function(LPVOID);

#endif