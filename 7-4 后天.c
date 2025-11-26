#include <stdio.h>
int main() {
    int D;
    scanf("%d", &D);
    int result;
    if (D<=5) {
        result = D+ 2;
    } else {
        result = D + 2 - 7;
    }
    printf("%d\n", result);
    return 0;
}
