#include <stdio.h>

int main() {
    int k, m, n;
    scanf("%d %d", &k, &m);
    n = k;
    k = m;
    m = n;
    printf("a=%d,b=%d\n", k,m);
    return 0;
}
