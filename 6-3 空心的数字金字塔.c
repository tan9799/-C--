#include <stdio.h>
void hollowPyramid ( int n );

int main()
{
    int n;

    scanf("%d", &n);
    hollowPyramid ( n );

    return 0;
}

/* 你的代码将被嵌在这里 */
void hollowPyramid(int n)
{
    int i, j, k;
    for(i = 1; i <= n; i++)
    {
        for(j = 1; j <= n - i; j++)
        {
            printf(" ");
        }
        if(i == 1)
        {
            printf("1\n");
        }
        else if(i == n)
        {
            for(k = 1; k <= 2 * n - 1; k++)
            {
                printf("%d", n);
            }
            printf("\n");
        }
        else
        {
            printf("%d", i);
            for(k = 1; k <= 2 * (i - 1) - 1; k++)
            {
                printf(" ");
            }
            printf("%d\n", i);
        }
    }
}
