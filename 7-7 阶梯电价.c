#include <stdio.h>
#include <stdlib.h>

int main()
{
    int amount;
    double money;
    scanf("%d", &amount);
    if (amount <= 50 && amount >= 0) {
        money = amount * 0.53;
        printf("cost = %.2f", money);
    } else if (amount > 50) {
        money = 50 * 0.53 + (amount - 50) * 0.58;
        printf("cost = %.2f", money);
    } else {
        printf("Invalid Value!");
    }
    return 0;
}
