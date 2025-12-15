#include  <stdio.h>
void fun (char  *s, char  *t);
int main()
{  char   s[100], t[100];
  scanf("%s", s);
  fun(s, t);
  printf("The result is: %s\n", t);
  return 0;
}


/* 请在这里填写答案 */
void fun(char *s, char *t)
{
    int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    for (int j = 0; j < len; j++)
        t[j] = s[j];
    for (int j = len - 1, i = len; j >= 0; j--) {
        t[i] = s[j];
        i++;
    }
    t[len * 2] = '\0';
}
