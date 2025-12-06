#include<stdio.h>
int main()
{
    int n, k;
    int flag = 0;
    scanf("%d %d", &n, &k);
    int count = 0;
    for(int i = n; i >= 2; i--)
    {
        flag = 0;
        for(int j = 2; j < i; j++)
        {
            if(i % j == 0)
            {
                flag = 1;
                break;
            }
        }
        if(flag == 0)
        {
            if(count == 0)
            {
                printf("%d", i);
            }
            else
            {
                printf("+%d", i);
            }
            k--;
            count += i;
        }
        if(k == 0)
        {
            break;
        }
    }
    printf("=%d\n", count);
    return 0;
}
