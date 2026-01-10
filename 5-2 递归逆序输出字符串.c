#include <stdio.h>
void reverse_str(char *p);
int main()
{
    char str[80];
    gets(str);
    reverse_str(str);
    return 0;
}
void reverse_str(char *p)
{
    if(*p)
    {
        reverse_str( p + 1 );
        putchar(*p);
    }
}
