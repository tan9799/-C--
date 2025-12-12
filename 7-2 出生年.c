#include <stdio.h>
#include <string.h>

int main() {
    int y, n;
    scanf("%d %d", &y, &n);

    int year = y;
    int age = 0;

    while (1) {
        char s[5];
        sprintf(s, "%04d", year);

        int count[10] = {0};
        for (int i = 0; i < 4; i++) {
            count[s[i] - '0']++;
        }

        int diff = 0;
        for (int i = 0; i < 10; i++) {
            if (count[i] > 0) {
                diff++;
            }
        }

        if (diff == n) {
            printf("%d %04d\n", age, year);
            break;
        }

        year++;
        age++;
    }

    return 0;
}
