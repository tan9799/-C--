#include <stdio.h>
int main() {
        int Amount = 1, N;
        int i;
        scanf("%d", &N);
        for (i = N - 1; i >= 1 ;i--)
        {
            Amount = 2 * (Amount + 1);
        }
        printf("%d\n", Amount);
        return 0;
}
