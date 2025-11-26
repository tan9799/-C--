#include <stdio.h>
int main() {
    int H;
    scanf("%d", &H);
    float W = (H - 100) * 0.9 * 2;
    printf("%.1f", W);
    return 0;
}
