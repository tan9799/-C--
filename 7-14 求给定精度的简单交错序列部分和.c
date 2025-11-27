#include <stdio.h>
#include <stdlib.h>
#include<math.h>
int main()
{
   double eps;
    scanf("%lf", &eps);
    double sum = 0.0;
    int sign = 1;
    int denominator = 1;
    double term;
    do {
        term = 1.0 / denominator;
        sum += sign * term;
        sign = -sign;
        denominator += 3;
    } while (fabs(term) > eps);
    printf("sum = %.6f", sum);
    return 0;
}
