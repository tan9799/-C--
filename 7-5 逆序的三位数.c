#include <stdio.h>
#include <stdlib.h>

int main()
{
    int a, b, c, m;
    scanf("%d", &m);
    a = m / 100;
    b= (m / 10) % 10;
    c= m - 100 * a - 10 * b;
    if (c == 0 && b == 0) {
        printf("%d", a);
    } else if (c == 0 && b != 0) {
        int result1;
        result1 = a + b * 10;
        printf("%d", result1);
    } else if (c != 0) {
        int result2;
        result2 = c * 100 + b * 10 + a;
        printf("%d", result2);
    }
    return 0;
}
