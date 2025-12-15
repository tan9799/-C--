#include<stdio.h>
int main()
{
    char s[50],a,b,*p; //程序要实现用字符变量b替换字符串s中的字符变量a
    gets(s);
    scanf("%c %c",&a,&b);
    for(p = s; *p != '\0'; p++)
        if(*p == a)
            *p = b;
    puts(s);
    return 0;
}
