#include "utils.h"

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int getValidInt(const char* prompt, int min, int max) {
    int val;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &val) == 1) {
            if ((min == -1 || val >= min) && (max == -1 || val <= max)) {
                clearInputBuffer();
                return val;
            }
            else {
                printf("输入无效，请重新输入（范围 %d - %d）\n", min, max);
            }
        }
        else {
            printf("请输入整数！\n");
            clearInputBuffer();
        }
    }
}

float getValidFloat(const char* prompt, float min) {
    float val;
    while (1) {
        printf("%s", prompt);
        if (scanf("%f", &val) == 1 && val >= min) {
            clearInputBuffer();
            return val;
        }
        else {
            printf("输入无效，请输入大于等于 %.2f 的数字\n", min);
            clearInputBuffer();
        }
    }
}

int isDateValid(const char* date) {
    int y, m, d;
    if (strlen(date) != 10) return 0;
    if (sscanf(date, "%d-%d-%d", &y, &m, &d) != 3) return 0;
    if (m < 1 || m > 12) return 0;
    int daysInMonth[] = { 31, 28 + ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (d < 1 || d > daysInMonth[m - 1]) return 0;
    return 1;
}

int daysBetween(const char* date1, const char* date2) {
    struct tm tm1 = { 0 }, tm2 = { 0 };
    sscanf(date1, "%d-%d-%d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday);
    sscanf(date2, "%d-%d-%d", &tm2.tm_year, &tm2.tm_mon, &tm2.tm_mday);
    tm1.tm_year -= 1900; tm2.tm_year -= 1900;
    tm1.tm_mon -= 1; tm2.tm_mon -= 1;
    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);
    return (int)(difftime(t1, t2) / (60 * 60 * 24));
}

void pressAnyKey() {
    printf("\n按任意键继续...");
    _getch();
}