#include <stdio.h>
void bubble (int *p, int n);
int main(void)
{
  int n, a[10];
    int i;
    scanf("%d", &n);
    for (i=0; i<n;i++)
        scanf("%d",&a[i]);
    bubble(a,n);
    for (i=0; i<n; i++)
        printf("%d ",a[i]);
  printf("\n");
    return 0;
}
/* 请在这里填写答案 */
void bubble (int *p, int n)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n - i -1; j++)
        {
            if(*(p + j) > *(p + j + 1) )
            {
                int temp = *(p + j);
                *(p + j) = *(p + j + 1);
                *(p + j + 1) = temp;
            }
        }
    }
}
