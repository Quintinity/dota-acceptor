#include "watcher_thread.h"

static bool dotaRunningStatus = false;

DWORD WINAPI watcher_thread_function(LPVOID /* data */) {
    std::cout << "Thread running!" << std::endl;
    return 0;
}

bool isDotaRunning() {
    return dotaRunningStatus;
}