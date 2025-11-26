#include <stdio.h>

int main() {
    char ch;
    scanf("%c", &ch);

    if (ch >= 'A' && ch <= 'Z') {
        printf("%d", ch);
    } else if (ch >= 'a' && ch <= 'z') {
        printf("%c", ch - 32);
    }

    return 0;
}
