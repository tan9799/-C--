#include <stdio.h>
# include <string.h>
# define MAXLINE 80
void encrypt ( char *s);
int main (void)
{
   char line [MAXLINE];

   gets(line);
   encrypt (line);
   printf ("%s\n", line);
     return 0;
}


/* 请在这里填写答案 */
void encrypt ( char *s)
{
    for(; *s != '\0'; s++)
    {
        if(*s == 'z')
            *s = 'a';
        else
            *s = *s + 1;
    }
}
