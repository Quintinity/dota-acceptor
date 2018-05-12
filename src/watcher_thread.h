#ifndef DOTA_WATCHER_THREAD_H
#define DOTA_WATCHER_THREAD_H

#include <windows.h>
#include <iostream>


bool is_dota_running();
DWORD WINAPI watcher_thread_function(LPVOID);

#endif