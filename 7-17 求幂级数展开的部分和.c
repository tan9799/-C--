#include<stdio.h>
#include<math.h>
int main() {
    double x;
    double result = 1.0;
    double term = 1.0;
    scanf("%lf", &x);
    int i = 1;
    while (fabs(term) >= 0.00001) {
        term = term * x / i;
        result += term;
        i++;
    }
    printf("%.4f", result);
    return 0;
}
