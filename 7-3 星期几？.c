#include <stdio.h>
#include <string.h>

// 基姆拉尔森计算公式（Kim Larsen calculation formula）
// 证明：该公式是蔡勒公式的简化形式，经过数学推导证明对于1582年后的日期都适用
// 对于1900-2100年范围内的日期，计算结果是准确的
int day_of_week(int y, int m, int d) {
    // 1月和2月当作上一年的13月和14月处理
    if (m == 1 || m == 2) {
        m += 12;
        y--;
    }
    
    int week = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7;
    
    // 转换结果：0-6分别对应星期日到星期六
    return week;
}

int main() {
    int year, month, day;
    
    // 循环读取多组数据，直到EOF
    while (scanf("%d/%d/%d", &year, &month, &day) != EOF) {
        int week = day_of_week(year, month, day);
        
        // 根据计算结果输出对应的星期几
        switch (week) {
            case 6: printf("sunday\n"); break;
            case 0: printf("monday\n"); break;
            case 1: printf("tuesday\n"); break;
            case 2: printf("wednesday\n"); break;
            case 3: printf("thursday\n"); break;
            case 4: printf("friday\n"); break;
            case 5: printf("saturday\n"); break;
        }
    }
    
    return 0;
}
