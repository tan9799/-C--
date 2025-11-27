#include<stdio.h>
int main() {
    int num;
    int count = 0;
    while(scanf("%d", &num) != EOF) {
        count++;
        if (num == 250) {
            printf("%d\n", count);
            break;
        }
    }
    return 0;
}
