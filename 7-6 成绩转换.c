#include <stdio.h>

int main() {
    int grade;
    char a;
    scanf("%d", &grade);
    if (grade <= 100 && grade >= 90) {
        a = 'A';
    } else if (grade < 90 && grade >=80) {
        a = 'B';
    } else if (grade < 80 && grade >= 70) {
        a = 'C';
    } else if (grade < 70 && grade >= 60) {
        a = 'D';
    } else {
        a = 'E';
    }
    printf("%c\n", a);
    return 0;
}
