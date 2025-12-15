#include <stdio.h>
#include <string.h>

void delchar(char *str, char c);

int main(void)
{
    int repeat;
    scanf("%d", &repeat);
    getchar();

    for (int i = 0; i < repeat; i++)
    {
        char str[81];
        char c;
        int j = 0;
        char ch;
        while ((ch = getchar()) != '\n' && j < 80) {
            str[j++] = ch;
        }
        str[j] = '\0';
        scanf("%c", &c);
        getchar();
        delchar(str, c);

        printf("result: %s\n", str);
    }

    return 0;
}

void delchar(char *str, char c)
{
    int i = 0, j = 0;

    while (str[i] != '\0') {
        if (str[i] != c) {
            str[j] = str[i];
            j++;
        }
        i++;
    }
    str[j] = '\0';
}
