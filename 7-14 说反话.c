#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char str[500001];  // 多分配1个字符给结束符
    char *words[250000];  // 存储单词指针，假设最多250000个单词
    int word_count = 0;
    
    // 读取整行
    fgets(str, sizeof(str), stdin);
    
    int len = strlen(str);
    // 去除末尾换行符
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
        len--;
    }
    
    // 从后向前处理字符串，这样可以直接找到单词
    int i = len - 1;
    
    while (i >= 0) {
        // 跳过末尾的空格
        while (i >= 0 && str[i] == ' ') {
            i--;
        }
        
        // 如果已经没有字符了，退出循环
        if (i < 0) break;
        
        // 记录单词的结束位置
        int end = i;
        
        // 找到单词的起始位置
        while (i >= 0 && str[i] != ' ') {
            i--;
        }
        
        // 记录单词（i+1 到 end 是一个单词）
        words[word_count] = &str[i + 1];
        
        // 在单词结尾添加结束符（替换空格）
        if (i >= 0) {
            str[i + 1 + (end - i)] = '\0';  // 临时修改，但后面会恢复
        }
        
        word_count++;
        
        // 跳过空格
        while (i >= 0 && str[i] == ' ') {
            i--;
        }
    }
    
    // 输出结果
    if (word_count > 0) {
        // 输出第一个单词
        printf("%s", words[0]);
        
        // 输出后续单词（前面加空格）
        for (int j = 1; j < word_count; j++) {
            printf(" %s", words[j]);
        }
    }
    
    printf("\n");
    return 0;
}
