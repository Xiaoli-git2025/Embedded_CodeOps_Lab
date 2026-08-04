#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 模拟一个简单的缓冲区处理函数
void process_buffer(const char *input) {
    if (input == NULL) {
        return;
    }

    // 1. 动态分配内存
    char *buffer = (char *)malloc(100 * sizeof(char));
    
    // 注意：这里故意没有检查 buffer 是否为 NULL
    strcpy(buffer, input);

    printf("Processed string: %s\n", buffer);

    // 2. 故意缺失 free(buffer); 触发内存泄漏 Bug
}

int main() {
    printf("Starting Cppcheck & LLM Agent Test...\n");

    const char *sample_text = "Hello CodeOps Lab!";
    process_buffer(sample_text);

    return 0;
}