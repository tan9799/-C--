#include <stdio.h>
int main() {
    int number, sum = 0;
    while (1)
    {
        scanf("%d", &number);
        if (number <= 0) {
            break;
        }
        if (number % 2 == 1) {
            sum = sum + number;
        }
    }
    printf("%d", sum);
    return 0;
}
