#include <stdio.h>
#include <stdlib.h>

int main()
{
    int N;
    scanf("%d", &N);
    if (N == 1) {
        printf("1");
        return 0;
    }
    int month = 2;
    int prev = 1;
    int curr = 1;
    while (curr < N) {
        int next = prev  + curr;
        prev = curr;
        curr = next;
        month++;
    }
    printf("%d", month);
    return 0;
}
