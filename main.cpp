#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "android_ion.hpp"
int mem_fd = -1;
void *addr = nullptr;
size_t mem_size = 1024;
int main() {
    int ret = GLOBAL_ANDROID_ION.init();
    if (ret) {
        printf("android ion init error code:%d\n", ret);
        return -1;
    }
    ret = GLOBAL_ANDROID_ION.alloc_memory(mem_size, &mem_fd, &addr);
    if (ret) {
        printf("android ion alloc memory error code:%d\n", ret);
        return -2;
    }
    printf("android ion alloc memory ok, memory fd:%d, virtual address:%p\n", mem_fd, addr);
    int pid = fork();
    if (pid > 0) {   		// parent process
        sleep(1);
        printf("parent process read:%s\n", (char*)addr);
        wait(NULL);
    }
    else if (0 == pid) {       // child process
        static const char* str = "I am a child process!";
        size_t len = strlen(str);
        if (len < mem_size) {
            memcpy((char*)addr, str, len);
        }
    }

    return 0;
}