#include <stdio.h>
#define MAXS 30

char *search(char *s, char *t);
void ReadString( char s[] ); /* 裁判提供，细节不表 */

int main()
{
    char s[MAXS], t[MAXS], *pos;

    ReadString(s);
    ReadString(t);
    pos = search(s, t);
    if ( pos != NULL )
        printf("%d\n", pos - s);
    else
        printf("-1\n");

    return 0;
}

/* 你的代码将被嵌在这里 */
char *search(char *s, char *t) {
    char *p, *q, *start;
    for (start = s; *start != '\0'; start++) {
        p = start;
        q = t;
        while (*p != '\0' && *q != '\0' && *p == *q) {
            p++;
            q++;
        }
        if (*q == '\0') {
            return start;
        }
    }
    return NULL;
}
