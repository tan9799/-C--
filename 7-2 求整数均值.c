#include <stdio.h>

int main() {
    int a,b,c,d;
    scanf("%d %d %d %d", &a, &b, &c, &d);
    int Sum = a + b +c +d;
    float Average = Sum / 4.0;
    printf("Sum = %d; Average = %.1f", Sum, Average) ;
    return 0;
}
