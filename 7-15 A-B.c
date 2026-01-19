#include <stdio.h>
#include <string.h>

int main()
{
    char A[10001];
    char B[10001];
    char C[10001];
    
    fgets(A, sizeof(A), stdin);
    fgets(B, sizeof(B), stdin);
    
    // 移除换行符
    A[strcspn(A, "\n")] = '\0';
    B[strcspn(B, "\n")] = '\0';
    
    char* p = A;  // 指向A的指针
    char* m = C;  // 指向C的指针
    
    // 遍历A中的每个字符
    while (*p != '\0')
    {
        char* q = B;  // 每次比较前，q重新指向B的开头
        int found = 0;  // 标记是否在B中找到当前字符
        
        // 在B中查找当前字符
        while (*q != '\0')
        {
            if (*q == *p)
            {
                found = 1;
                break;
            }
            q++;
        }
        
        // 如果没找到，复制到C
        if (!found)
        {
            *m = *p;
            m++;
        }
        
        p++;  // 移动到A的下一个字符
    }
    
    *m = '\0';  // 字符串结束符
    
    puts(C);
    return 0;	
}
