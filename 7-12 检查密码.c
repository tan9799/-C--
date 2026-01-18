#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    int N;
    char password[85];  // 密码字符串，长度不超过80，多留一些空间
    
    scanf("%d", &N);
    getchar();  // 读取换行符，避免影响后面的输入
    
    for (int i = 0; i < N; i++) {
        // 使用fgets读取一行，包括可能的空格
        fgets(password, sizeof(password), stdin);
        
        // 去除末尾的换行符
        int len = strlen(password);
        if (len > 0 && password[len - 1] == '\n') {
            password[len - 1] = '\0';
            len--;
        }
        
        // 情况1：密码太短（少于6个字符）
        if (len < 6) {
            printf("Your password is tai duan le.\n");
            continue;
        }
        
        // 检查密码中的字符类型
        int has_letter = 0;  // 是否有字母
        int has_digit = 0;   // 是否有数字
        int has_invalid = 0; // 是否有非法字符
        
        for (int j = 0; j < len; j++) {
            char c = password[j];
            
            if (isalpha(c)) {        // 字母
                has_letter = 1;
            } else if (isdigit(c)) { // 数字
                has_digit = 1;
            } else if (c == '.') {   // 小数点
                // 小数点允许，不做特殊处理
            } else {                 // 非法字符
                has_invalid = 1;
                break;  // 发现非法字符就可以退出循环了
            }
        }
        
        // 情况2：有非法字符
        if (has_invalid) {
            printf("Your password is tai luan le.\n");
            continue;
        }
        
        // 情况3：只有字母没有数字
        if (has_letter && !has_digit) {
            printf("Your password needs shu zi.\n");
            continue;
        }
        
        // 情况4：只有数字没有字母
        if (!has_letter && has_digit) {
            printf("Your password needs zi mu.\n");
            continue;
        }
        
        // 情况5：密码合法
        if (has_letter && has_digit) {
            printf("Your password is wan mei.\n");
            continue;
        }
    }
    
    return 0;
}
