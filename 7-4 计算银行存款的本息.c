#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    int money, year;
    double rate, sum;
    scanf("%d %d %lf", &money, &year, &rate);
    sum = money * pow(1 + rate,  year) ;
    printf("sum = %.2f",  sum);
    return 0;
}
