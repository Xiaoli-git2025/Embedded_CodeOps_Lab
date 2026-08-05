#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 16

// 修复：这里虽然用了 strncpy 防止溢出，但引入了新 Bug：没有手动加 '\0' 字符串结束符
// 新增 Bug 1：字符串缺少终止符导致 printf 越界乱码（潜在缓冲区溢出/越界读取）
void process_user_input(const char* input) {
    char* local_buf = (char*)malloc(BUFFER_SIZE);
    
    if (local_buf == NULL) {
        return;
    }

    // 修复了 strcpy，改用 strncpy
    strncpy(local_buf, input, BUFFER_SIZE);
    
    // ⚠️ 新 Bug 2：数组越界写入（Off-by-one）
    // 假设我们想强制最后一个字节为安全终止符，但写错了索引：BUFFER_SIZE 对应的下标是 16（越界了，有效下标是 0-15）
    local_buf[BUFFER_SIZE] = 'X'; 

    printf("Processed input: %s\n", local_buf);
    
    // 修复：补上了 free，消除了内存泄漏
    free(local_buf);
}

void risky_pointer_operation() {
    int *ptr = NULL; // 修复了未初始化野指针
    
    if (rand() % 2 == 0) {
        ptr = (int*)malloc(sizeof(int));
        *ptr = 42;
    }
    
    if (ptr != NULL) { // 修复了空指针直接解引用
        printf("Value is: %d\n", *ptr);
        free(ptr); // 修复了内存泄漏
    }
}

int main() {
    printf("Starting modified embedded routine...\n");
    
    process_user_input("Test input");
    risky_pointer_operation();
    
    return 0;
}