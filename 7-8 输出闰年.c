#include <stdio.h>
#include <stdlib.h>

int main()
{
    int year;
    scanf("%d", &year);
    if ((year > 2100) || (year < 2001)) {
        printf("Invalid year!");
        return 0;
    }
    int flag = 0;
    for (int i = 2004; i <= year; i++) {
        if ((i % 4 == 0 && i % 100 != 0) || (i % 400 == 0)) {
            printf("%d\n", i);
            flag = 1;
        }
    }
    if (flag == 0) {
        printf("None");
    }
    return 0;
}
