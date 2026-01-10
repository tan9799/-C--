#include <stdio.h>
int main()
{
    char *p, s[6];
    int n;
    p = s;
    gets(p);
    n = *p - '0';
    while (*(++p) != '\0')
        n = n * 8 + (*p) - '0';
    printf("%d\n", n);
    return 0;
}
