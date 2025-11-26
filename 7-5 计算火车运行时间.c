#include <stdio.h>

int main() {
    int a, b;
    scanf("%d %d", &a, &b);
    int d = a / 100;
    int c = a % 100;
    int e = b / 100;
    int f = b % 100;
    int total = (e * 60 + f) - (d * 60 + c);
    int minutes = total % 60;
    int hours = total / 60;
    printf("%02d:%02d", hours, minutes);
    return 0;
}
