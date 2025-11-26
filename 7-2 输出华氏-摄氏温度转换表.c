#include <stdio.h>
int main() {
    int lower, upper, fahr;
    double celsius;
    scanf("%d %d", &lower, &upper);
    if ((upper >= lower) && (upper <= 100))
    {
        printf("fahr celsius\n");
        for (fahr = lower; fahr <= upper; fahr += 2) {
            celsius = 5.0 * (fahr - 32) / 9;
            printf("%d%6.1f\n", fahr, celsius);
        }
     } else {
            printf("Invalid.\n");
     }
     return 0;
}
