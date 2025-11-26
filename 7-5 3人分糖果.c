#include <stdio.h>
int main() {
    int a = 8, b = 9, c = 10;
    a = a / 3;
    b = b + a;
    c = c + a;
    b = b / 3;
    a = a + b;
    c = c + b;
    c = c / 3;
    a = a + c;
    b = b + c;
    printf("%d %d %d", a, b, c);
    return 0;
}
