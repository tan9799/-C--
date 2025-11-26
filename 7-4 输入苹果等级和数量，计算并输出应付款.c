#include <stdio.h>

int main() {
    int grade;
    double amount, money;
    scanf("%d %lf", &grade, &amount);
    switch(grade)
    {
        case 1:
            money = 5.50 * amount;
            printf("%.2f", money);
            break;
        case 2:
            money = 4.30 * amount;
            printf("%.2f", money);
            break;
        case 3:
            money = 3.00 * amount;
            printf("%.2f", money);
            break;
        case 4:
            money = 2.50 * amount;
            printf("%.2f", money);
            break;
        default:
            printf("Data Error!");
            break;
    }
    return 0;
}
