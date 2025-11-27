#include <stdio.h>
#include <stdlib.h>

int main()
{
    int M, N, min, q, p;
    scanf("%d %d", &M, &N);
    if (M >= N) {
        min = N;
    } else {
        min = M;
    }
    for (int i = min; i >=1; i--) {
        if ((N % i == 0) && (M % i == 0)) {
            q = i;
            break;
        }
    }
    p = M * N / q;
    printf("%d %d", q, p);
    return 0;
}
