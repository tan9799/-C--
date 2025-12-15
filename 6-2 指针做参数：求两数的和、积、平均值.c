#include <stdio.h>

void fun(int x, int y, int *p, int *q, float  *s) ;
int main()
{
 int x,y,sum,product; float aver;
  scanf("%d%d", &x, &y);
  fun (x ,y, &sum,&product,&aver );
  printf("%d,%d,%.1f", sum,product,aver);
  return 0;
}

/* 请在这里填写答案 */
void fun(int x, int y, int *p, int *q,float *s)
{
    *p = x + y;
    *q = x * y;
    *s = *p / 2.0;
}
