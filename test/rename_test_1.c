#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 16

void process_user_input(const char* input) {
    char* local_buf = (char*)malloc(BUFFER_SIZE);
    
    if (local_buf == NULL) {
        return;-
    }

    // ⚠️ 引入新 Bug 1：如果 input 为空指针或非法格式，我们直接 return，
    // 但是忘记了在此处调用 free(local_buf)，导致内存泄漏（Memory Leak）！
    if (input == NULL) {
        return; 
    }

    strncpy(local_buf, input, BUFFER_SIZE);
    local_buf[BUFFER_SIZE - 1] = '\0'; 

    printf("Processed input: %s\n", local_buf);
    
    free(local_buf);
}

void risky_pointer_operation() {
    int *ptr = NULL;
    
    if (rand() % 2 == 0) {
        ptr = (int*)malloc(sizeof(int));
        *ptr = 42;
    }
    
    if (ptr != NULL) {
        printf("Value is: %d\n", *ptr);
        free(ptr);
        
        // ✅ 修复：删除了上一轮导致双重释放（Double Free）的错误代码
        // ptr = NULL; // 养成好习惯顺便置空
    }
}

int main() {
    printf("Starting modified embedded routine...\n");
    
    process_user_input("Test input");
    risky_pointer_operation();
    
    return 0;
}