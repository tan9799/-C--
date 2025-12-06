#include<stdio.h>
int main()
{
    int m, n;
    int flag = 0;
    scanf("%d %d", &m, &n);
    for(int i = m; i <= n; i++)
    {
        int sum = 0;
        for(int j = 1; j <= i / 2; j++)
        {
            if(i % j == 0)
            {
                sum += j;
            }
        }
        if (sum == i)
        {
            flag = 1;
            printf("%d = 1", i);
            for(int j = 2; j <= i / 2; j++)
            {
                if(i % j == 0)
                {
                    printf(" + %d", j);
                }
            }
            printf("\n");
        }
    }
    if(flag == 0)
    {
        printf("None");
    }
    return 0;
}
