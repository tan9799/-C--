# include <stdio.h>
#define MAXS 80
void cpy( char *s, char *t );

int main( )
{
    char s[MAXS], t[MAXS];

    scanf("%s", t );
    cpy (s, t);
    printf("%s\n", s);
}

void cpy( char *s, char *t )
{
    while(*t != '\0')
    {
        *s = *t, s++, t++;
    }
    *s = '\0';
}
