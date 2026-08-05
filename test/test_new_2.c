#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 16

// 缺陷 1：缓冲区溢出风险 (strcpy 没有长度限制)
// 缺陷 2：内存泄漏 (动态申请的内存未释放)
void process_user_input(const char* input) {
    char* local_buf = (char*)malloc(BUFFER_SIZE);
    
    if (local_buf == NULL) {
        return;
    }

    // 危险操作：如果 input 长度大于 15，会直接导致栈/堆旁边的内存被踩踏溢出
    strcpy(local_buf, input);
    
    printf("Processed input: %s\n", local_buf);
    
    // 漏掉了 free(local_buf); 导致内存泄漏！
}

// 缺陷 3：空指针解引用 / 未初始化变量
void risky_pointer_operation() {
    int *ptr; // 未初始化，里面是野指针地址
    
    // 随机判断，可能导致程序崩溃或非法内存访问
    if (rand() % 2 == 0) {
        ptr = (int*)malloc(sizeof(int));
        *ptr = 42;
    }
    
    // 危险：如果上面 rand() 结果不走分支，ptr 就是野指针，这里直接解引用会崩
    printf("Value is: %d\n", *ptr);
    
    // 漏掉了 free(ptr)
}

int main() {
    printf("Starting vulnerable embedded routine...\n");
    
    process_user_input("This string is way too long for the tiny buffer!");
    risky_pointer_operation();
    
    return 0;
}