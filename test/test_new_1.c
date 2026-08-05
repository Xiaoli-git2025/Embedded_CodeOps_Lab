#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 16

void process_user_input(const char* input) {
    char* local_buf = (char*)malloc(BUFFER_SIZE);
    
    if (local_buf == NULL) {
        return;
    }

    strncpy(local_buf, input, BUFFER_SIZE);
    
    // ✅ 修复：将越界写入 local_buf[BUFFER_SIZE] 改为正确的最后一个字节手动加结束符
    local_buf[BUFFER_SIZE - 1] = '\0'; 

    printf("Processed input: %s\n", local_buf);
    
    free(local_buf);
}

void risky_pointer_operation() {
    int *ptr = NULL;
    
    if (rand() % 2 == 0) {
        ptr = (int*)malloc(sizeof(int));
        *ptr = 42;
    } else {
        // 故意引入新 Bug 1：如果分支没走 malloc，但后面却错误地执行了 free 或者没有正确初始化
        // 或者更隐蔽的：引入一个内存泄漏（当随机数满足条件时申请了内存，但忘记 free 且没有指针保存好，或者如下的双重释放/悬空指针风险）
    }
    
    if (ptr != NULL) {
        printf("Value is: %d\n", *ptr);
        free(ptr);
        
        // ⚠️ 新增 Bug：悬空指针二次释放 (Double Free 风险/或逻辑错误)
        // 假设我们在后面又错误地 free 了一次
        free(ptr); 
    }
}

int main() {
    printf("Starting modified embedded routine...\n");
    
    process_user_input("Test input");
    risky_pointer_operation();
    
    return 0;
}