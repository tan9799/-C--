# include <stdio.h>
#define MAXS 80
int cmp ( char *s, char *t );

int main( )
{   
    char s[MAXS], t[MAXS];

    scanf ( "%s%s", s, t );
    if ( cmp(s, t) > 0) 
    printf("\"%s\" > \"%s\"\n", s, t);
    else if ( cmp(s, t) == 0)  
    printf("\"%s\" = \"%s\"\n", s, t);
    else
    printf("\"%s\" < \"%s\"\n", s, t);
}

int cmp ( char *s, char *t )
{   
    while ( *s != '\0' && *t != '\0' ){
    if ( *s != *t ) break;
    s++, t++;
   }
   return *s - *t;
}

