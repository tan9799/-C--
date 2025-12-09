#include <stdio.h>

char Encode(char plain, int key);
void Encrypt(int key);

int main()
{
    int key;
    scanf(" %d%*c", &key);
    Encrypt(key);
    putchar('\n');
    return 0;
}

/* 你提交的代码将被嵌在这里 */
void Encrypt(int key) {
    char ch = getchar();

    // 递归终止条件：遇到换行符
    if (ch == '\n') {
        return;
    }

    // 先递归读取和处理后续字符
    Encrypt(key);

    // 递归返回后处理当前字符（实现倒置）
    // 题目说"均为小写字母"，但样例显示需要处理大小写
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
        putchar(Encode(ch, key));
    } else {
        putchar(ch);
    }
}
