#include "watcher_thread.h"

int main() {
    HANDLE thread = CreateThread(NULL, 0, watcher_thread_function, NULL, 0, NULL);
    if (thread) {
        WaitForSingleObject(thread, INFINITE);
        printf("Thread terminated\n");
    }
}