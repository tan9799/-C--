#include<stdio.h>
void fun(char s[]);
int  main()
 {char s[80];
  gets(s);
  fun(s);
  puts(s);
  return 0;
  }

/* 请在这里填写答案 */
void fun(char s[])
{
    int len = 0;
    while(s[len] != '\0') {
        len++;
    }
    for(int i = 0, j = len - 1; i < j; i++, j--)
    {
        char temp = s[i];
        s[i] = s[j];
        s[j] = temp;
    }
}
