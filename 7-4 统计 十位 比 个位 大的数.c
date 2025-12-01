#include<stdio.h>
int main()
{
    int m, n, sum;
    scanf("%d %d", &m, &n);
    for(int i = m; i <= n; i++) {
        int c = i % 10;
        int d = (i / 10) % 10;
        if(d > c) {
            sum += i;
        }
    }
    printf("%d", sum);
    printf("\n");
    return 0;
}
