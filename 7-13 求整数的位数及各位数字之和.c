#include <stdio.h>
#include <stdlib.h>

int main()
{
    int N;
    int count = 0;
    int sum = 0;
    scanf("%d", &N);
    while (N > 0) {
        int a = N % 10;
        sum += a;
        count++;
        N = N / 10;
    }
    printf("%d %d", count, sum);
    return 0;
}
